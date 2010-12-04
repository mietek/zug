#define debug(str) CFShow(CFSTR(str))
#define halt(str) do { debug(str); exit(EXIT_FAILURE); } while (0)
#define debugf(str, ...) do { CFStringRef ref = CFStringCreateWithFormat(NULL, NULL, CFSTR(str), __VA_ARGS__); CFShow(ref); CFRelease(ref); } while (0)
#define haltf(str, ...) do { debugf(str, __VA_ARGS__); exit(EXIT_FAILURE); } while (0)


enum ZAnchor {
	Z_NO_ANCHOR = 0,
	Z_CENTER,
	Z_LEFT,
	Z_RIGHT,
	Z_TOP,
	Z_BOTTOM,
	Z_TOP_LEFT,
	Z_TOP_RIGHT,
	Z_BOTTOM_LEFT,
	Z_BOTTOM_RIGHT
};
typedef UInt32 ZAnchor;
#define Z_ANCHOR_COUNT 9

enum ZIndex {
	Z_NO_INDEX = 0,
	Z_INDEX_1,
	Z_INDEX_2,
	Z_INDEX_3,
	Z_INDEX_4,
	Z_INDEX_5,
	Z_INDEX_6,
	Z_INDEX_7,
	Z_INDEX_8,
	Z_INDEX_9
};
typedef UInt32 ZIndex;
#define Z_INDEX_COUNT 9

enum ZAction {
	Z_NO_ACTION = 0,
	Z_FOCUS_ACTION,
	Z_RESIZE_ACTION,
	Z_MOVE_ACTION
};
typedef UInt32 ZAction;
#define Z_ACTION_COUNT 3

typedef void (*ZKeyEventHandler)(ZAction action, UInt32 anchorCount[Z_ANCHOR_COUNT], ZIndex displayIndex);

typedef struct {
	ZKeyEventHandler handler;
	ZAction action;
	UInt32 anchorCount[Z_ANCHOR_COUNT];
	ZIndex displayIndex;
} ZKeyEventState;


CGFloat ZGetMainDisplayHeight();
CGRect ZFlipRect(CGRect rect);
CGFloat ZGetRectArea(CGRect rect);
CGSize ZGuessRatio(CGRect rect, CGRect bounds);
ZAnchor ZGuessAnchor(CGRect rect, CGRect bounds);
ZAnchor ZNormalizeAnchor(ZAnchor anchor);
CGRect ZAnchorRect(ZAnchor anchor, CGSize size, CGRect bounds);
Boolean ZIsAnchorLeft(ZAnchor anchor);
Boolean ZIsAnchorRight(ZAnchor anchor);
Boolean ZIsAnchorTop(ZAnchor anchor);
Boolean ZIsAnchorBottom(ZAnchor anchor);

bool ZAmIAuthorized();
AXUIElementRef ZCreateFrontApplication();
AXUIElementRef ZCopyFrontWindow(AXUIElementRef frontApp);
AXUIElementRef ZCopyFrontApplicationFrontWindow();
CGPoint ZGetWindowOrigin(AXUIElementRef win);
CGSize ZGetWindowSize(AXUIElementRef win);
CGRect ZGetWindowBounds(AXUIElementRef win);
void ZSetWindowOrigin(AXUIElementRef win, CGPoint origin);
void ZSetWindowSize(AXUIElementRef win, CGSize size);
void ZSetWindowBounds(AXUIElementRef win, CGRect bounds);

void ZInstallKeyEventHandler(ZKeyEventHandler handler);
CGEventRef ZHandleInternalKeyEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, ZKeyEventState *state);
Boolean ZBeginAction(ZAction action, ZAnchor anchor, ZIndex displayIndex, ZKeyEventState *state);
Boolean ZContinueAction(ZAnchor anchor, ZIndex displayIndex, ZKeyEventState *state);
Boolean ZFinishAction(ZAction action, ZKeyEventState *state);

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
