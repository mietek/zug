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


void ZHandleKeyEvent(ZAction action, UInt32 anchorCount[9], ZIndex displayIndex) {
	debugf("ending action: %d, anchorCount: %d, %d %d %d %d, %d %d %d %d, displayIndex: %d", action, anchorCount[Z_CENTER], anchorCount[Z_LEFT], anchorCount[Z_RIGHT], anchorCount[Z_TOP], anchorCount[Z_BOTTOM], anchorCount[Z_TOP_LEFT], anchorCount[Z_TOP_RIGHT], anchorCount[Z_BOTTOM_LEFT], anchorCount[Z_BOTTOM_RIGHT], displayIndex);
}

int main() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSApplication *app = [NSApplication sharedApplication];
	if (!ZAmIAuthorized()) {
		NSRunCriticalAlertPanel(@"ZAgent cannot run.", @"Please enable access for assistive devices in the Universal Access system preference pane.", @"OK", @"", @"");
		return EXIT_FAILURE;
	}
	ZInstallKeyEventHandler(&ZHandleKeyEvent);
	[app setDelegate: [[[ZAgent alloc] init] autorelease]];
	[app run];
	[pool release];
	return EXIT_SUCCESS;
}
