#import <Cocoa/Cocoa.h>

#import "ZCommon.h"
#import "ZCocoa.h"

#define Z_MAX_DISPLAYS 256


@implementation NSScreen (ZCocoa)

+ (NSScreen *)screenWithDisplayID: (CGDirectDisplayID)aDisplayID {
	NSArray *screens = [NSScreen screens];
	NSUInteger screenCount = [screens count], i;
	for (i = 0; i < screenCount; i++) {
		if ([[screens objectAtIndex: i] displayID] == aDisplayID)
			return [screens objectAtIndex: i];
	}
	return nil;
}

+ (NSScreen *)screenAfterScreen: (NSScreen *)aScreen {
	NSArray *screens = [NSScreen screens];
	NSUInteger screenCount = [screens count], screenIndex, i;
	if (screenCount == 1)
		return nil;
	for (i = 0; i < screenCount; i++) {
		if ([[screens objectAtIndex: i] isScreen: aScreen]) {
			screenIndex = (i + 1) % screenCount;
			return [screens objectAtIndex: screenIndex];
		}
	}
	return nil;
}

+ (NSScreen *)screenBeforeScreen: (NSScreen *)aScreen {
	NSArray *screens = [NSScreen screens];
	NSUInteger screenCount = [screens count], screenIndex, i;
	if (screenCount == 1)
		return nil;
	for (i = 0; i < screenCount; i++) {
		if ([[screens objectAtIndex: i] isScreen: aScreen]) {
			if ((screenIndex = i - 1) < 0)
				screenIndex += screenCount;
			return [screens objectAtIndex: screenIndex];
		}
	}
	return nil;
}

+ (NSScreen *)screenWithRect: (CGRect)aRect {
	CGError err;
	CGDirectDisplayID displayIDs[Z_MAX_DISPLAYS];
	uint32_t displayIDCount;
	if ((err = CGGetDisplaysWithRect(ZFlipRect(aRect), Z_MAX_DISPLAYS, displayIDs, &displayIDCount)))
		haltf("Error in [NSScreen screenWithRect:]: CGGetDisplaysWithRect() -> %d", err);
	uint32_t screenCount = 0, i;
	NSScreen *screens[displayIDCount];
	for (i = 0; i < displayIDCount; i++) {
		if (CGDisplayIsActive(displayIDs[i])) {
			if (!(screens[screenCount] = [NSScreen screenWithDisplayID: displayIDs[i]]))
				halt("Error in [NSScreen screenWithRect:]: [NSScreen screenWithDisplay:] -> nil");
			screenCount++;
		}
		else
			debug("Warning in [NSScreen screenWithRect:]: CGDisplayIsActive() -> false");
	}
	NSScreen *bestScreen = nil;
	CGFloat bestArea = 0, area;
	for (i = 0; i < screenCount; i++) {
		area = ZGetRectArea(NSIntersectionRect(aRect, [screens[i] frame]));
		if (area > bestArea) {
			bestArea = area;
			bestScreen = screens[i];
		}
	}
	return bestScreen;
}

- (CGDirectDisplayID)displayID {
	return (CGDirectDisplayID)[[[self deviceDescription] objectForKey: @"NSScreenNumber"] intValue];
}

- (BOOL)isScreen: (NSScreen *)aScreen {
	return [self displayID] == [aScreen displayID];
}

@end
