//////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT
//
// This program is Copyright (c) 2002 by Mike Gogulski <mike@gogulski.com>
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Syadasti's open-source human-readable software LICENSE
//
// This code is freely redistributable, provided that all credits and
// copyright notices are preserved.
//
// You may use this code in your own software without restriction, provided
// that your software is distributed with complete source code.
//
// You agree by making use of this code that whatever problems you experience
// as a result are your fault, and yours alone.
//
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include "automation.h"
#include "logging.h"
#include "util.h"
#include "external/LinkedList/linkedlist.h"

// all screen X, Y coordinates passed to these functions must be relative 
// to FULLSCREEN mode
//
// Thanks to Gayak for the click functions

int keydelay = 20;
//BOOL fAccessingMq = FALSE;
BOOL fQueueing = FALSE;
CRITICAL_SECTION AutomationMutex;

KEYTABLE KeyTable[NUM_KEY_ENTRIES] = {
	"NUMPADSUB", VK_SUBTRACT,
	"TAB", VK_TAB,
	"ENTER", VK_RETURN,
	"ESC", VK_ESCAPE,
	"SPACE", ' ',
	"!", '!',
	"UP", VK_UP,
	"DOWN", VK_DOWN,
	"F1", VK_F1,
	"F2", VK_F2,
	"F3", VK_F3,
	"F4", VK_F4,
	"F5", VK_F5,
	"F6", VK_F6,
	"F7", VK_F7,
	"F8", VK_F8,
	"F9", VK_F9,
	"F10", VK_F10,
	"F11", VK_F11,
	"F12", VK_F12,
	"SLASH", VK_DIVIDE };

VOID SetKeyDelay(int d) {
	dprintf(DEBUG_AUTOMATION, "Setting key delay to %dms", d);
	keydelay = d;
	return; }

// FIXME: this should be generalized and moved to util.cpp
char tablelookup(char *k) {
	for (int i = 0; i < NUM_KEY_ENTRIES; i++) {
		if (!strcmp(k, KeyTable[i].string)) {
			return KeyTable[i].code; } }
	return FALSE; }

VOID EnQueue(MESSAGE *message) {
	EnterCriticalSection(&AutomationMutex);
	/*while (fQueueing) {}
	fQueueing = TRUE;
	BOOL fAccessingMqOld = fAccessingMq;
	fAccessingMq = TRUE;*/
	mq->AddItem(message);
	/*fAccessingMq = fAccessingMqOld;
	fQueueing = FALSE;*/
	LeaveCriticalSection(&AutomationMutex); }

VOID SendString(char *string) {
	char SendStringBuf[64], keyname[64];
	int result = 0;
	int repeat = 1;
	
	dprintf(DEBUG_KEYS_CLICKS, "SendString: Sending string \"%s\"", string);
	for (int i = 0; i < strlen(string) && string[i] != '\0'; i++) {
		repeat = 1;
		int named = 0;
		
		switch (string[i]) {
		case '{':
			named = 1;
			sscanf(string+i+1, "%[^}]s", SendStringBuf);
			i += strlen(SendStringBuf) + 1;
			
			if (IsIn(' ', SendStringBuf)) {
				sscanf(SendStringBuf, "%s %d", keyname, &repeat); }
			else {
				strcpy(keyname, SendStringBuf); }
			
			if (strlen(keyname) == 1) {
				result = keyname[0]; }
			else {
				result = tablelookup(keyname); }
			
			break;
		default:
			result = string[i];
			break; }
		
		if (result != 0 && repeat > 0) {
			MESSAGE *message = new MESSAGE;
			dprintf(DEBUG_AUTOMATION, "SendString: String[%d] = 0x%X result = 0x%x", i, string[i], result);
			message->Type = MESSAGE_KEY;
			if (named) {
				strcpy(message->KeyName, keyname); }
			else {
				strcpy(message->KeyName, ""); }
			memcpy(&message->KeyCode, &result, sizeof(int));
			memcpy(&message->Repeat, &repeat, sizeof(int));
			EnQueue(message); } } }

VOID PRIVATE RightClick(int x, int y) {
	//x += 3 * fWindowed;
	//y += 21 * fWindowed;
	MESSAGE *message = new MESSAGE;
	dprintf(DEBUG_KEYS_CLICKS, "Right clicking at x=%d y=%d", x, y);
	message->Type = MESSAGE_CLICK;
	message->Button = BUTTON_RIGHT;
	message->x = x;
	message->y = y;
	EnQueue(message); }

VOID PRIVATE LeftClick(int x, int y) {
	//x += 3 * fWindowed;
	//y += 21 * fWindowed;
	MESSAGE *message = new MESSAGE;
	dprintf(DEBUG_KEYS_CLICKS, "Left clicking at x=%d y=%d", x, y);
	message->Type = MESSAGE_CLICK;
	message->Button = BUTTON_LEFT;
	message->x = x;
	message->y = y;
	EnQueue(message); }

VOID PRIVATE SendPacketToServer(LPBYTE aMsg, DWORD aLen) {
	MESSAGE *message = new MESSAGE;
	message->Type = MESSAGE_PACKET;
	memcpy(message->PacketData, aMsg, aLen);
	message->len = aLen;
	EnQueue(message); }

VOID PRIVATE Pause(WORD length) {			// length in milliseconds
	MESSAGE *message = new MESSAGE;
	message->Type = MESSAGE_PAUSE;
	memcpy(&message->Pause, &length, sizeof(WORD));
	EnQueue(message); }

DWORD WINAPI AutomationThreadProc(LPVOID lpParameter) {
    THREAD_DATA *tdAutomation = (THREAD_DATA *)lpParameter;
	HWND h = tdAutomation->hD2Window;
	LinkedItem *linkeditem;
	MESSAGE *message;
	int MaxLenMq = 0;

	InitializeCriticalSection(&AutomationMutex);

	//dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: entered AutomationThreadProc");
	while (tdAutomation->Active) {
		if (/*!fAccessingMq && */(linkeditem = mq->GetFirstItem())) {
			EnterCriticalSection(&AutomationMutex);
			if (mq->GetItemCount() > 100) {
				dprintf(DEBUG_ALL,
					"WARNING: Automation message queue back-up: %d items now",
					mq->GetItemCount()); }
			//fAccessingMq = 1;
			message = (MESSAGE *)linkeditem->lpData;
			if (!message) {
				dprintf(DEBUG_ALL, "WARNING: Null message received by AutomationThreadProc"); }
			else {
				Sleep(keydelay);
				if (mq->GetItemCount() > MaxLenMq) {
					MaxLenMq = mq->GetItemCount(); }
				dprintf(DEBUG_AUTOMATION,
					"AutomationThreadProc: Got a message (queue len = %d, max len = %d)",
					mq->GetItemCount(), MaxLenMq);
				switch (message->Type) {
					
				case MESSAGE_KEY: {
					int key = message->KeyCode;
					char keybuf[5];
					sprintf(keybuf, "%c", key);
					short vk = VkKeyScan(keybuf[0]) & 0xFF;
					if (strlen(message->KeyName) > 1) {
						vk = message->KeyCode; }
					short shiftkey = VkKeyScan(keybuf[0]) & 256;
					char oemchar[3] = "  ";				// 2 character buffer
					CharToOem(keybuf, &oemchar[0]);
					DWORD scan = OemKeyScan(oemchar[0]) & 0xFF;
					int upperkey = isalpha(key) ? toupper(key) : key;
					DWORD scancode = (MapVirtualKey(key, 0) << 16);
					dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: message->Keycode = 0x%X scancode = %X Repeat = %d", key, scancode, message->Repeat);
					for (int j = 0; j < message->Repeat; j++) {
						if (key == VK_RETURN || key == VK_ESCAPE) {
							dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: saw ENTER or ESC, sleeping");
							Sleep(dClick); }
						if (strlen(message->KeyName) > 1) {
							dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: Sending keystroke %s", message->KeyName); }
						else {
							dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: Sending keystroke '%c'", key); }
						// FIXME:
						// shift up/down is not working, at least D2 doesn't see it
						if (isalpha(key) && isupper(key)) {
							dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: Shift down");
							PostMessage(h, WM_KEYDOWN, 0x00000010, 0x002a0001); }
						PostMessage(h, WM_KEYDOWN, (WPARAM)vk, (LPARAM)(scan | 0x00000001));
						//PostMessage(h, WM_CHAR, (WPARAM)key, (LPARAM)(scan | 0x00000001));
						PostMessage(h, WM_KEYUP, (WPARAM)vk, (LPARAM)(scan | 0xC0000001));
						if (isalpha(key) && isupper(key)) {
							dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: Shift up");
							PostMessage(h, WM_KEYUP, 0x00000010, 0xc02a0001); }
						if (key == VK_RETURN || key == VK_ESCAPE) {
							dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: saw ENTER or ESC, sleeping (again)");
							Sleep(dClick); } }
					break; }
					
				case MESSAGE_CLICK: {
					int pos = ((short)message->y << 16) + (short)message->x;
					WPARAM down = (message->Button == BUTTON_RIGHT ? WM_RBUTTONDOWN : WM_LBUTTONDOWN);
					WPARAM up = (message->Button == BUTTON_RIGHT ? WM_RBUTTONUP : WM_LBUTTONUP);
					LPARAM mk = (message->Button == BUTTON_RIGHT ? MK_RBUTTON : MK_LBUTTON);
					dprintf(DEBUG_AUTOMATION, "AutomationThreadProc: processing mouse click down=%d, up=%d, mk=%d, x=%d, y=%d, pos=%d", down, up, mk, message->x, message->y, pos);
					PostMessage(h, WM_MOUSEMOVE, NULL, pos); Sleep(20);
					PostMessage(h, down, mk, (LPARAM)pos); Sleep(10);
					PostMessage(h, up, NULL, (LPARAM)pos); Sleep(10);
					Sleep(dClick);
					break; }
					
				case MESSAGE_PACKET: {
					server->GameSendPacketToServer(message->PacketData, message->len);
					break; }
					
				case MESSAGE_PAUSE: {
					Sleep(message->Pause);
					break; }
					
				default: {
					dprintf(DEBUG_ALL, "WARNING: Invalid message received by keys & clicks thread");
					break; } } }
			
			mq->RemoveItem(linkeditem);
			//fAccessingMq = 0;
			LeaveCriticalSection(&AutomationMutex); }
		else { 
			// FIXME
			Sleep(2); } }

	dprintf(DEBUG_ALL, "Keystrokes, mouseclicks, and packets thread exiting.  Maximum queue length: %d", MaxLenMq);
	DeleteCriticalSection(&AutomationMutex);
	return 0; }



/*
WM_KEYDOWN nVirtKey:'Q'  [wParam:00000051 lParam:00100001]
WM_CHAR chCharCode:'q' (113)  [wParam:00000071 lParam:00100001]
WM_KEYUP nVirtKey:'Q'  [wParam:00000051 lParam:C0100001]

WM_KEYDOWN nVirtKey:VK_SHIFT  [wParam:00000010 lParam:002A0001]
WM_KEYDOWN nVirtKey:'Q'  [wParam:00000051 lParam:00100001]
WM_CHAR chCharCode:'Q' (81)  [wParam:00000051 lParam:00100001]
WM_KEYUP nVirtKey:'Q'  [wParam:00000051 lParam:C0100001]
WM_KEYUP nVirtKey:VK_SHIFT  [wParam:00000010 lParam:C02A0001]
*/

/*
WM_LBUTTONDOWN fwKeys:MK_LBUTTON xPos:16 yPos:169 [wParam:00000001 lParam:00A90010]
WM_LBUTTONUP fwKeys:0000 xPos:16 yPos:169 [wParam:00000000 lParam:00A90010]
*/
