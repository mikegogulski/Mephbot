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

extern FILE *hLogFile;			// filehandle we'll use for logging throughout the run

extern VOID DumpPacket(BYTE *, DWORD, int);
extern VOID PRIVATE dprint(char *);
extern VOID PRIVATE dprintf(int, const char *, ...);
extern VOID PRIVATE WriteLog(char *);
extern VOID PRIVATE SetUpLogInfo(char *buf, DWORD tglobal, DWORD tgame, DWORD restarts, DWORD aborts, DWORD runs, DWORD kills, DWORD deaths, DWORD chickens);
extern VOID PRIVATE LogInfo(char *buf, DWORD tglobal, DWORD tgame, DWORD restarts, DWORD aborts, DWORD runs, DWORD kills, DWORD deaths, DWORD chickens);

// NOTE: DEBUG_ALL is both a user config setting for all messages,
// AND a tag indicating we should output debug messages regardless
// of user selected debug level
#define	DEBUG_ALL					0
#define	DEBUG_MISC					1
#define	DEBUG_CHANGE_STATE			2
#define	DEBUG_PLAYER_MOVE			4
#define	DEBUG_CONFIG				8
#define	DEBUG_SKILLS				16
#define	DEBUG_PLAYER_UPDATES		32
#define	DEBUG_ITEMS					64
#define	DEBUG_KEYS_CLICKS			128
#define	DEBUG_UNIT_MOVE				256
#define	DEBUG_UNIT_UPDATES			512
#define	DEBUG_PACKETS_PKTIDENT		1024
#define	DEBUG_PACKETS_ASCII			2048
#define	DEBUG_PACKETS				4096
#define	DEBUG_FILE_ACCESS			8192
#define	DEBUG_SOLVE					16384
#define	DEBUG_UNIT_INSTANTIATION	32768
#define	DEBUG_AUTOMATION			65536
