#include <Carbon/Carbon.h>

#include "ZCommon.h"

#define Z_SNAP_DISTANCE 8


CGFloat ZGetMainDisplayHeight() {
	return CGDisplayBounds(CGMainDisplayID()).size.height;
}

CGRect ZFlipRect(CGRect rect) {
	return CGRectMake(rect.origin.x, ZGetMainDisplayHeight() - rect.origin.y - rect.size.height, rect.size.width, rect.size.height);
}

CGFloat ZGetRectArea(CGRect rect) {
	return rect.size.width * rect.size.height;
}

CGSize ZGuessRatio(CGRect rect, CGRect bounds) {
	CGSize ratio;
	ratio.width = rect.size.width / bounds.size.width;
	ratio.height = rect.size.height / bounds.size.height;
	return ratio;
}

ZAnchor ZGuessAnchor(CGRect rect, CGRect bounds) {
	Boolean left = false;
	if (abs(rect.origin.x - bounds.origin.x) <= Z_SNAP_DISTANCE)
		left = true;
	Boolean right = false;
	if (abs((rect.origin.x + rect.size.width) - (bounds.origin.x + bounds.size.width)) <= Z_SNAP_DISTANCE)
		right = true;
	Boolean top = false;
	if (abs(rect.origin.y - bounds.origin.y) <= Z_SNAP_DISTANCE)
		top = true;
	Boolean bottom = false;
	if (abs((rect.origin.y + rect.size.height) - (bounds.origin.y + bounds.size.height)) <= Z_SNAP_DISTANCE)
		bottom = true;
	if (left && right)
		left = right = false;
	if (top && bottom)
		top = bottom = false;
	if (left && top)
		return Z_TOP_LEFT;
	if (right && top)
		return Z_TOP_RIGHT;
	if (left && bottom)
		return Z_BOTTOM_LEFT;
	if (right && bottom)
		return Z_BOTTOM_RIGHT;
	if (left)
		return Z_LEFT;
	if (right)
		return Z_RIGHT;
	if (top)
		return Z_TOP;
	if (bottom)
		return Z_BOTTOM;
	return Z_CENTER;
}

CGRect ZAnchorRect(ZAnchor anchor, CGSize size, CGRect bounds) {
	if (anchor == Z_NO_ANCHOR)
		haltf("Error in ZAnchorRect(): anchor == %d", anchor);
	CGRect rect;
	rect.size = size;
	if (anchor == Z_LEFT || anchor == Z_TOP_LEFT || anchor == Z_BOTTOM_LEFT)
		rect.origin.x = bounds.origin.x;
	else if (anchor == Z_RIGHT || anchor == Z_TOP_RIGHT || anchor == Z_BOTTOM_RIGHT)
		rect.origin.x = bounds.origin.x + bounds.size.width - size.width;
	else
		rect.origin.x = roundf(bounds.origin.x + (bounds.size.width - size.width) / 2);
	if (anchor == Z_TOP || anchor == Z_TOP_LEFT || anchor == Z_TOP_RIGHT)
		rect.origin.y = bounds.origin.y;
	else if (anchor == Z_BOTTOM || anchor == Z_BOTTOM_LEFT || anchor == Z_BOTTOM_RIGHT)
		rect.origin.y = bounds.origin.y + bounds.size.height - size.height;
	else
		rect.origin.y = roundf(bounds.origin.y + (bounds.size.height - size.height) / 2);
	return rect;
}

CGRect ZAnchorPartRect(ZAnchor anchor, CGSize size, UInt32 horizPart, UInt32 vertPart, CGRect bounds) {
	if (anchor == Z_NO_ANCHOR)
		haltf("Error in ZAnchorRect(): anchor == %d", anchor);
	CGRect rect;
	rect.size = size;
	if (anchor == Z_LEFT || anchor == Z_TOP_LEFT || anchor == Z_BOTTOM_LEFT)
		rect.origin.x = bounds.origin.x + horizPart * size.width;
	else if (anchor == Z_RIGHT || anchor == Z_TOP_RIGHT || anchor == Z_BOTTOM_RIGHT)
		rect.origin.x = bounds.origin.x + bounds.size.width - (horizPart + 1) * size.width;
	else
		rect.origin.x = roundf(bounds.origin.x + (bounds.size.width - (horizPart + 1) * size.width) / 2);
	if (anchor == Z_TOP || anchor == Z_TOP_LEFT || anchor == Z_TOP_RIGHT)
		rect.origin.y = bounds.origin.y + vertPart * size.height;
	else if (anchor == Z_BOTTOM || anchor == Z_BOTTOM_LEFT || anchor == Z_BOTTOM_RIGHT)
		rect.origin.y = bounds.origin.y + bounds.size.height - (vertPart + 1) * size.height;
	else
		rect.origin.y = roundf(bounds.origin.y + (bounds.size.height - (vertPart + 1) * size.height) / 2);
	return rect;
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
	bounds.size = ZGetWindowSize(win);
	bounds.origin = ZGetWindowOrigin(win);
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


void ZInstallKeyEventHandler(ZKeyEventHandler handler, void *handlerData) {
	static ZHandlerState state;
	state.handler = handler;
	state.handlerData = handlerData;
	CFMachPortRef tap;
	if (!(tap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged), &ZHandleEvent, &state)))
		halt("Error in ZInstallKeyEventHandler(): CGEventTapCreate() -> NULL");
	state.tap = tap;
	CFRunLoopSourceRef source;
	if (!(source = CFMachPortCreateRunLoopSource(NULL, tap, 0)))
		halt("Error in ZInstallKeyEventHandler(): CFMachPortCreateRunLoopSource() -> NULL");
	CFRelease(tap);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), source, kCFRunLoopCommonModes);
	CFRelease(source);
}

CGEventRef ZHandleEvent(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userData) {
	(void)proxy;
	ZHandlerState *state = (ZHandlerState *)userData;
	if (type == kCGEventTapDisabledByTimeout) {
		debug("Warning in ZHandleEvent(): type == kCGEventTapDisabledByTimeout");
		CGEventTapEnable(state->tap, true);
		return NULL;
	}
	if (type == kCGEventTapDisabledByUserInput) {
		debug("Warning in ZHandleEvent(): type == kCGEventTapDisabledByUserInput");
		CGEventTapEnable(state->tap, true);
		return NULL;
	}
	if (state->handler(event, state->handlerData))
		return NULL;
	return event;
}
