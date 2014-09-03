@interface NSScreen (ZCocoa)

+ (NSScreen *)screenWithDisplayID: (CGDirectDisplayID)aDisplayID;
+ (NSScreen *)screenAfterScreen: (NSScreen *)aScreen;
+ (NSScreen *)screenBeforeScreen: (NSScreen *)aScreen;
+ (NSScreen *)screenWithRect: (CGRect)aRect;
+ (NSScreen *)screenWithIndex: (NSUInteger)anIndex;
- (CGDirectDisplayID)displayID;
- (BOOL)isScreen: (NSScreen *)aScreen;

@end
