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
#include <windows.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>

#include "external/D2Hackit.h"
#include "external/D2Client.h"

#define	VERSION_MAJOR	0
#define	VERSION_MINOR	91

//////////////////////////////////////////////////////////////////////////////
//
// globals
//
//////////////////////////////////////////////////////////////////////////////

extern BOOL inGame;
extern WORD StairsX, StairsY;
extern HWND hD2Window;
extern int DebugLevel;
extern BOOL fDebug, fDebugToScreen, fMoveItems, fItemJustDroppedToStorage, fCubeOpen;
extern char szLogFileName[];
extern LinkedList *mq;
extern BOOL fWindowed;
extern WORD CurrWeapon;
extern DWORD dwTickGlobal;
extern DWORD Tick1;
extern DWORD NumAborts;
extern int CurrentSkill[];
extern int DeltaTeleport, DeltaTeleport2;
extern WORD WpDurance2X, WpDurance2Y;
extern DWORD CubeId;
extern int dClick;

//////////////////////////////////////////////////////////////////////////////
//
// prototypes
//
//////////////////////////////////////////////////////////////////////////////
extern BOOL PRIVATE InstallPatches();
extern VOID PRIVATE RemovePatches();
extern VOID PRIVATE SetUpLogInfo(VOID);
extern BOOL PRIVATE OnCommandDebug(char **, int);
extern BOOL PRIVATE OnCommandHelp(char **, int);
extern BOOL PRIVATE OnCommandQuitGame(char **,int);
extern DWORD PRIVATE GetDistanceSquared(DWORD, DWORD, DWORD, DWORD);
extern VOID PRIVATE ExitGame(VOID);
extern VOID PRIVATE ChangeState(int);
extern VOID PRIVATE OverheadInfo(VOID);
extern BOOL PRIVATE InGame(VOID);

//////////////////////////////////////////////////////////////////////////////
//
// constants
//
//////////////////////////////////////////////////////////////////////////////
// BE CAREFUL HERE!!! Must update strings in mephbot.cpp as well.
enum {
	STATE_UNKNOWN = 0,
	STATE_GAME_STARTED,
	STATE_WALK_TO_ORMUS,
	STATE_OPENING_STORE,
	STATE_STORE_OPEN,
	STATE_CLOSING_STORE,
	STATE_WALK_TO_STASH,
	STATE_OPEN_STASH,
	STATE_CLOSE_STASH,
	STATE_WALK_TO_WAYPOINT,
	STATE_TAKE_WAYPOINT,
	STATE_CAST_PROTECTION,
	STATE_FIND_STAIRS,
	STATE_TELEPORT_TOWARD_STAIRS,
	STATE_TELEPORT_TO_STAIRS,
	STATE_TAKE_STAIRS,
	STATE_TELEPORT_TO_MEPH,
	STATE_RECOVER_MANA,
	STATE_ATTACK_MEPH,
	STATE_MEPH_DEAD,
	STATE_EXIT_GAME,
	STATE_START_NEW_GAME,
	STATE_START_ISSUED,
	STATE_PLAYER_DEAD,
	STATE_WALK_TO_HRATLI,
	STATE_REPAIR,
	MAXSTATES };


#define	WEAPON_PRIMARY				0
#define	WEAPON_SECONDARY			1

#define	DISTANCE_TOO_CLOSE			-1
#define	DISTANCE_IN_RANGE			0
#define	DISTANCE_TOO_FAR			1

//////////////////////////////////////////////////////////////////////////////
//
// macros
//
//////////////////////////////////////////////////////////////////////////////

#define	QUICKSPELLDELAY	(FastCastPctToTickDelay(FastCastPct))
#define	PLAYERX			(thisgame->player->PlayerPositionX)
#define	PLAYERY			(thisgame->player->PlayerPositionY)
#define	PLAYERID		(thisgame->player->PlayerID)
#define	ARRAY_SIZE(x)	(sizeof(x) / (sizeof(x[0])))

//////////////////////////////////////////////////////////////////////////////
//
// typedefs
//
//////////////////////////////////////////////////////////////////////////////

typedef struct Obj {
	//int type;		// npc/monster/fixed object
	DWORD id;
	WORD x, y;
} OBJ;

