# Pocket Change's iOS SDK

Follow the instructions below to integrate the SDK.

Compatibility: iOS 4.3+.

Prerequisites: [Xcode][1].

**After you finish testing please submit your .ipa file to your iOS representative for QA with test mode enabled.**


## Step 1: Obtain an API Key for Your Application

In order to integrate the Pocket Change SDK, you must first obtain an API Key. To locate your application's API key, select your application from the dashboard, then navigate to Settings » Keys » API Key. Each application has a different API key; if you have multiple applications, be sure to use the correct key for each one.

## Step 2: Download the SDK

You can either clone the GitHub repository:

```sh
git clone git://github.com/pocketchange/pocketchange-ios-sdk.git
```

Or download and extract the files here: <http://github.com/pocketchange/pocketchange-ios-sdk/zipball/master>.

## Step 3: Add the SDK to Your Project

To use the SDK, you must make it visible to the compiler.

### 1. Add the SDK to your project
Drag the `PocketChangeSDK` folder over to your XCode Project.

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-1.png" alt="Dragging SDK Over" />

When the file import dialog comes up, select "Copy items into destination group's folder", "Create groups for any added folders", and check off whichever targets you want to add the PocketChangeSDK to.
<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-2.png" alt="Import file dialog" />

### 2. Verify that the SDK library has been linked to your application's target

Make sure that `libPocketChangeSDK.a` is among the linked binaries.

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-3.png" alt="Initial linked binaries" />

### 3. Add the SDK Framework Dependencies

The SDK depends on multiple external frameworks. To incorporate these frameworks into the project, under the "Link Binary with Libraries" phase, add the following frameworks if they have not already been added (and `libPocketChangeSDK.a` if it wasn't among the frameworks in the previous step):

  * AdSupport.framework
  * CoreData.framework
  * CoreGraphics.framework
  * CoreTelephony.framework
  * QuartzCore.framework
  * Security.framework
  * SystemConfiguration.framework

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-4.png" alt="Adding the frameworks dialog" />

Since AdSupport.framework is for iOS 6+, mark it as optional to allow your app to work on previous versions of iOS. The SDK automatically adjusts its behavior to function on devices which cannot use this framework.

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-5.png" alt="Link Phase after adding all required frameworks and libraries" />

### 4. Verify the library search path

Your library search path should have an entry like `"$(SRCROOT)/PocketChangeSDK"` that XCode automatically added. If not, go ahead and add it.

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-6.png" alt="Library search paths" />

### 5. Update Your Project's Linker Flags
To ensure that the linker correctly includes the SDK's code, in your project's "Build Settings" tab, search for "other_ld", and you should see an entry under Linking » Other Linker Flags. Add the following flag to "Other Linker Flags":
  * `-ObjC`

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/step3-7.png" alt="Other Linker Flags" />

### 6. Edit Your Application's Info.plist
In your application's Info.plist file (typically named `<application name>-Info.plist`), add a row of type string whose key is `com.pocketchange.pocketchange-ios-sdk.APIKey` and whose value is the API key you obtained in step 1.

If you have not already configured an appropriate display name for your application, search for `CFBundleDisplayName` in your plist file and change the value in the highlighted row to an appropriate user-facing name. The SDK uses your application's `CFBundleDisplayName` when referencing your application in user interface components.

## Step 4: Integrate the SDK into Your Application
In your application delegate, import the `PocketChangeSDK.h` header: 
```objective-c
#import "PocketChangeSDK.h"
```

In your delegate's application:didFinishLaunchingWithOptions: method, call:
```objective-c
[[PocketChangeSDK sharedInstance] applicationDidFinishLaunching];
```

**Do not cache the value of [PocketChangeSDK sharedInstance] between function invocations, as it may change.**

Visual notifications may accompany certain rewards. In order to avoid interfering with your application, the SDK queues these notifications so that you can deliver them at convenient times. Your application must periodically display these notifications, or users will be unaware of their rewards.

When the SDK enqueues a notification, it posts a `PocketChangeNotificationIsPendingNotification` to the default notification center. Your application should observe these notifications and respond by invoking:
```objective-c
[[PocketChangeSDK sharedInstance] showNotification];
```

As the showNotification method interacts with the user interface, you must invoke it on the main thread. **All methods in the PocketChangeSDK class must be invoked on the main thread.** The SDK posts its notifications from the main thread, so your observers will be invoked on the main thread unless you alter the default invocation context by providing a custom operation queue.

The `showNotification` method returns a `BOOL` indicating whether the SDK will show a notification. In certain circumstances, such as when the user lacks network connectivity, notifications may expire. Therefore, even if your application observes a `PocketChangeNotificationIsPendingNotification`, `showNotification` may not show a notification. 

If your application cannot synchronously respond to the `PocketChangeNotificationIsPendingNotification`, consider invoking the `showNotification` method periodically at natural transition points in your application's workflow.

After showing a notification, the SDK posts a `PocketChangeNotificationWasShownNotification` to the default notification center. Your application should respond to this notification by pausing any expensive tasks, such as animation timers.

When the user dismisses a notification, the SDK posts a `PocketChangeNotificationWasDismissedNotification`. Observing this notification allows your application to resume any animations and other expensive tasks it may have paused while showing the notification.

Visually, notifications function similarly to UIAlertViews. To aid in determining which tasks to pause upon showing a notification, you can conceive of the notification as an alert.

Unlike alerts, notifications integrate with iOS 6 state saving. If the system restores your application from a saved state, the SDK will automatically restore any previously visible notification, and will eventually post a `PocketChangeNotificationWasDismissedNotification` when the user closes the notification. **When restoring a notification from a saved state, the SDK does not post a `PocketChangeNotificationWasShownNotification`**, as it posted one during the previous application cycle. Therefore, **if your application supports state restoration, you must account for any currently visible notification when restoring your user interface.** As your application's interface must already serialize and restore its state to participate in state restoration, this SDK behavior should not impose much, if any, additional burden.

The following skeleton application delegate demonstrates a basic SDK integration:

```objective-c
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [[PocketChangeSDK sharedInstance] applicationDidFinishLaunching];
    NSArray *pocketChangeNotifications = [NSArray
                                          arrayWithObjects:
                                          PocketChangeNotificationIsPendingNotification,
                                          PocketChangeNotificationWasShownNotification,
                                          PocketChangeNotificationWasDismissedNotification,
                                          nil];
    NSNotificationCenter *defaultNotificationCenter = [NSNotificationCenter defaultCenter];
    for (NSString *notification in pocketChangeNotifications) {
        [defaultNotificationCenter
         addObserver:self
         selector:@selector(handlePocketChangeNotification:)
         name:notification
         object:nil];
    }
    // ... other startup code ...
    return YES;
}

- (void)handlePocketChangeNotification:(NSNotification *)notification
{
    NSString *name = [notification name];
    if ([name
         isEqualToString:PocketChangeNotificationIsPendingNotification]) {
        [[PocketChangeSDK sharedInstance] showNotification];
    } else if ([name
                isEqualToString:PocketChangeNotificationWasShownNotification]) {
        [self pauseInterface];
    } else if ([name
                isEqualToString:PocketChangeNotificationWasDismissedNotification]) {
        [self resumeInterface];
    }
}

- (void)pauseInterface
{
    // pause timers and other expensive UI tasks
}

- (void)resumeInterface
{
    // resume timers and other expensive UI tasks
}
```

## Step 5 (Optional): Integrate Event Rewards

If you've set up event-based rewards with your account representative, you can grant a reward in response to an event using:
```objective-c
NSString *rewardId = @"a reward identifier";
[[PocketChangeSDK sharedInstance] grantReward:rewardId];
```

the rewardId is the referenceId you received from your account representive for the event in question. 

**Your application must be in <a href="#testing">sandbox mode</a> to test event-based rewards.**

## Step 6 (Optional): Added a Pocket Change Shop button

SDK version 1.0.5 adds the ability to open the shop from any point within your app, without having to wait for a notification to display. To integrate, check the `canOpenShop` property in the PocketChangeSDK instance and, if it returns `YES`, you may call `-(void)openShop`.

The value of `canOpenShop` can change at any point and is initially set to `NO`, so you should set up your application to observe `PocketChangeShopAvailabilityChangedNotification` and respond to it (e.g., add/remove a "Shop" button as needed). For example:

```objective-c
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [[NSNotificationCenter defaultCenter] addObserver:self 
                                             selector:@selector(setUpPocketChangeButton) 
                                                 name:PocketChangeShopAvailabilityChangedNotification 
                                               object:nil];

    ...
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self 
                                                    name:PocketChangeShopAvailabilityChangedNotification
                                                  object:nil];

    ...
}

- (void)setUpPocketChangeButton
{
    if ([[PocketChangeSDK sharedInstance] canOpenShop]) {
        // Add shop button
    }
    else {
        // Remove shop button
    }
}
```

## Testing
You can use sandbox mode to help validate your integration: In sandbox mode, the SDK grants unlimited rewards so that you can confirm your application's behavior after a reward has been granted. 

To enable sandbox mode, in your application's Info.plist file, add a row of type boolean whose key is "com.pocketchange.pocketchange-ios-sdk.sandboxMode"
and whose value is YES. See example below.

<img src="https://raw.github.com/pocketchange/pocketchange-ios-sdk/master/docs/images/test_mode.png" alt="Enabling Test Mode" />

To trigger additional daily gift grants in sandbox mode, you can either restart your application, or execute:
```objective-c
[[PocketChangeSDK sharedInstance] grantReward:@"daily"];
```

**You must disable sandbox mode before submiting your application to the App Store, or users will not receive real rewards.**

[1]: https://developer.apple.com/xcode/
