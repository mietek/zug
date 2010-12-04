#define debug(str) CFShow(CFSTR(str))
#define halt(str) do { debug(str); exit(EXIT_FAILURE); } while (0)
#define debugf(str, ...) do { CFStringRef ref = CFStringCreateWithFormat(NULL, NULL, CFSTR(str), __VA_ARGS__); CFShow(ref); CFRelease(ref); } while (0)
#define haltf(str, ...) do { debugf(str, __VA_ARGS__); exit(EXIT_FAILURE); } while (0)


enum ZAnchor {
	Z_NO_ANCHOR,
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

enum ZAction {
	Z_NO_ACTION,
	Z_FOCUS_ACTION,
	Z_RESIZE_ACTION,
	Z_MOVE_ACTION
};
typedef UInt32 ZAction;

typedef struct {
	ZAction action;
	UInt32 anchorCount[11];
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

CGEventRef ZHandleInternalKeyEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, ZKeyEventState *state);
void ZInstallKeyEventHandler();

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
ZAction ZFlagsToAction(CGEventFlags flags);
Boolean ZAreFlagsFocus(CGEventFlags flags);
Boolean ZAreFlagsResize(CGEventFlags flags);
Boolean ZAreFlagsMove(CGEventFlags flags);
