@interface NSScreen (ZCocoa)

+ (NSScreen *)screenWithDisplayID: (CGDirectDisplayID)aDisplayID;
+ (NSScreen *)screenAfterScreen: (NSScreen *)aScreen;
+ (NSScreen *)screenBeforeScreen: (NSScreen *)aScreen;
+ (NSScreen *)screenWithRect: (CGRect)aRect;
- (CGDirectDisplayID)displayID;
- (BOOL)isScreen: (NSScreen *)aScreen;

@end


@interface NSMenu (ZCocoa)

- (NSMenuItem *)addHotKeyItemWithTitle: (NSString *)aTitle keyEquivalent: (NSString *)aKeyEquiv tag: (NSInteger)aTag;
- (NSMenuItem *)addHotKeyItemWithTitle: (NSString *)aTitle keyEquivalent: (NSString *)aKeyEquiv tag: (NSInteger)aTag alternate: (BOOL)anAlt;

@end
