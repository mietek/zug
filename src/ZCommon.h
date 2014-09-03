#define debug(str) CFShow(CFSTR(str))
#define halt(str) do { debug(str); exit(EXIT_FAILURE); } while (0)
#define debugf(str, ...) do { CFStringRef ref = CFStringCreateWithFormat(NULL, NULL, CFSTR(str), __VA_ARGS__); CFShow(ref); CFRelease(ref); } while (0)
#define haltf(str, ...) do { debugf(str, __VA_ARGS__); exit(EXIT_FAILURE); } while (0)


typedef enum {
	Z_NO_ANCHOR = 0x7FFFFFFF,
	Z_CENTER = 0,
	Z_LEFT,
	Z_RIGHT,
	Z_TOP,
	Z_BOTTOM,
	Z_TOP_LEFT,
	Z_TOP_RIGHT,
	Z_BOTTOM_LEFT,
	Z_BOTTOM_RIGHT
} ZAnchor;

#define Z_ANCHOR_COUNT 9

typedef enum {
	Z_NO_INDEX = 0x7FFFFFFF,
	Z_INDEX_1 = 0,
	Z_INDEX_2,
	Z_INDEX_3,
	Z_INDEX_4,
	Z_INDEX_5,
	Z_INDEX_6,
	Z_INDEX_7,
	Z_INDEX_8,
	Z_INDEX_9
} ZIndex;

#define Z_INDEX_COUNT 9

typedef enum {
	Z_NO_ACTION = 0x7FFFFFFF,
	Z_FOCUS_ACTION = 0,
	Z_RESIZE_ACTION,
	Z_MOVE_ACTION
} ZAction;

#define Z_ACTION_COUNT 3

typedef Boolean (*ZKeyEventHandler)(CGEventRef event, void *handlerData);

typedef struct {
	ZKeyEventHandler handler;
	void *handlerData;
	CFMachPortRef tap;
} ZHandlerState;


CGFloat ZGetMainDisplayHeight(void);
CGRect ZFlipRect(CGRect rect);
CGFloat ZGetRectArea(CGRect rect);
CGSize ZGuessRatio(CGRect rect, CGRect bounds);
ZAnchor ZGuessAnchor(CGRect rect, CGRect bounds);
CGRect ZAnchorRect(ZAnchor anchor, CGSize size, CGRect bounds);
CGRect ZAnchorPartRect(ZAnchor anchor, CGSize size, UInt32 horizPart, UInt32 vertPart, CGRect bounds);
Boolean ZIsAnchorLeft(ZAnchor anchor);
Boolean ZIsAnchorRight(ZAnchor anchor);
Boolean ZIsAnchorTop(ZAnchor anchor);
Boolean ZIsAnchorBottom(ZAnchor anchor);

bool ZAmIAuthorized(void);
AXUIElementRef ZCreateFrontApplication(void);
AXUIElementRef ZCopyFrontWindow(AXUIElementRef frontApp);
AXUIElementRef ZCopyFrontApplicationFrontWindow(void);
CGPoint ZGetWindowOrigin(AXUIElementRef win);
CGSize ZGetWindowSize(AXUIElementRef win);
CGRect ZGetWindowBounds(AXUIElementRef win);
void ZSetWindowOrigin(AXUIElementRef win, CGPoint origin);
void ZSetWindowSize(AXUIElementRef win, CGSize size);

void ZInstallKeyEventHandler(ZKeyEventHandler handler, void *handlerData);
CGEventRef ZHandleEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userData);
