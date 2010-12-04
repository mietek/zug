#include <Carbon/Carbon.h>

#include "ZCommon.h"


CGFloat ZGetMainDisplayHeight() {
	return CGDisplayBounds(CGMainDisplayID()).size.height;
}

CGRect ZFlipRect(CGRect rect) {
	return CGRectMake(rect.origin.x, ZGetMainDisplayHeight() - rect.origin.y - rect.size.height, rect.size.width, rect.size.height);
}

CGFloat ZGetRectArea(CGRect rect) {
	return rect.size.width * rect.size.height;
}

// CGSize ZGuessRatio(CGRect rect, CGRect bounds) {
// 	CGSize ratio;
// 	ratio.width = rect.size.width / bounds.size.width;
// 	ratio.height = rect.size.height / bounds.size.height;
// 	// debugf("ZGuessRatio: %g %g", ratio.width, ratio.height);
// 	return ratio;
// }
//
// ZAnchor ZGuessAnchor(CGRect rect, CGRect bounds) {
// 	ZAnchor anchor = Z_CENTER;
// 	if (rect.origin.x == bounds.origin.x)
// 		anchor |= Z_LEFT;
// 	if (rect.origin.y == bounds.origin.y)
// 		anchor |= Z_TOP;
// 	if (rect.origin.x + rect.size.width == bounds.origin.x + bounds.size.width)
// 		anchor |= Z_RIGHT;
// 	if (rect.origin.y + rect.size.height == bounds.origin.y + bounds.size.height)
// 		anchor |= Z_BOTTOM;
// 	anchor = ZNormalizeAnchor(anchor);
// 	// debugf("ZGuessAnchor: %d", anchor);
// 	return anchor;
// }
//
// ZAnchor ZNormalizeAnchor(ZAnchor anchor) {
// 	ZAnchor newAnchor = anchor;
// 	if (anchor & Z_LEFT && anchor & Z_RIGHT)
// 		newAnchor ^= Z_LEFT | Z_RIGHT;
// 	if (anchor & Z_TOP && anchor & Z_BOTTOM)
// 		newAnchor ^= Z_TOP | Z_BOTTOM;
// 	return newAnchor;
// }

CGRect ZAnchorRect(ZAnchor anchor, CGSize size, CGRect bounds) {
	if (anchor == Z_NO_ANCHOR)
		haltf("Error in ZAnchorRect(): anchor == %d", anchor);
	CGRect rect;
	rect.size = size;
	if (ZIsAnchorLeft(anchor))
		rect.origin.x = bounds.origin.x;
	else if (ZIsAnchorRight(anchor))
		rect.origin.x = bounds.origin.x + bounds.size.width - rect.size.width;
	else
		rect.origin.x = bounds.origin.x + (bounds.size.width - rect.size.width) / 2;
	if (ZIsAnchorTop(anchor))
		rect.origin.y = bounds.origin.y;
	else if (ZIsAnchorBottom(anchor))
		rect.origin.y = bounds.origin.y + bounds.size.height - rect.size.height;
	else
		rect.origin.y = bounds.origin.y + (bounds.size.height - rect.size.height) / 2;
	return rect;
}

Boolean ZIsAnchorLeft(ZAnchor anchor) {
	return anchor == Z_LEFT || anchor == Z_TOP_LEFT || anchor == Z_BOTTOM_LEFT;
}

Boolean ZIsAnchorRight(ZAnchor anchor) {
	return anchor == Z_RIGHT || anchor == Z_TOP_RIGHT || anchor == Z_BOTTOM_RIGHT;
}

Boolean ZIsAnchorTop(ZAnchor anchor) {
	return anchor == Z_TOP || anchor == Z_TOP_LEFT || anchor == Z_TOP_RIGHT;
}

Boolean ZIsAnchorBottom(ZAnchor anchor) {
	return anchor == Z_BOTTOM || anchor == Z_BOTTOM_LEFT || anchor == Z_BOTTOM_RIGHT;
}


bool ZAmIAuthorized() {
	if (AXAPIEnabled())
		return true;
	if (AXIsProcessTrusted())
		return true;
	// TODO: Become a root process using authorization services, call AXMakeProcessTrusted(), restart
	return false;
}

AXUIElementRef ZCreateFrontApplication() {
	OSErr err1;
	ProcessSerialNumber psn;
	if ((err1 = GetFrontProcess(&psn)))
		haltf("Error in ZCreateFrontApplication(): GetFrontProcess() -> %d", err1);
	OSStatus err2;
	pid_t pid;
	if ((err2 = GetProcessPID(&psn, &pid)))
		haltf("Error in ZCreateFrontApplication(): GetProcessPID() -> %d", err2);
	AXUIElementRef frontApp;
	if (!(frontApp = AXUIElementCreateApplication(pid)))
		halt("Error in ZCreateFrontApplication(): AXUIElementCreateApplication() -> NULL");
	return frontApp;
}

AXUIElementRef ZCopyFrontWindow(AXUIElementRef frontApp) {
	AXError err;
	AXUIElementRef frontWin;
	if ((err = AXUIElementCopyAttributeValue(frontApp, kAXFocusedWindowAttribute, (CFTypeRef *)&frontWin)) && err == kAXErrorNoValue)
		return NULL;
	if (err)
		haltf("Error in ZCopyFrontWindow(): AXUIElementCopyAttributeValue() -> %d", err);
	return frontWin;
}

AXUIElementRef ZCopyFrontApplicationFrontWindow() {
	AXUIElementRef frontApp = ZCreateFrontApplication();
	AXUIElementRef frontWin = ZCopyFrontWindow(frontApp);
	CFRelease(frontApp);
	return frontWin;
}

CGPoint ZGetWindowOrigin(AXUIElementRef win) {
	AXError err;
	AXValueRef originVal;
	CGPoint origin;
	if ((err = AXUIElementCopyAttributeValue(win, kAXPositionAttribute, (CFTypeRef *)&originVal)))
		haltf("Error in ZGetWindowOrigin(): AXUIElementCopyAttributeValue() -> %d", err);
	if (!AXValueGetValue(originVal, kAXValueCGPointType, &origin))
		halt("Error in ZGetWindowOrigin(): AXValueGetValue() -> false");
	CFRelease(originVal);
	return origin;
}

CGSize ZGetWindowSize(AXUIElementRef win) {
	AXError err;
	AXValueRef sizeVal;
	CGSize size;
	if ((err = AXUIElementCopyAttributeValue(win, kAXSizeAttribute, (CFTypeRef *)&sizeVal)))
		haltf("Error in ZGetWindowSize(): AXUIElementCopyAttributeValue() -> %d", err);
	if (!AXValueGetValue(sizeVal, kAXValueCGSizeType, &size))
		halt("Error in ZGetWindowSize(): AXValueGetValue() -> false");
	CFRelease(sizeVal);
	return size;
}

CGRect ZGetWindowBounds(AXUIElementRef win) {
	CGRect bounds;
	bounds.origin = ZGetWindowOrigin(win);
	bounds.size = ZGetWindowSize(win);
	return bounds;
}

void ZSetWindowOrigin(AXUIElementRef win, CGPoint origin) {
	AXError err;
	AXValueRef originVal;
	if (!(originVal = AXValueCreate(kAXValueCGPointType, &origin)))
		halt("Error in ZSetWindowOrigin(): AXValueCreate() -> NULL");
	if ((err = AXUIElementSetAttributeValue(win, kAXPositionAttribute, originVal)))
		haltf("Error in ZSetWindowOrigin(): AXUIElementSetAttributeValue() -> %d", err);
	CFRelease(originVal);
}

void ZSetWindowSize(AXUIElementRef win, CGSize size) {
	AXError err;
	AXValueRef sizeVal;
	if (!(sizeVal = AXValueCreate(kAXValueCGSizeType, &size)))
		halt("Error in ZSetWindowSize(): AXValueCreate() -> NULL");
	if ((err = AXUIElementSetAttributeValue(win, kAXSizeAttribute, sizeVal)))
		haltf("Error in ZSetWindowSize(): AXUIElementSetAttributeValue() -> %d", err);
	CFRelease(sizeVal);
}

void ZSetWindowBounds(AXUIElementRef win, CGRect bounds) {
	ZSetWindowOrigin(win, bounds.origin);
	ZSetWindowSize(win, bounds.size);
}


CGEventRef ZHandleInternalKeyEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, ZKeyEventState *state) {
	if (state->action == Z_NO_ACTION) {
		if (type == kCGEventKeyDown) {
			ZAction action = ZFlagsToAction(CGEventGetFlags(event));
			if (action != Z_NO_ACTION) {
				UInt32 keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				ZAnchor anchor = ZKeycodeToAnchor(keycode);
				ZIndex displayIndex = ZKeycodeToIndex(keycode);
				if (anchor != Z_NO_ANCHOR || displayIndex != Z_NO_INDEX) {
					debugf("starting action: %d", action);
					state->action = action;
					memset(state->anchorCount, 0, sizeof(state->anchorCount));
					state->displayIndex = Z_NO_INDEX;
					if (anchor != Z_NO_ANCHOR)
						state->anchorCount[anchor]++;
					if (displayIndex != Z_NO_INDEX)
						state->displayIndex = displayIndex;
					return NULL;
				}
			}
		}
	}
	else {
		if (type == kCGEventKeyDown) {
			UInt32 keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
			ZAnchor anchor = ZKeycodeToAnchor(keycode);
			ZIndex displayIndex = ZKeycodeToIndex(keycode);
			if (anchor != Z_NO_ANCHOR || displayIndex != Z_NO_INDEX) {
				if (anchor != Z_NO_ANCHOR)
					state->anchorCount[anchor]++;
				if (displayIndex != Z_NO_INDEX)
					state->displayIndex = displayIndex;
				return NULL;
			}
		}
		else if (type == kCGEventFlagsChanged) {
			ZAction action = ZFlagsToAction(CGEventGetFlags(event));
			if (action == Z_NO_ACTION) {
				debugf("ending action: %d, anchorCount: %d, %d %d %d %d, %d %d %d %d, displayIndex: %d", state->action, state->anchorCount[Z_CENTER], state->anchorCount[Z_LEFT], state->anchorCount[Z_RIGHT], state->anchorCount[Z_TOP], state->anchorCount[Z_BOTTOM], state->anchorCount[Z_TOP_LEFT], state->anchorCount[Z_TOP_RIGHT], state->anchorCount[Z_BOTTOM_LEFT], state->anchorCount[Z_BOTTOM_RIGHT], state->displayIndex);
				state->action = Z_NO_ACTION;
				return NULL;
			}
		}
		else
			debugf("Warning in ZHandleInternalKeyEvent(): type == %d", type);
	}
	return event;
}

void ZInstallKeyEventHandler() {
	static ZKeyEventState state;
	CFMachPortRef tap;
	if (!(tap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged), (CGEventTapCallBack)&ZHandleInternalKeyEvent, &state)))
		halt("Error in ZInstallKeyEventHandler(): CGEventTapCreate() -> NULL");
	CFRunLoopSourceRef source;
	if (!(source = CFMachPortCreateRunLoopSource(NULL, tap, 0)))
		halt("Error in ZInstallKeyEventHandler(): CFMachPortCreateRunLoopSource() -> NULL");
	CFRelease(tap);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopCommonModes);
	CFRelease(source);
}


ZAnchor ZKeycodeToAnchor(UInt32 keycode) {
	if (ZIsKeycodeCenter(keycode))
		return Z_CENTER;
	if (ZIsKeycodeLeft(keycode))
		return Z_LEFT;
	if (ZIsKeycodeRight(keycode))
		return Z_RIGHT;
	if (ZIsKeycodeTop(keycode))
		return Z_TOP;
	if (ZIsKeycodeBottom(keycode))
		return Z_BOTTOM;
	if (ZIsKeycodeTopLeft(keycode))
		return Z_TOP_LEFT;
	if (ZIsKeycodeTopRight(keycode))
		return Z_TOP_RIGHT;
	if (ZIsKeycodeBottomLeft(keycode))
		return Z_BOTTOM_LEFT;
	if (ZIsKeycodeBottomRight(keycode))
		return Z_BOTTOM_RIGHT;
	return Z_NO_ANCHOR;
}

Boolean ZIsKeycodeCenter(UInt32 keycode) {
	return keycode == kVK_ANSI_S;
}

Boolean ZIsKeycodeLeft(UInt32 keycode) {
	return keycode == kVK_ANSI_A;
}

Boolean ZIsKeycodeRight(UInt32 keycode) {
	return keycode == kVK_ANSI_D;
}

Boolean ZIsKeycodeTop(UInt32 keycode) {
	return keycode == kVK_ANSI_W;
}

Boolean ZIsKeycodeBottom(UInt32 keycode) {
	return keycode == kVK_ANSI_X;
}

Boolean ZIsKeycodeTopLeft(UInt32 keycode) {
	return keycode == kVK_ANSI_Q;
}

Boolean ZIsKeycodeTopRight(UInt32 keycode) {
	return keycode == kVK_ANSI_E;
}

Boolean ZIsKeycodeBottomLeft(UInt32 keycode) {
	return keycode == kVK_ANSI_Z;
}

Boolean ZIsKeycodeBottomRight(UInt32 keycode) {
	return keycode == kVK_ANSI_C;
}

ZIndex ZKeycodeToIndex(UInt32 keycode) {
	if (ZIsKeycode1(keycode))
		return Z_INDEX_1;
	if (ZIsKeycode2(keycode))
		return Z_INDEX_2;
	if (ZIsKeycode3(keycode))
		return Z_INDEX_3;
	if (ZIsKeycode4(keycode))
		return Z_INDEX_4;
	if (ZIsKeycode5(keycode))
		return Z_INDEX_5;
	if (ZIsKeycode6(keycode))
		return Z_INDEX_6;
	if (ZIsKeycode7(keycode))
		return Z_INDEX_7;
	if (ZIsKeycode8(keycode))
		return Z_INDEX_8;
	if (ZIsKeycode9(keycode))
		return Z_INDEX_9;
	return Z_NO_INDEX;
}

Boolean ZIsKeycode1(UInt32 keycode) {
	return keycode == kVK_ANSI_1;
}

Boolean ZIsKeycode2(UInt32 keycode) {
	return keycode == kVK_ANSI_2;
}

Boolean ZIsKeycode3(UInt32 keycode) {
	return keycode == kVK_ANSI_3;
}

Boolean ZIsKeycode4(UInt32 keycode) {
	return keycode == kVK_ANSI_4;
}

Boolean ZIsKeycode5(UInt32 keycode) {
	return keycode == kVK_ANSI_5;
}

Boolean ZIsKeycode6(UInt32 keycode) {
	return keycode == kVK_ANSI_6;
}

Boolean ZIsKeycode7(UInt32 keycode) {
	return keycode == kVK_ANSI_7;
}

Boolean ZIsKeycode8(UInt32 keycode) {
	return keycode == kVK_ANSI_8;
}

Boolean ZIsKeycode9(UInt32 keycode) {
	return keycode == kVK_ANSI_9;
}

ZAction ZFlagsToAction(CGEventFlags flags) {
	if (ZAreFlagsFocus(flags))
		return Z_FOCUS_ACTION;
	if (ZAreFlagsResize(flags))
		return Z_RESIZE_ACTION;
	if (ZAreFlagsMove(flags))
		return Z_MOVE_ACTION;
	return Z_NO_ACTION;
}

Boolean ZAreFlagsFocus(CGEventFlags flags) {
	return flags & kCGEventFlagMaskSecondaryFn && !(flags & kCGEventFlagMaskControl) && !(flags & kCGEventFlagMaskShift);
}

Boolean ZAreFlagsResize(CGEventFlags flags) {
	return flags & kCGEventFlagMaskSecondaryFn && flags & kCGEventFlagMaskControl && !(flags & kCGEventFlagMaskShift);
}

Boolean ZAreFlagsMove(CGEventFlags flags) {
	return flags & kCGEventFlagMaskSecondaryFn && !(flags & kCGEventFlagMaskControl) && flags & kCGEventFlagMaskShift;
}
