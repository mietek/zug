typedef struct {
	ZAction action;
	UInt32 anchorCount[Z_ANCHOR_COUNT];
	ZIndex screenIndex;
} ZKeyEventState;


@interface ZAgent : NSObject {
	NSStatusItem *statusItem;
}

@end


Boolean ZHandleKeyEvent(CGEventRef event, void *handlerData);

ZAnchor ZKeycodeToAnchor(UInt32 keycode);
Boolean ZIsKeycodeCenter(UInt32 keycode);
Boolean ZIsKeycodeLeft(UInt32 keycode);
Boolean ZIsKeycodeRight(UInt32 keycode);
Boolean ZIsKeycodeTop(UInt32 keycode);
Boolean ZIsKeycodeBottom(UInt32 keycode);
Boolean ZIsKeycodeTopLeft(UInt32 keycode);
Boolean ZIsKeycodeTopRight(UInt32 keycode);
Boolean ZIsKeycodeBottomLeft(UInt32 keycode);
Boolean ZIsKeycodeBottomRight(UInt32 keycode);
ZIndex ZKeycodeToIndex(UInt32 keycode);
Boolean ZIsKeycode1(UInt32 keycode);
Boolean ZIsKeycode2(UInt32 keycode);
Boolean ZIsKeycode3(UInt32 keycode);
Boolean ZIsKeycode4(UInt32 keycode);
Boolean ZIsKeycode5(UInt32 keycode);
Boolean ZIsKeycode6(UInt32 keycode);
Boolean ZIsKeycode7(UInt32 keycode);
Boolean ZIsKeycode8(UInt32 keycode);
Boolean ZIsKeycode9(UInt32 keycode);
ZAction ZFlagsToAction(CGEventFlags flags);
Boolean ZAreFlagsFocus(CGEventFlags flags);
Boolean ZAreFlagsResize(CGEventFlags flags);
Boolean ZAreFlagsMove(CGEventFlags flags);
