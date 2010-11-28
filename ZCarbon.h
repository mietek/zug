typedef void (*ZHotKeyHandler)(UInt32 hotKeyId);


OSStatus ZHandleSignedHotKey(EventHandlerCallRef eventHandler, EventRef event, void *userData);
void ZInstallHotKeyHandler(ZHotKeyHandler handlerFunc);
EventHotKeyRef ZRegisterHotKey(UInt32 hotKeyCode, UInt32 hotKeyMods, UInt32 hotKeyId);
