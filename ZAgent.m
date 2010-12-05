#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#import "ZCommon.h"
#import "ZCocoa.h"
#import "ZAgent.h"


@implementation ZAgent

- (void)applicationDidFinishLaunching: (NSNotification *)aNotification {
	NSMenu *menu = [[[NSMenu allocWithZone: [NSMenu menuZone]] init] autorelease];
	[menu addItemWithTitle: @"Quit" action: @selector(terminate:) keyEquivalent: @""];
	statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength: NSSquareStatusItemLength] retain];
	[statusItem setTitle: @"Z"];
	[statusItem setMenu: menu];
	[statusItem setHighlightMode: YES];
}

@end


Boolean ZHandleKeyEvent(CGEventRef event, void *handlerData) {
	ZKeyEventState *state = (ZKeyEventState *)handlerData;
	CGEventType type = CGEventGetType(event);
	ZAction action = ZFlagsToAction(CGEventGetFlags(event));
	if (type == kCGEventKeyDown) {
		UInt32 keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
		ZAnchor anchor = ZKeycodeToAnchor(keycode);
		ZIndex screenIndex = ZKeycodeToIndex(keycode);
		if (ZBeginAction(action, anchor, screenIndex, state))
			return true;
		if (ZContinueAction(anchor, screenIndex, state))
			return true;
	}
	else if (type == kCGEventFlagsChanged) {
		if (ZFinishAction(action, state))
			return true;
	}
	return false;
}

Boolean ZBeginAction(ZAction action, ZAnchor anchor, ZIndex screenIndex, ZKeyEventState *state) {
	if (state->action == Z_NO_ACTION && action != Z_NO_ACTION && (anchor != Z_NO_ANCHOR || screenIndex != Z_NO_INDEX)) {
		state->action = action;
		if (ZContinueAction(anchor, screenIndex, state))
			return true;
		state->action = Z_NO_ACTION;
	}
	return false;
}

Boolean ZContinueAction(ZAnchor anchor, ZIndex screenIndex, ZKeyEventState *state) {
	if (state->action != Z_NO_ACTION && (anchor != Z_NO_ANCHOR || screenIndex != Z_NO_INDEX)) {
		if (anchor != Z_NO_ANCHOR)
			state->anchorCount[anchor]++;
		if (screenIndex != Z_NO_INDEX)
			state->screenIndex = screenIndex;
		return true;
	}
	return false;
}

Boolean ZFinishAction(ZAction action, ZKeyEventState *state) {
	if (state->action != Z_NO_ACTION && action == Z_NO_ACTION) {
		debug("woop");
		// state->handler(state->action, state->anchorCount, state->screenIndex);
		state->action = Z_NO_ACTION;
		memset(state->anchorCount, 0, sizeof(state->anchorCount));
		state->screenIndex = Z_NO_INDEX;
		return true;
	}
	return false;
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


int main() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSApplication *app = [NSApplication sharedApplication];
	if (!ZAmIAuthorized()) {
		NSRunCriticalAlertPanel(@"ZAgent cannot run.", @"Please enable access for assistive devices in the Universal Access system preference pane.", @"OK", @"", @"");
		return EXIT_FAILURE;
	}
	ZKeyEventState state;
	memset(&state, 0, sizeof(state));
	ZInstallKeyEventHandler(&ZHandleKeyEvent, &state);
	[app setDelegate: [[[ZAgent alloc] init] autorelease]];
	[app run];
	[pool release];
	return EXIT_SUCCESS;
}
