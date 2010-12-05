#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#import "ZCommon.h"
#import "ZCocoa.h"
#import "ZAgent.h"


int main() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSApplication *app = [NSApplication sharedApplication];
	if (!ZAmIAuthorized()) {
		NSRunCriticalAlertPanel(@"ZAgent cannot run.", @"Please enable access for assistive devices in the Universal Access system preference pane.", @"OK", @"", @"");
		return EXIT_FAILURE;
	}
	ZKeyEventState state;
	state.action = Z_NO_ACTION;
	memset(&state.anchorCount, 0, sizeof(state.anchorCount));
	state.screenIndex = Z_NO_INDEX;
	ZInstallKeyEventHandler(&ZHandleKeyEvent, &state);
	[app setDelegate: [[[ZAgent alloc] init] autorelease]];
	[app run];
	[pool release];
	return EXIT_SUCCESS;
}


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
	ZAction action = ZFlagsToAction(CGEventGetFlags(event));
	CGEventType type = CGEventGetType(event);
	if (type == kCGEventKeyDown) {
		if (state->action == Z_NO_ACTION) {
			if (action != Z_NO_ACTION) {
				UInt32 keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				ZIndex screenIndex = ZKeycodeToIndex(keycode);
				if (screenIndex != Z_NO_INDEX) {
					if ([NSScreen screenWithIndex: screenIndex]) {
						state->action = action;
						state->screenIndex = screenIndex;
					}
					else
						NSBeep();
					return true;
				}
				ZAnchor anchor = ZKeycodeToAnchor(keycode);
				if (anchor != Z_NO_ANCHOR) {
					state->action = action;
					state->anchorCount[anchor]++;
					return true;
				}
			}
		}
		else {
			if (action != Z_NO_ACTION) {
				UInt32 keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
				ZIndex screenIndex = ZKeycodeToIndex(keycode);
				if (screenIndex != Z_NO_INDEX) {
					if ([NSScreen screenWithIndex: screenIndex])
						state->screenIndex = screenIndex;
					else
						NSBeep();
					return true;
				}
				ZAnchor anchor = ZKeycodeToAnchor(keycode);
				if (anchor != Z_NO_ANCHOR) {
					state->anchorCount[anchor]++;
					return true;
				}
			}
		}
	}
	else if (type == kCGEventFlagsChanged) {
		if (state->action != Z_NO_ACTION && action == Z_NO_ACTION) {
			debugf("action: %d, anchorCount: %d, %d %d %d %d, %d %d %d %d, screenIndex: %d", state->action, state->anchorCount[Z_CENTER], state->anchorCount[Z_LEFT], state->anchorCount[Z_RIGHT], state->anchorCount[Z_TOP], state->anchorCount[Z_BOTTOM], state->anchorCount[Z_TOP_LEFT], state->anchorCount[Z_TOP_RIGHT], state->anchorCount[Z_BOTTOM_LEFT], state->anchorCount[Z_BOTTOM_RIGHT], state->screenIndex);
			state->action = Z_NO_ACTION;
			memset(state->anchorCount, 0, sizeof(state->anchorCount));
			state->screenIndex = Z_NO_INDEX;
			return true;
		}
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
