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
