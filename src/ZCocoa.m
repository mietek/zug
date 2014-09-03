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
	NSUInteger screenCount = [screens count], i;
	if (screenCount == 1)
		return nil;
	for (i = 0; i < screenCount; i++) {
		if ([[screens objectAtIndex: i] isScreen: aScreen]) {
			return [screens objectAtIndex: (i + 1) % screenCount];
		}
	}
	return nil;
}

+ (NSScreen *)screenBeforeScreen: (NSScreen *)aScreen {
	NSArray *screens = [NSScreen screens];
	NSUInteger screenCount = [screens count], i;
	if (screenCount == 1)
		return nil;
	for (i = 0; i < screenCount; i++) {
		if ([[screens objectAtIndex: i] isScreen: aScreen]) {
			return [screens objectAtIndex: i ? i - 1 : screenCount - 1];
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
	NSMutableArray *screens = [NSMutableArray arrayWithCapacity:displayIDCount];
	uint32_t i;
	for (i = 0; i < displayIDCount; i++) {
		if (CGDisplayIsActive(displayIDs[i])) {
			NSScreen *screen;
			if (!(screen = [NSScreen screenWithDisplayID: displayIDs[i]]))
				halt("Error in [NSScreen screenWithRect:]: [NSScreen screenWithDisplay:] -> nil");
			[screens addObject:screen];
		}
		else
			debug("Warning in [NSScreen screenWithRect:]: CGDisplayIsActive() -> false");
	}
	NSScreen *bestScreen = nil;
	CGFloat bestArea = 0, area;
	for (i = 0; i < [screens count]; i++) {
		area = ZGetRectArea(NSIntersectionRect(aRect, [[screens objectAtIndex:i] frame]));
		if (area > bestArea) {
			bestArea = area;
			bestScreen = [screens objectAtIndex:i];
		}
	}
	return bestScreen;
}

+ (NSScreen *)screenWithIndex: (NSUInteger)anIndex {
	NSArray *screens = [NSScreen screens];
	if (anIndex >= [screens count])
		return nil;
	return [screens objectAtIndex: anIndex];
}

- (CGDirectDisplayID)displayID {
	return (CGDirectDisplayID)[[[self deviceDescription] objectForKey: @"NSScreenNumber"] intValue];
}

- (BOOL)isScreen: (NSScreen *)aScreen {
	return [self displayID] == [aScreen displayID];
}

@end
