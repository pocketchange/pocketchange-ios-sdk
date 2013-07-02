## v1.0.4 (2013-04-08)

  * Resolved a potential race condition causing the exception 'NSInvalidArgumentException', reason: '-[__NSCFSet addObject:]: attempt to insert nil'

## v1.0.5 (2013-04-15)

  * Rewards notificaton now uses native iOS views
  * Added `-(void)openShop` method

## v1.0.6 (2013-04-22)

  * Removed some unnecessary copying of variables

## v1.0.7 (2013-05-13)

  * Fixed the repeat reward notifications bug
  * Fixed two rare crashes on certain devices

## v1.0.8 (2013-05-20)

  * Notifications no longer have the dark background

## v1.0.9 (2013-06-04)

  * Fixed a potential crash on iOS 4

## v1.0.10 (2013-06-20)

  * New opt-in flow
  * Fixed a possible bug where a notification would still be pending after a user had visited the store

## v1.0.11 (2013-06-28)

  * Fixed a bug present only in sandbox mode that affected buttons in the reward dialog
  * Fixed the keyboard orientation bug across all iOS versions

## v1.1.0 (2013-07-02)

  * The openShop method now returns a boolean indicating whether the shop was opened successfully
  * Added a showNotification: method that takes a boolean indicating whether to rate limit consecutive notifications (default is YES)
