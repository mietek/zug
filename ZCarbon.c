#include <Carbon/Carbon.h>

#include "ZCommon.h"
#include "ZCarbon.h"

#define Z_HOT_KEY_SIGNATURE 'Zzz!'


OSStatus ZHandleSignedHotKey(EventHandlerCallRef eventHandler, EventRef event, void *userData) {
	ZHotKeyHandler handlerFunc = (ZHotKeyHandler)userData;
	OSType eventClass;
	if ((eventClass = GetEventClass(event)) != kEventClassKeyboard)
		haltf("Error in ZHandleSignedHotKey(): eventClass == %d", eventClass);
	UInt32 eventKind;
	if ((eventKind = GetEventKind(event)) != kEventHotKeyPressed)
		haltf("Error in ZHandleSignedHotKey(): kind == %d", eventKind);
	OSStatus err;
	EventHotKeyID signedHotKeyId;
	if ((err = GetEventParameter(event, kEventParamDirectObject, typeEventHotKeyID, NULL, sizeof(EventHotKeyID), NULL, &signedHotKeyId)))
		haltf("Error in ZHandleSignedHotKey(): GetEventParameter() -> %d", err);
	handlerFunc(signedHotKeyId.id);
	return noErr;
}

void ZInstallHotKeyHandler(ZHotKeyHandler handlerFunc) {
	EventTargetRef eventTarget = GetApplicationEventTarget();
	EventTypeSpec eventType;
	eventType.eventClass = kEventClassKeyboard;
	eventType.eventKind = kEventHotKeyPressed;
	OSStatus err;
	if ((err = InstallEventHandler(eventTarget, &ZHandleSignedHotKey, 1, &eventType, handlerFunc, NULL)))
		haltf("Error in ZInstallHotKeyHandler(): InstallEventHandler() -> %d", err);
}

EventHotKeyRef ZRegisterHotKey(UInt32 hotKeyCode, UInt32 hotKeyMods, UInt32 hotKeyId) {
	EventTargetRef eventTarget = GetApplicationEventTarget();
	EventHotKeyID signedHotKeyId;
	signedHotKeyId.signature = Z_HOT_KEY_SIGNATURE;
	signedHotKeyId.id = hotKeyId;
	OSStatus err;
	EventHotKeyRef hotKeyRef;
	if ((err = RegisterEventHotKey(hotKeyCode, hotKeyMods, signedHotKeyId, eventTarget, 0, &hotKeyRef)))
		haltf("Error in ZRegisterHotKey: RegisterEventHotKey() -> %d", err);
	return hotKeyRef;
}
