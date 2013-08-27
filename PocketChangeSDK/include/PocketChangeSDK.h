//
//  PocketChangeSDK.h
//  pocketchange-ios-sdk
//
//  Copyright (c) 2012 Lunch Money Co. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifndef _POCKET_CHANGE_SDK_H_
#define _POCKET_CHANGE_SDK_H_

OBJC_EXPORT NSString * const PocketChangeSDKVersion;
OBJC_EXPORT NSString * const PocketChangeSDKReadyNotification;
OBJC_EXPORT NSString * const PocketChangeShopAvailabilityChangedNotification;
OBJC_EXPORT NSString * const PocketChangeRewardGrantedNotification;
OBJC_EXPORT NSString * const PocketChangeRewardGrantedNotificationKeyGrantedRewardIDs;
OBJC_EXPORT NSString * const PocketChangeNotificationIsPendingNotification;
OBJC_EXPORT NSString * const PocketChangeNotificationWasShownNotification;
OBJC_EXPORT NSString * const PocketChangeNotificationWasDismissedNotification;

@interface PocketChangeSDK : NSObject

/**
 *  A boolean indicating whether the Pocket Change shop is available for the current user and device.
 */
@property (nonatomic, readonly) BOOL canOpenShop;

- (void)applicationDidFinishLaunching;

/**
 *  Grants a reward to the user.
 *
 *  @param rewardId A string identifying the reward type. This must be set up on the Pocket Change servers before you are able to grant it.
 */
- (void)grantReward:(NSString *)rewardId;
- (void)grantReward:(NSString *)rewardId amount:(NSUInteger)amount;

/**
 *  Check whether the current user has any pending notifications to be shown.
 *
 *  @return A boolean indicating whether there is a pending notification.
 */
- (BOOL)hasPendingNotification;

/**
 *  Displays any pending notifications and clears the pending notification queue.
 *
 *  @return A boolean indicating whether any notification was shown. This may be `NO` if a notification was shown very recently and the SDK determines that showing another notification would be a poor user experience.
 *
 *  @see -showNotification:
 */
- (BOOL)showNotification;

/**
 *  Similar to `-showNotification`, but allows you to disable the default rate limiting. Most developers will not need to use this.
 *
 *  @param shouldRateLimit A boolean indicating whether the default rate limiting should apply.
 *
 *  @return A boolean indicating whether any notification was shown.
 */
- (BOOL)showNotification:(BOOL)shouldRateLimit;

/**
 *  Open the Pocket Change shop, if it is available. If the user has not opted in and you call this method, an opt-in dialog will be shown before the store is opened.
 *
 *  @return A boolean indicating whether the store or an opt-in dialog were able to be shown.
 */
- (BOOL)openShop;

/**
 *  Returns the singleton instance of the `PocketChangeSDK`. It is not necessary to store the return value in any instance variables, since the SDK retains itself.
 *
 *  @return The singleton instance of the Pocket Change SDK.
 */
+ (PocketChangeSDK *)sharedInstance;

@end


#endif // #ifndef _POCKET_CHANGE_SDK_H_