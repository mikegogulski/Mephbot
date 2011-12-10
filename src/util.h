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
#include "logging.h"
#include "automation.h"
#include "pktk/DiabloItems.h"

extern VOID ReadStatDword(char *, char *, DWORD &);
extern VOID WriteStatDword(char *, char *, DWORD);
extern VOID ReadStatString(char *, char *, char *);
extern VOID WriteStatString(char *, char *, char *);
extern VOID ReadConfigInt(char *, int &);
extern VOID ReadConfigString(char *, char *);
extern char *RandString(int);
extern DWORD Dist2(DWORD, DWORD, DWORD, DWORD);
extern DWORD Dist(DWORD, DWORD, DWORD, DWORD);
extern VOID CreateFlagFile(char *);
extern VOID RemoveFlagFile(char *);
extern char *toBinary(BYTE);
extern BOOL ChatSocketDetected(VOID);
extern BOOL GameSocketDetected(VOID);
extern BOOL IsIn(char, char *);
extern VOID StripWhitespace(char *, char *);
extern VOID AddItemToInventory(CDiabloItems *);
extern LinkedList *Movables;
extern VOID MoveItems(VOID);
extern VOID ClearInventory(VOID);
extern DWORD WINAPI UtilThreadProc(LPVOID);
extern THREAD_DATA tdUtil;
extern CRITICAL_SECTION UtilMutex;
extern BOOL GetItemDimensions(int &, int &, CDiabloItems *);
