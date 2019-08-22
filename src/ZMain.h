typedef struct {
	ZAction action;
	ZIndex screenIndex;
	ZAnchor anchor;
	UInt32 anchorCount[Z_ANCHOR_COUNT];
} ZKeyEventState;


@interface ZMain : NSObject {}

@property (nonatomic, retain) NSStatusItem *statusItem;

@end


void ZDoAction(ZAction action, ZIndex screenIndex, ZAnchor anchor, UInt32 anchorCount[Z_ANCHOR_COUNT]);
Boolean ZHandleKeyEvent(CGEventRef event, void *handlerData);

ZAnchor ZKeycodeToAnchor(int64_t keycode);
Boolean ZIsKeycodeCenter(int64_t keycode);
Boolean ZIsKeycodeLeft(int64_t keycode);
Boolean ZIsKeycodeRight(int64_t keycode);
Boolean ZIsKeycodeTop(int64_t keycode);
Boolean ZIsKeycodeBottom(int64_t keycode);
Boolean ZIsKeycodeTopLeft(int64_t keycode);
Boolean ZIsKeycodeTopRight(int64_t keycode);
Boolean ZIsKeycodeBottomLeft(int64_t keycode);
Boolean ZIsKeycodeBottomRight(int64_t keycode);
ZIndex ZKeycodeToIndex(int64_t keycode);
Boolean ZIsKeycode1(int64_t keycode);
Boolean ZIsKeycode2(int64_t keycode);
Boolean ZIsKeycode3(int64_t keycode);
Boolean ZIsKeycode4(int64_t keycode);
Boolean ZIsKeycode5(int64_t keycode);
Boolean ZIsKeycode6(int64_t keycode);
Boolean ZIsKeycode7(int64_t keycode);
Boolean ZIsKeycode8(int64_t keycode);
Boolean ZIsKeycode9(int64_t keycode);
ZAction ZFlagsToAction(CGEventFlags flags);
Boolean ZAreFlagsFocus(CGEventFlags flags);
Boolean ZAreFlagsResize(CGEventFlags flags);
Boolean ZAreFlagsMove(CGEventFlags flags);
