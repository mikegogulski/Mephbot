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
#pragma once
#define WIN32_LEAN_AND_MEAN
#include "mephbot.h"

#define MESSAGE_KEY		1
#define	MESSAGE_CLICK	2
#define MESSAGE_PACKET	3
#define MESSAGE_PAUSE	4

#define BUTTON_RIGHT	1
#define	BUTTON_LEFT		2

typedef struct thread_data_t
{
	// Thread ID
	HANDLE Thread;

	// D2 Window handle
	HWND hD2Window;

	// TRUE if in a game
	BOOL Active;
    THISGAMESTRUCT *thisgame;

    // Message queue
	LinkedList *mq;

} THREAD_DATA;

typedef struct message_t {
	int Type;
	int KeyCode;
	char KeyName[64];
	int Delay;
	int Repeat;
	int Button;
	int x;
	int y;
	BYTE PacketData[4096];
	DWORD len;
	WORD Pause;
} MESSAGE;

typedef struct keytable_t {
	char *string;
	char code;
} KEYTABLE;

#define NUM_KEY_ENTRIES		21

extern VOID SendString(char *);
extern VOID SetKeyDelay(int);
extern VOID LeftClick(int, int);
extern VOID RightClick(int, int);
extern VOID SendPacketToServer(LPBYTE, DWORD);
extern VOID Pause(WORD);
extern DWORD WINAPI AutomationThreadProc(LPVOID);
extern THREAD_DATA tdAutomation;
extern CRITICAL_SECTION AutomationMutex;

