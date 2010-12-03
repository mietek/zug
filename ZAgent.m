#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#import "ZCommon.h"
#import "ZCarbon.h"
#import "ZCocoa.h"
#import "ZAgent.h"


void ZHandleHotKey(ZHotKey hotKey) {
	AXUIElementRef win;
	if (!(win = ZCopyFrontApplicationFrontWindow()))
		return;
	CGRect winBounds = ZGetWindowBounds(win);
	NSScreen *screen, *newScreen;
	if (!(screen = [NSScreen screenWithRect: ZFlipRect(winBounds)]))
		halt("Error in ZHandleHotKey(): [NSScreen screenWithRect:] -> nil");
	CGRect screenBounds = ZFlipRect([screen visibleFrame]), newScreenBounds;
	ZAnchor anchor;
	CGSize ratio;
	if (hotKey & Z_NEXT_SCREEN) {
		if (!(newScreen = [NSScreen screenAfterScreen: screen]))
			return;
		newScreenBounds = ZFlipRect([newScreen visibleFrame]);
		anchor = ZGuessAnchor(winBounds, screenBounds);
		ratio = ZGuessRatio(winBounds, screenBounds);
	}
	else {
		newScreen = screen;
		newScreenBounds = screenBounds;
		anchor = hotKey & Z_ANCHOR_MASK;
		ratio = ZGetRatio(hotKey);
	}
	CGSize newSize;
	if (hotKey & Z_MOVE)
		newSize = winBounds.size;
	else if (hotKey & Z_RESIZE)
		newSize = CGSizeMake(roundf(newScreenBounds.size.width * ratio.width), roundf(newScreenBounds.size.height * ratio.height));
	else
		haltf("Error in ZHandleHotKey(): hotKey == %d", hotKey);
	CGRect newBounds = ZAnchorRect(anchor, newSize, newScreenBounds);
	ZSetWindowBounds(win, newBounds);
}

CGSize ZGetRatio(ZHotKey hotKey) {
	if (hotKey & Z_LEFT || hotKey & Z_RIGHT) {
		if (hotKey & Z_TOP || hotKey & Z_BOTTOM)
			return CGSizeMake(0.5, 0.5);
		return CGSizeMake(0.5, 1);
	}
	if (hotKey & Z_TOP || hotKey & Z_BOTTOM)
		return CGSizeMake(1, 0.5);
	if (hotKey & Z_FULL_SCREEN)
		return CGSizeMake(1, 1);
	return CGSizeMake(0.667, 1);
}


@implementation ZAgent

- (void)applicationDidFinishLaunching: (NSNotification *)aNotification {
	NSMenu *menu = [[[NSMenu allocWithZone: [NSMenu menuZone]] init] autorelease];
	[menu addHotKeyItemWithTitle: @"Resize to Full Screen" keyEquivalent: @"f" tag: Z_RESIZE_FULL_SCREEN];
	[menu addItem: [NSMenuItem separatorItem]];
	[menu addHotKeyItemWithTitle: @"Resize to Top Left" keyEquivalent: @"q" tag: Z_RESIZE_TOP_LEFT];
	[menu addHotKeyItemWithTitle: @"Move to Top Left" keyEquivalent: @"Q" tag: Z_MOVE_TOP_LEFT alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Top" keyEquivalent: @"w" tag: Z_RESIZE_TOP];
	[menu addHotKeyItemWithTitle: @"Move to Top" keyEquivalent: @"W" tag: Z_MOVE_TOP alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Top Right" keyEquivalent: @"e" tag: Z_RESIZE_TOP_RIGHT];
	[menu addHotKeyItemWithTitle: @"Move to Top Right" keyEquivalent: @"E" tag: Z_MOVE_TOP_RIGHT alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Left" keyEquivalent: @"a" tag: Z_RESIZE_LEFT];
	[menu addHotKeyItemWithTitle: @"Move to Left" keyEquivalent: @"A" tag: Z_MOVE_LEFT alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Center" keyEquivalent: @"s" tag: Z_RESIZE_CENTER];
	[menu addHotKeyItemWithTitle: @"Move to Center" keyEquivalent: @"S" tag: Z_MOVE_CENTER alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Right" keyEquivalent: @"d" tag: Z_RESIZE_RIGHT];
	[menu addHotKeyItemWithTitle: @"Move to Right" keyEquivalent: @"D" tag: Z_MOVE_RIGHT alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Bottom Left" keyEquivalent: @"z" tag: Z_RESIZE_BOTTOM_LEFT];
	[menu addHotKeyItemWithTitle: @"Move to Bottom Left" keyEquivalent: @"Z" tag: Z_MOVE_BOTTOM_LEFT alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Bottom" keyEquivalent: @"x" tag: Z_RESIZE_BOTTOM];
	[menu addHotKeyItemWithTitle: @"Move to Bottom" keyEquivalent: @"X" tag: Z_MOVE_BOTTOM alternate: YES];
	[menu addHotKeyItemWithTitle: @"Resize to Bottom Right" keyEquivalent: @"c" tag: Z_RESIZE_BOTTOM_RIGHT];
	[menu addHotKeyItemWithTitle: @"Move to Bottom Right" keyEquivalent: @"C" tag: Z_MOVE_BOTTOM_RIGHT alternate: YES];
	[menu addItem: [NSMenuItem separatorItem]];
	[menu addHotKeyItemWithTitle: @"Resize to Next Screen" keyEquivalent: @"\u21E5" tag: Z_RESIZE_NEXT_SCREEN];
	NSMenuItem *moveNextScreenItem = [menu addHotKeyItemWithTitle: @"Move to Next Screen" keyEquivalent: @"\u21E5" tag: Z_MOVE_NEXT_SCREEN alternate: YES];
	[moveNextScreenItem setKeyEquivalentModifierMask: NSCommandKeyMask | NSControlKeyMask | NSShiftKeyMask];
	[menu addItem: [NSMenuItem separatorItem]];
	[menu addItemWithTitle: @"Quit" action: @selector(terminate:) keyEquivalent: @""];
	statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength: NSSquareStatusItemLength] retain];
	[statusItem setTitle: @"Z"];
	[statusItem setMenu: menu];
	[statusItem setHighlightMode: YES];
}

- (void)handleHotKeyItem: (NSMenuItem *)aMenuItem {
	ZHandleHotKey([aMenuItem tag]);
}

@end


int main() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSApplication *app = [NSApplication sharedApplication];
	if (!ZAmIAuthorized()) {
		NSRunCriticalAlertPanel(@"ZAgent cannot run.", @"Please enable access for assistive devices in the Universal Access system preference pane.", @"OK", @"", @"");
		return EXIT_FAILURE;
	}
	ZInstallHotKeyHandler(&ZHandleHotKey);
	ZRegisterHotKey(kVK_ANSI_F, cmdKey | controlKey, Z_RESIZE_FULL_SCREEN);
	ZRegisterHotKey(kVK_ANSI_Q, cmdKey | controlKey, Z_RESIZE_TOP_LEFT);
	ZRegisterHotKey(kVK_ANSI_W, cmdKey | controlKey, Z_RESIZE_TOP);
	ZRegisterHotKey(kVK_ANSI_E, cmdKey | controlKey, Z_RESIZE_TOP_RIGHT);
	ZRegisterHotKey(kVK_ANSI_A, cmdKey | controlKey, Z_RESIZE_LEFT);
	ZRegisterHotKey(kVK_ANSI_S, cmdKey | controlKey, Z_RESIZE_CENTER);
	ZRegisterHotKey(kVK_ANSI_D, cmdKey | controlKey, Z_RESIZE_RIGHT);
	ZRegisterHotKey(kVK_ANSI_Z, cmdKey | controlKey, Z_RESIZE_BOTTOM_LEFT);
	ZRegisterHotKey(kVK_ANSI_X, cmdKey | controlKey, Z_RESIZE_BOTTOM);
	ZRegisterHotKey(kVK_ANSI_C, cmdKey | controlKey, Z_RESIZE_BOTTOM_RIGHT);
	ZRegisterHotKey(kVK_ANSI_Q, cmdKey | controlKey | shiftKey, Z_MOVE_TOP_LEFT);
	ZRegisterHotKey(kVK_ANSI_W, cmdKey | controlKey | shiftKey, Z_MOVE_TOP);
	ZRegisterHotKey(kVK_ANSI_E, cmdKey | controlKey | shiftKey, Z_MOVE_TOP_RIGHT);
	ZRegisterHotKey(kVK_ANSI_A, cmdKey | controlKey | shiftKey, Z_MOVE_LEFT);
	ZRegisterHotKey(kVK_ANSI_S, cmdKey | controlKey | shiftKey, Z_MOVE_CENTER);
	ZRegisterHotKey(kVK_ANSI_D, cmdKey | controlKey | shiftKey, Z_MOVE_RIGHT);
	ZRegisterHotKey(kVK_ANSI_Z, cmdKey | controlKey | shiftKey, Z_MOVE_BOTTOM_LEFT);
	ZRegisterHotKey(kVK_ANSI_X, cmdKey | controlKey | shiftKey, Z_MOVE_BOTTOM);
	ZRegisterHotKey(kVK_ANSI_C, cmdKey | controlKey | shiftKey, Z_MOVE_BOTTOM_RIGHT);
	ZRegisterHotKey(kVK_Tab, cmdKey | controlKey, Z_RESIZE_NEXT_SCREEN);
	ZRegisterHotKey(kVK_Tab, cmdKey | controlKey | shiftKey, Z_MOVE_NEXT_SCREEN);
	[app setDelegate: [[[ZAgent alloc] init] autorelease]];
	[app run];
	[pool release];
	return EXIT_SUCCESS;
}
