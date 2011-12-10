//////////////////////////////////////////////////////////////////////////////
//
// mephbot.cpp by syadasti <mike@gogulski.com>
// Copyright (c) 2002 by Mike Gogulski
//
// Based on original code by Mousepad, ackmed, thohell, Gayak, g0llum, icky,
// Onlyer, YobGuls, Mozart McLaus, FallNAngel, Jonathan Wu, druttis, Cigamit
// masterste0, Herzog_Zwei, Nuttzy and others
//
//////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////
//
// Version History
//
// 0.1		Initial code
// 0.2		Added .ini file handling, repair, nova, many speed improvements
//			and bugfixes
// 0.3		Bunches more features and fixes, FIRST ALPHA RELEASE
// 0.4		Second alpha release, see mephbot-dev.txt for release notes
// 0.5		Quick bugfix mostly in STATE_ATTACK_MEPH, see mephbot-dev.txt
// 0.6		New stairs-finding method, message serialization, runs minimized,
//			no more keystrokes/mouseclicks (all packets), split single source
//			into multiple files, PKTK code integration, other fixes, see
//			mephbot-dev.txt
//			FIRST BETA RELEASE
// 0.7		Faster stairs-finding with Dijkstra's shortest path algorithm
//			Eliminated several user options. Fixed chicken code.
//			INI-configurable skill selection.  Improved precast logic.
//			Dodging code from Gayak.  Fixes to TeleportAway().
//			See mephbot-dev.txt
//			SECOND BETA RELEASE
// 0.8		Bugfix release.  See mephbot-dev.txt
//			FIRST GENERAL RELEASE
// 0.9		Move all unid items to stash/cube, packetized resurrect and corpse
//			pickup.  No more tr.d2h or mover.d2h in distribution (hooray!).
//			Various additional bugfixes and incremental improvements
//			See mephbot-dev.txt
// 0.91		Fixed a weapon switch bug introduced in 0.9
//			Fixed mana recovery bug for good (I hope)
//			Fixed a crashing error in the automation thread
//			Packetized weapon switch
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// DEPENDENCIES
//
// Microsoft Windows Platform SDK (for iphlpapi.h/.lib)
//
// d2hackit.dll			Diablo 2 hack framework
// pickit.d2h			item pickup
// pktident.ini			Packet decodes (debug only)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "mephbot.h"
#include "packethelper.h"
#include "waypoint.h"
#include "util.h"
#include "logging.h"
#include "automation.h"
#include "pktk/DiabloItems.h"
#include "skills.h"
#include "maphack.h"
#include "dijkstra.h"

//////////////////////////////////////////////////////////////////////////////
//
// globals, flags, buffers, etc.
//
//////////////////////////////////////////////////////////////////////////////
FUNCTIONENTRYPOINTS	*server;
THISGAMESTRUCT *thisgame;

BOOL	fInGame = FALSE,
		fModuleActive = TRUE,
		fJustLoaded = TRUE,
		fAlreadySwitchedWeapon = FALSE,
		fInventoryFull = FALSE,
		fGoldQuantityKnown = FALSE,
		fAlreadyBoughtPotions = FALSE,
		fAlreadyDepositing = FALSE,
		fFindingPathToStairs = FALSE,
		fNeedToBuyPotions = TRUE,
		fRepairGame = FALSE,
		fTelesafeAfterWait = FALSE,
		fWrongAct = FALSE,
		fDrinkHealingPotion = FALSE,
		fItemJustDroppedToStorage = FALSE,
		fMoveItems = FALSE,
		fCubeOpen = FALSE,
		fStashOpen = FALSE,
		fContinueWhenInventoryFull = FALSE,
		fAlreadyRecoveredMana = FALSE;;

DWORD	Tick1 = 0,
		dwTickStairs = 0,
		dwTickWaypoint = 0,
		dwTickGlobal = 0,
		dwTickGame = 0,
		dwTickHealingPotion = 0,
		state = 0,
		nextstate = 0,
		NumRuns = 0,
		NumKills = 0,
		NumDeaths = 0,
		NumAborts = 0,
		NumRestarts = 0,
		NumChickens = 0,
		dwCorpseId = 0;

DWORD InventoryGold = 0, StashGold = 0;

DWORD dwMephId = 0;
WORD wMephX = 0, wMephY = 0;
int iMephHealth = 100;				// Percentage
DWORD dwOrmusId = 0;
WORD wOrmusX = 0, wOrmusY = 0;
DWORD dwHratliId = 0;
WORD wHratliX = 0, wHratliY = 0;
WORD WpIdKurastDocks = 0;
WORD WpKurastDocksX = 0, WpKurastDocksY = 0;
WORD WpDurance2X = 0, WpDurance2Y = 0;
WORD CurrentHP = 0, CurrentMP = 0, MaxHP = 0, MaxMP = 0, LastHP = 0, LastMP = 0;
BYTE bMephPathIndex = 0, bOrmusPathIndex = 0, bStashPathIndex = 0, bHratliPathIndex = 0;

char	szInfoBuf[1024];
char	szRunName[32];
char	szInGameFlagFileName[_MAX_PATH],
		szInvFullFlagFileName[_MAX_PATH],
		szWrongActFlagFileName[_MAX_PATH];
char	szWeaponPrimary[64],
		szShieldPrimary[64],		
		szWeaponCurrent[64],
		szShieldCurrent[64];

BOOL	fWeaponCurrent = WEAPON_PRIMARY,
		fWeaponPrimary = WEAPON_PRIMARY,
		fWeaponDesired = WEAPON_PRIMARY;

WORD oldpx, oldpy;

DWORD dwStairsObjID = 0x00;

LinkedList *MessageQueue = new LinkedList;
LinkedList *ExcessPotions = new LinkedList;
LinkedList *BeltPotions = new LinkedList;
LinkedList *mq;

BYTE CurrentLevel = 0;
HWND hD2Window;
FILE *hLogFile;					// filehandle we'll use for logging throughout the run

THREAD_DATA tdAutomation, tdUtil;
int cMephMissing = 0;

int DeltaTeleport = 40;
int DeltaTeleportAway = 4;					// when fleeing Mephisto... maybe calculateable?

int cNumPotionsInBelt = 0;
int ProtectionSkillIndex = 0;

int cNoTeleportProgress = 0;

int currentsafe = 0, lastsafe = 0;

DWORD CubeId = (DWORD)-1;

DWORD BestPotionID = 0;
BYTE BestPotionType = 0;

CRITICAL_SECTION MainMutex;

//////////////////////////////////////////////////////////////////////////////
//
// User-settable variables from mephbot.ini
// These are the burned-in defaults
// All time values are in tenths of a second
//
// FIXME:  I'm not really maintaining this anymore, just the INI defaults
// probably need to sync these with those before general release
//
// Synced 8/25/02 for 0.8 release
//
//////////////////////////////////////////////////////////////////////////////

BOOL fDebug = FALSE;
BOOL fDebugToScreen = FALSE;
int DebugLevel = DEBUG_MISC | DEBUG_CHANGE_STATE;
BOOL fWindowed = TRUE;
BOOL fWeaponSwitch = FALSE;
BOOL fOverheadInfo = TRUE;
BOOL fDodging = TRUE;
int MinDistToMeph2 = 8 * 8;
int MaxDistToMeph2 = 15 * 15;
int DelayStep = 10;
int DelayGameLoad = 50;
int FastCastPct = 0;
int DelayExitGame = 20;
int DelayStartNewGame = 60;
int DelayCreateGame = 80;
int DelayAfterDeath = 30;
int DelayOverheadUpdate = 5;
int DelayBeforePrecast = 5;
int DelayPickup = 10;
int dClick = 150;
int MaxTickRestart = 250;
int MaxTickWaypoint = 400;
int MaxTickStairs = 600;
int MaxTickGame = 1800;
int HealingLifePercent = 70;
int ChickenLifePercent = 20;
int ComboChickenLifePercent = 40;
int ComboChickenManaPercent = 20;
int MinManaPercentBeforeAttackRun = 90;
int StashVisitGameNumber = 10;
char szLogFileName[_MAX_FNAME] = "mephbotlog.txt";	// no default == fopen() assert failure
char szMephbotDir[_MAX_PATH] = "";					// current directory
int MephWeaponSwitchHealth = 10;
char szAwayMessage[1024] = "";						// random by default
char szGameNamePrefix[1024] = "";					// first 3 chars of char name by default

// Putting defaults here will cause errors
char ProtectionSkill[8][64] = { "", "", "", "", "", "", "", "" };
char PrimaryAttackSkill[64] = "Frozen Orb";
int PrimaryAttackHand = SKILL_LEFT;
char SecondaryAttackSkill[64] = "Glacial Spike";
int SecondaryAttackHand = SKILL_RIGHT;

int MephMinHealthToCastStatic = 50;					// for hell

//////////////////////////////////////////////////////////////////////////////
//
// Defines, statics
//
//////////////////////////////////////////////////////////////////////////////

char *StateNames[MAXSTATES] = {
	"STATE UNKNOWN!",
	"Game Started",
	"Walking to Ormus",
	"Talking to Ormus",
	"Store open",
	"Closing store",
	"Walking to stash",
	"Opening Stash",
	"Closing Stash",
	"Walking to waypoint",
	"Taking waypoint",
	"Casting protection",
	"Finding stairs",
	"Moving toward stairs",
	"Moving to stairs",
	"Taking stairs",
	"Moving to Mephisto",
	"Waiting for mana recovery",
	"Attacking Mephisto",
	"Waiting for pickup",
	"Exiting Game",
	"Starting new game",
	"Start issued",
	"Player dead",
	"Walking to Hratli",
	"Repairing" };

// approximately where Mephisto first spawns, above and left of the seal on the floor
WORD SafePlaceX = 0x4485, SafePlaceY = 0x1f82;

int aRandBox[4][2] = {
	{ 1, 0},
	{ 0, 1},
	{ 0,-1},
	{-1, 0} };

WORD PathToMeph[][2] = {
	{0x450d, 0x1f5e},
	{0x44fb, 0x1f5a},
	{0x44e2, 0x1f5b},
	{0x44d3, 0x1f65},
	{0x44c3, 0x1f6e},
	{0x44b3, 0x1f77},
	{0x44a3, 0x1f80},
	{0x449c, 0x1f85} };
	
WORD PathToOrmus[][2] = {
	{0x140a, 0x142e},
	{0x140c, 0x13ff},
	{0x140c, 0x13e8} };

WORD PathToStash[][2] = {
	{0x141d, 0x13e4},
	{0x141c, 0x13cc},
	{0x1416, 0x13c0} };
	
WORD PathToHratli[][2] = {
	{0x13fe, 0x13dd},
	{0x13fe, 0x13dd},
	{0x1410, 0x13df},
	{0x1410, 0x13df},
	{0x1426, 0x13df},
	{0x1426, 0x13df},
	{0x143e, 0x13df},
	{0x143e, 0x13df},
	{0x1455, 0x13df},
	{0x1455, 0x13df},
	{0x1464, 0x13df},
	{0x1464, 0x13df},
	{0x1463, 0x13ce},
	{0x1463, 0x13ce},
	{0x1467, 0x13c7},
	{0x1467, 0x13c7},
	{0x1467, 0x13af} };

// Contributed by Gayak <170@guyau.qc.ca>
WORD safes[][2] = {
	{0x449e,0x1f8d},
	{0x449e,0x1f7d},
	{0x44a8,0x1f8f},
	{0x44a8,0x1f7b},
	{0x4492,0x1f85},
	{0x44ab,0x1f85},
	{0x4492,0x1f8b},
	{0x4492,0x1f7d},
	{0x449e,0x1f89},
	{0x449a,0x1f81} };
// End Gayak code

// from Nuttzy's PKTK
/*
////
//// 4 bit inventory coordinates for 0,1,2,4, and 6
////
CB+			CB+
[05->06]	045->048	Body Code		where on the body this item is
										00 = not worn (stored)
										01 = head
										02 = amulet
										03 = body
										04 = weapon1
										05 = shield1
										06 = left ring
										07 = right ring
										08 = belt
										09 = boots
										0A = gloves
										0B = weapon2
										0C = shield2
*/
char *BodyCodes[13] = {
//#define BODY_NOT_WORN		0x00
	"not worn (stored)",
//#define	BODY_HEAD			0x01
	"head",
//#define BODY_AMULET			0x02
	"amulet",
//#define BODY_ARMOR			0x03
	"body (armor)",
#define	BODY_WEAPON_1		0x04
	"weapon 1",
#define BODY_SHIELD_1		0x05
	"shield 1",
//#define BODY_RING_LEFT		0x06
	"left ring",
//#define BODY_RING_RIGHT		0x07
	"right ring",
//#define BODY_BELT			0x08
	"belt",
//#define BODY_BOOTS			0x09
	"boots",
//#define BODY_GLOVES			0x0a
	"gloves",
#define BODY_WEAPON_2		0x0b
	"weapon 2",
#define BODY_SHIELD_2		0x0c
	"shield 2" };

char *ItemQuality[8] = {
	"crafted",
	"inferior",
	"normal",
	"superior",
	"magic",
	"set",
	"rare",
	"unique" };

//////////////////////////////////////////////////////////////////////////////
//
// Module info
//
//////////////////////////////////////////////////////////////////////////////

DWORD	ModuleVersion = MAKELONG(VERSION_MAJOR, VERSION_MINOR);
char	ModuleAuthor[] = "syadasti (based on code and ideas from Mousepad, ackmed, thohell, Gayak, g0llum, icky, YobGuls, Mozart McLaus, FallNAngel, Jonathan Wu, druttis, Cigamit, masterste0, Herzog_Zwei, Nuttzy and others)";
char	ModuleWebsite[] = "http://www.otaku-elite.com/syadasti";
char	ModuleDescription[] = "mephbot 0.9";
char	ModuleEmail[] = "mike@gogulski.com";

//////////////////////////////////////////////////////////////////////////////
//
// Module commands
//
//////////////////////////////////////////////////////////////////////////////

MODULECOMMANDSTRUCT ModuleCommands[]= {
	{ "debug", OnCommandDebug,
		"debugÿc0 toggles debug mode\n"	},
	{ "quitgame", OnCommandQuitGame,
		"quitgameÿc0 issues \"save and exit\" command\n" },
	{ "help", OnGameCommandHelp,
		"helpÿc0 display this help text\n" },
	{NULL}
};

//////////////////////////////////////////////////////////////////////////////
//
// Local functions
//
//////////////////////////////////////////////////////////////////////////////

VOID PRIVATE ChangeState(int newstate) {
	dprintf(DEBUG_CHANGE_STATE, "***** Changing state to ***** %s", StateNames[newstate]);
	state = newstate;
	return; }

BOOL PRIVATE InGame(VOID) {
	if (!fInGame) {
		return FALSE; }
	if (!thisgame) {
		return FALSE; }
	if (!thisgame->player) {
		return FALSE; }
	// we could add socket 4000 detection here, but that might be expensive
	return TRUE; }

BOOL PRIVATE InTown(VOID) {
	return (state == STATE_START_ISSUED ||
			state == STATE_GAME_STARTED ||
			state == STATE_WALK_TO_ORMUS ||
			state == STATE_OPENING_STORE ||
			state == STATE_STORE_OPEN ||
			state == STATE_CLOSING_STORE ||
			state == STATE_WALK_TO_STASH ||
			state == STATE_OPEN_STASH ||
			state == STATE_CLOSE_STASH ||
			state == STATE_WALK_TO_WAYPOINT ||
			state == STATE_TAKE_WAYPOINT ||
			state == STATE_WALK_TO_HRATLI ||
			state == STATE_REPAIR); }

BOOL PRIVATE InGameReset(VOID) {
	if (!InGame()) {
		Tick1 = 0;
		ChangeState(STATE_START_NEW_GAME);
		return FALSE; }
	return TRUE; }

VOID PRIVATE SwitchWeapon(BOOL request) {
	if (fWeaponSwitch) {
		dprintf(DEBUG_MISC, "Switching weapons");
		fWeaponDesired = request;
		WeaponSwitch(); } }

VOID PRIVATE WeaponReset(VOID) {
	if (fWeaponSwitch && InGameReset()) {
		fAlreadySwitchedWeapon = FALSE;
		if (strcmp(szWeaponPrimary, szWeaponCurrent)) {
			dprintf(DEBUG_MISC, "Resetting weapon to primary");
			SwitchWeapon(WEAPON_PRIMARY); } }
	return; }

BOOL PRIVATE TimedOutGettingToWaypoint(VOID) {
	if (dwTickWaypoint++ > MaxTickWaypoint) {
		dprintf(DEBUG_ALL, "ABORT: Stuck before taking waypoint");
		NumAborts++;
		Tick1 = 0;
		ChangeState(STATE_EXIT_GAME);
		return TRUE; }
	return FALSE; }

BOOL PRIVATE TimedOutReachingStairs(VOID) {
 	if (dwTickStairs++ > MaxTickStairs) {
		dprintf(DEBUG_ALL, "ABORT: Can't reach the stairs down");
		NumAborts++;
		ChangeState(STATE_EXIT_GAME);
		return TRUE; }
	return FALSE; }

VOID PRIVATE SendD2HackitCommand(char *Cmd) {
	dprintf(DEBUG_MISC, "Sending d2hackit command \"%s\"", Cmd);
	server->GameCommandLine(Cmd);
	Pause(dClick); }

VOID PRIVATE LoadConfig(VOID) {
	int tmp = 0;
	int fDebugOld = fDebug;
	char TempFname[_MAX_FNAME], TempString[1024] = "\0";

	ReadConfigInt("Debug", fDebug);
	if (fDebug && !fDebugOld) {
		ReadConfigInt("Debug", fDebug); }

	ReadConfigString("LogFileName", TempFname);
	ReadConfigString("MephbotDir", szMephbotDir);
	sprintf(szLogFileName, "%s%s%s", szMephbotDir, strlen(szMephbotDir) ? "\\" : "", TempFname);

	if (hLogFile = fopen(szLogFileName, "a+")) {
		dprintf(DEBUG_CONFIG, "Log file %s opened successfully", szLogFileName); }

	dprintf(DEBUG_CONFIG, "Loading config");
	ReadConfigInt("DebugToScreen", fDebugToScreen);
	ReadConfigInt("DebugLevel", DebugLevel);
	ReadConfigInt("Windowed", fWindowed);
	ReadConfigInt("WeaponSwitch", fWeaponSwitch);
	ReadConfigInt("OverheadInfo", fOverheadInfo);
	sprintf(szInGameFlagFileName, "%s%smephbot-ingame.tmp",
		szMephbotDir, strlen(szMephbotDir) ? "\\" : "");
	sprintf(szInvFullFlagFileName, "%s%smephbot-invfull.tmp",
		szMephbotDir, strlen(szMephbotDir) ? "\\" : "");
	sprintf(szWrongActFlagFileName, "%s%smephbot-wrongact.tmp",
		szMephbotDir, strlen(szMephbotDir) ? "\\" : "");

	ReadConfigString("GameNamePrefix", szGameNamePrefix);
	if (!strlen(szGameNamePrefix)) {
		char *slot, *acct;
		slot = server->GetHackProfileString("mephbot", "_global_", "DefaultSlot");
		acct = server->GetHackProfileString("mephbot", slot, "Accountname");
		sprintf(szGameNamePrefix, "%s", acct); }
	szGameNamePrefix[3] = (char)NULL;

	ReadConfigInt("MephWeaponSwitchHealth", MephWeaponSwitchHealth);
	ReadConfigInt("StashVisitGameNumber", StashVisitGameNumber);
	if (!StashVisitGameNumber) {
		StashVisitGameNumber = 5; }
	ReadConfigInt("ContinueWhenInventoryFull", fContinueWhenInventoryFull);
	ReadConfigInt("Dodging", fDodging);
	ReadConfigInt("HealingLifePercent", HealingLifePercent);
	ReadConfigInt("ChickenLifePercent", ChickenLifePercent);
	ReadConfigInt("ComboChickenLifePercent", ComboChickenLifePercent);
	ReadConfigInt("ComboChickenManaPercent", ComboChickenManaPercent);
	ReadConfigInt("MinManaPercentBeforeAttackRun", MinManaPercentBeforeAttackRun);
	ReadConfigString("AwayMessage", szAwayMessage);
	ReadConfigInt("MinDistanceToMeph", tmp);
	if (tmp) {
		MinDistToMeph2 = tmp * tmp; }
	tmp = 0;
	ReadConfigInt("MaxDistanceToMeph", tmp);
	if (tmp) {
		MaxDistToMeph2 = tmp * tmp; }
	tmp = 0;
	// FIXME: huh???
	DeltaTeleportAway = ((int)(sqrt((double)MaxDistToMeph2) - sqrt((double)MinDistToMeph2))) * 2 / 3;
	ReadConfigInt("DelayStep", DelayStep);
	ReadConfigInt("DelayGameLoad", DelayGameLoad);
	ReadConfigInt("FastCastPercent", FastCastPct);
	ReadConfigInt("DelayExitGame", DelayExitGame);
	ReadConfigInt("DelayStartNewGame", DelayStartNewGame);
	ReadConfigInt("DelayCreateGame", DelayCreateGame);
	ReadConfigInt("DelayAfterDeath", DelayAfterDeath);
	ReadConfigInt("DelayOverheadUpdate", DelayOverheadUpdate);
	ReadConfigInt("DelayBeforePrecast", DelayBeforePrecast);
	ReadConfigInt("DelayPickup", DelayPickup);
	dClick = atoi(server->GetHackProfileString("mephbot", "_expert_", "dClick"));
	ReadConfigInt("MaxTickRestart", MaxTickRestart);
	ReadConfigInt("MaxTickWaypoint", MaxTickWaypoint);
	ReadConfigInt("MaxTickStairs", MaxTickStairs);
	ReadConfigInt("MaxTickGame", MaxTickGame);

	ReadConfigString("ProtectionSkill1", ProtectionSkill[0]);
	ReadConfigString("ProtectionSkill2", ProtectionSkill[1]);
	ReadConfigString("ProtectionSkill3", ProtectionSkill[2]);
	ReadConfigString("ProtectionSkill4", ProtectionSkill[3]);
	ReadConfigString("ProtectionSkill5", ProtectionSkill[4]);
	ReadConfigString("ProtectionSkill6", ProtectionSkill[5]);
	ReadConfigString("ProtectionSkill7", ProtectionSkill[6]);
	ReadConfigString("ProtectionSkill8", ProtectionSkill[7]);

	ReadConfigString("PrimaryAttackSkill", PrimaryAttackSkill);
	ReadConfigString("PrimaryAttackHand", TempString);
	if (!stricmp(TempString, "left")) {
		PrimaryAttackHand = SKILL_LEFT; }
	else if (!stricmp(TempString, "right")) {
		SecondaryAttackHand = SKILL_RIGHT; }
	else {
		dprintf(DEBUG_ALL, "ERROR: PrimaryAttackHand configuration \"%s\" invalid"); }

	TempString[0] = '\0';

	ReadConfigString("SecondaryAttackSkill", SecondaryAttackSkill);
	ReadConfigString("SecondaryAttackHand", TempString);
	if (!stricmp(TempString, "left")) {
		SecondaryAttackHand = SKILL_LEFT; }
	else if (!stricmp(TempString, "right")) {
		SecondaryAttackHand = SKILL_RIGHT; }
	else {
		dprintf(DEBUG_ALL, "ERROR: SecondaryAttackHand configuration \"%s\" invalid"); }

	TempString[0] = '\0';

	dprintf(DEBUG_CONFIG, "Config loaded");
	ReadConfigInt("Debug", fDebug);
	if (!fJustLoaded) {
		fDebug = fDebugOld; }
	server->GamePrintInfo(fDebug ? "mephbot: Debugging enabled" : "mephbot: Debugging disabled");
	return; }

VOID PRIVATE LoadStats(VOID) {
	char *LoadStatsBuf;
	LoadStatsBuf = server->GetHackProfileString("mephbot-stats", "current_run", "RunName");
	strcpy(szRunName, LoadStatsBuf);
	if (strlen(szRunName)) {
		ReadStatDword(szRunName, "tGlobal", dwTickGlobal);
		ReadStatDword(szRunName, "NumRuns", NumRuns);
		ReadStatDword(szRunName, "NumKills", NumKills);
		ReadStatDword(szRunName, "NumDeaths", NumDeaths);
		ReadStatDword(szRunName, "NumAborts", NumAborts);
		ReadStatDword(szRunName, "NumRestarts", NumRestarts);
		ReadStatDword(szRunName, "NumChickens", NumChickens);
		ReadStatString(szRunName, "PrimaryWeapon", szWeaponPrimary);
		ReadStatString(szRunName, "CurrentWeapon", szWeaponCurrent); } }

VOID PRIVATE WriteStats(VOID) {
	if (strlen(szRunName)) {
		WriteStatDword(szRunName, "tGlobal", dwTickGlobal);
		WriteStatDword(szRunName, "NumRuns", NumRuns);
		WriteStatDword(szRunName, "NumKills", NumKills);
		WriteStatDword(szRunName, "NumDeaths", NumDeaths);
		WriteStatDword(szRunName, "NumAborts", NumAborts);
		WriteStatDword(szRunName, "NumChickens", NumChickens);
		WriteStatString(szRunName, "CurrentWeapon", szWeaponCurrent); } }

VOID PRIVATE TeleportTo(WORD x, WORD y) {
	oldpx = PLAYERX;
	oldpy = PLAYERY;
	dprintf(DEBUG_PLAYER_MOVE, "Teleporting to (%.4X, %.4X) distance = %d", x, y,
		(int)Dist(x, y, PLAYERX, PLAYERY));
	CastSkillOnLocationByName("Teleport", SKILL_RIGHT, x, y);
	return; }

// both of these functions were just wrong... we needed to do some trig here
// okay.. so after doing the trig we discover no trig is needed

VOID PRIVATE TeleportToward(WORD x, WORD y, int delta) {
	WORD targetx, targety;
	WORD r = (WORD)Dist(PLAYERX, PLAYERY, x, y);
	if (delta > 0 && r <= (WORD)delta) {
		targetx = x;
		targety = y; }
	else {
		targetx = PLAYERX + ((WORD)delta * (x - (WORD)PLAYERX)) / (r > 0 ? r : 1);
		targety = PLAYERY + ((WORD)delta * (y - (WORD)PLAYERY)) / (r > 0 ? r : 1); }
	TeleportTo(targetx, targety);
	return; }

// This function is poorly named... it's really for teleporting away from
// or closer to Mephisto.
// The "flag" argument is the result from RangeToTarget()
VOID PRIVATE TeleportAway(int flag, int delta, WORD x, WORD y) {
	TeleportToward(x, y, flag * delta);
	return; }

// the hideous "drunkard's teleport" method for solving the maze.
// Not very efficient
// add: bias against teleporting directly away from stairs
VOID PRIVATE TeleportRandomly() {
	int rnd = rand() % 4;
	dprintf(DEBUG_PLAYER_MOVE, "Teleporting randomly (%d,%d)",
		DeltaTeleport * aRandBox[rnd][0], DeltaTeleport * aRandBox[rnd][1]);
	TeleportTo(PLAYERX + DeltaTeleport * aRandBox[rnd][0],
		PLAYERY + DeltaTeleport * aRandBox[rnd][1]);
	return; }

VOID PRIVATE TeleportRandomly(int delta) {
	int oDeltaTeleport = DeltaTeleport;
	DeltaTeleport = delta;
	TeleportRandomly();
	DeltaTeleport = oDeltaTeleport;
	return; }

// Contributed by Gayak <170@guyau.qc.ca>
BOOL Telesafe() {
	int chosen = 0;
	DWORD distance[10];
	int best = 16;
	int variation = 1;
	
	for (int xp = 0; xp <= 9; xp++) {
		distance[xp] = Dist2(wMephX, wMephY, safes[xp][0], safes[xp][1]); }
	while (!chosen) {
		for (xp = 0; xp <= 9; xp++) {
			if (distance[xp] <  (best + variation) * (best + variation) &&
				distance[xp] > (best - variation) * (best - variation) &&
				currentsafe != xp &&
				lastsafe != xp) {
				chosen = xp;
				break; } }
		variation++;
		if (variation == 24) {
			dprintf(DEBUG_ALL, "WARNING: Problem with telesafe()");
			break; } }
	TeleportToward(safes[chosen][0], safes[chosen][1], DeltaTeleport);
	lastsafe = currentsafe;
	currentsafe = chosen;
	
	return TRUE; }
// End Gayak code

int PRIVATE RangeToTarget(WORD x, WORD y) {
	DWORD ret = 0;
	ret = Dist2(PLAYERX, PLAYERY, x, y);
	dprintf(DEBUG_UNIT_MOVE, "Range to target: %d", (int)sqrt((double)ret));
	if (ret > MaxDistToMeph2) {
		return DISTANCE_TOO_FAR; }
	if (ret < MinDistToMeph2) {
		return DISTANCE_TOO_CLOSE; }
	return DISTANCE_IN_RANGE; }

VOID PRIVATE ExitGame(VOID) {
	dprintf(DEBUG_MISC, "Exiting game");
	//SendD2HackitCommand("pickit deactivate");
	WeaponReset();
	fJustLoaded = FALSE;
	RemoveFlagFile(szInGameFlagFileName);
	D2ExitGame(0);
	return; }

VOID PRIVATE Chicken(VOID) {
	NumChickens++;
	Tick1 = DelayExitGame;
	ChangeState(STATE_EXIT_GAME); }

/*	Chat-ovrhed PlayerID--- ?? ?? NUL----Text--- NUL
	0  1  2  3  4  5  6  7  8  9  10 11
	26 05 00 00 0d 00 00 00 af 44 00 74 65 73 74 00  (null)  00000000D0test0
	26 05 00 00 11 00 00 00 00 e0 00 74 65 73 74 00  (null)  0000000000test0 */

VOID PRIVATE OverheadInfo(VOID) {
	if (fOverheadInfo && InGame()) {
		char OverheadInfoBuf[4096];
		SetUpLogInfo(szInfoBuf, dwTickGlobal, dwTickGame, NumRestarts, NumAborts, NumRuns, NumKills, NumDeaths, NumChickens);
		sprintf(OverheadInfoBuf, "ÿc3------ÿc1Mephbotÿc6-ÿc1Statusÿc3------ ");
		sprintf(OverheadInfoBuf, "%sÿc7%s ", OverheadInfoBuf, StateNames[state]);
		sprintf(OverheadInfoBuf, "%sÿc0%s ", OverheadInfoBuf, szInfoBuf);
		sprintf(OverheadInfoBuf, "%sÿc7x%.4X y%.4X Lv%d ",
			OverheadInfoBuf, PLAYERX, PLAYERY, (int)CurrentLevel);
		sprintf(OverheadInfoBuf, "%sÿc7Life:%d%s Mana:%d%s ",
			OverheadInfoBuf,
			MaxHP ? CurrentHP * 100 / MaxHP : CurrentHP, MaxHP ? "%" : "",
			MaxMP ? CurrentMP * 100 / MaxMP : CurrentMP, MaxMP ? "%" : "");
		if (CurrentLevel == 101) {
			sprintf(OverheadInfoBuf, "%sÿc7Distance to Stairs: ÿc1%d ", OverheadInfoBuf, Dist(PLAYERX, PLAYERY, StairsX, StairsY)); }
		sprintf(OverheadInfoBuf,
			"%sÿc3------ÿc1v%0d.%dÿc6-ÿc1byÿc6-ÿc1syadastiÿc3------",
			OverheadInfoBuf, VERSION_MAJOR, VERSION_MINOR);
		OverheadPkt(OverheadInfoBuf); } }

BOOL PRIVATE OnCommandQuitGame(char **argv, int argc) {
	dprintf(DEBUG_ALL, "mephbot quitgame invoked");
	ExitGame();
	return TRUE; }

BOOL PRIVATE OnCommandDebug(char **argv, int argc) {
	fDebug = 1 - fDebug;
	server->GamePrintInfo(fDebug ? "mephbot: Debugging enabled" : "mephbot: Debugging disabled");
	dprintf(DEBUG_ALL, fDebug ? "Debugging enabled" : "Debugging disabled");
	return TRUE; }

//////////////////////////////////////////////////////////////////////////////
//
// Functions called by D2Hackit
//
//////////////////////////////////////////////////////////////////////////////

BOOL EXPORT OnClientStart() {
	char TempFname[_MAX_FNAME];
	InitializeCriticalSection(&MainMutex);
	ReadConfigInt("Debug", fDebug);
	ReadConfigInt("DebugToScreen", fDebugToScreen);
	ReadConfigInt("DebugLevel", DebugLevel);
	ReadConfigString("LogFileName", TempFname);
	ReadConfigString("MephbotDir", szMephbotDir);
	sprintf(szLogFileName, "%s%s%s", szMephbotDir, strlen(szMephbotDir) ? "\\" : "", TempFname);
	LoadConfig();
	dprintf(DEBUG_ALL, "Mephbot version %d.%d started", VERSION_MAJOR, VERSION_MINOR);
	dprintf(DEBUG_ALL, "Built on %s at %s", __DATE__, __TIME__);
	LoadStats();
	RemoveFlagFile(szInGameFlagFileName);
	fModuleActive = TRUE;
	fJustLoaded = TRUE;
	fAlreadySwitchedWeapon = FALSE;
	Tick1 = dwTickGame = dwTickStairs = 0;
	dprintf(DEBUG_ALL, "Starting keystrokes, mouseclicks, and packet-sending thread");
	// Start thread
	tdAutomation.Active = TRUE;
	tdUtil.Active = TRUE;
	mq = MessageQueue;
	//tdAutomation.mq = MessageQueue;			// FIXME: why did i do this?
	tdAutomation.hD2Window = hD2Window;
	tdUtil.hD2Window = hD2Window;
	DWORD dummy = 0;
    if (!(tdAutomation.Thread = CreateThread(NULL, 0, AutomationThreadProc, &tdAutomation, 0, &dummy))) {
		dprintf(DEBUG_ALL, "Automation Thread creation failed, error code %d", GetLastError()); }
	else {
		dprintf(DEBUG_ALL, "Automation Thread started successfully, tdAutomation.Thread = %X", tdAutomation.Thread); }
	dummy = 0;
    if (!(tdUtil.Thread = CreateThread(NULL, 0, UtilThreadProc, &tdUtil, 0, &dummy))) {
		dprintf(DEBUG_ALL, "Utility Thread creation failed, error code %d", GetLastError()); }
	else {
		dprintf(DEBUG_ALL, "Utility Thread started successfully, tdUtil.Thread = %X", tdUtil.Thread); }
	ChangeState(STATE_START_NEW_GAME);
	return TRUE; }

BOOL EXPORT OnClientStop() {
	dprintf(DEBUG_ALL, "Module exiting in OnClientStop()");
	RemoveFlagFile(szInGameFlagFileName);
	// kill keys & clicks thread
	tdAutomation.Active = FALSE;
	tdUtil.Active = FALSE;
	LogInfo(szInfoBuf, dwTickGlobal, dwTickGame, NumRestarts, NumAborts, NumRuns, NumKills, NumDeaths, NumChickens);
	// doing WriteStats() here sometimes causes an access violation in ntdll:strtoul(),
	// but this seems to only happen when the user exits mephbot with "unload mephbot"
	WriteStats();
	fModuleActive = FALSE;
	fInGame = FALSE;
	fclose(hLogFile);
	DeleteCriticalSection(&MainMutex);
	return TRUE; }
			
VOID EXPORT OnGameJoin(THISGAMESTRUCT *aThisgame) {
	dprintf(DEBUG_ALL, "-------------------------GAME-JOINED-----------------------------");
	dprintf(DEBUG_MISC, "Joined game");
	thisgame = aThisgame;
	tdAutomation.thisgame = thisgame;
	fInGame = TRUE;
	Tick1 = dwTickGame = dwTickStairs = dwTickHealingPotion = 0;
	StairsX = StairsY = 0;
	wMephX = wMephY = 0;
	iMephHealth = 100;							// he starts out at 100%
	bOrmusPathIndex = bHratliPathIndex = bStashPathIndex = bMephPathIndex = 0;
	CurrentLevel = 0;
	cMephMissing = 0;
	InventoryGold = 0;
	fAlreadyBoughtPotions = FALSE;
	fNeedToBuyPotions = TRUE;
	BestPotionID = 0;
	BestPotionType = 0;
	fAlreadyDepositing = FALSE;
	fAlreadyRecoveredMana = FALSE;
	fDrinkHealingPotion = FALSE;
	fCubeOpen = FALSE;
	fStashOpen = FALSE;
	fMoveItems = FALSE;
	cNumPotionsInBelt = 0;
	ProtectionSkillIndex = 0;
	cNoTeleportProgress = 0;
	dwCorpseId = 0;

	// clean up linked lists
	EnterCriticalSection(&AutomationMutex);
	PURGELIST(MessageQueue, (MESSAGE *))
	LeaveCriticalSection(&AutomationMutex);
	EnterCriticalSection(&MainMutex);
	PURGELIST(ExcessPotions, (CDiabloItems *))
	PURGELIST(BeltPotions, (CDiabloItems *))
	LeaveCriticalSection(&MainMutex);
	EnterCriticalSection(&UtilMutex);
	PURGELIST(Movables, (CDiabloItems *))
	LeaveCriticalSection(&UtilMutex);

	ClearInventory();
	
	srand(time(NULL));
	RemoveFlagFile(szInvFullFlagFileName);
	// packet 0x04 takes us into STATE_GAME_STARTED
	return; }

VOID EXPORT OnGameLeave(THISGAMESTRUCT *aThisgame) {
	thisgame = NULL;
	tdAutomation.thisgame = NULL;
	fInGame = FALSE;
	fJustLoaded = FALSE;
	fDrinkHealingPotion = FALSE;
	dprintf(DEBUG_MISC, "Left game");
	RemoveFlagFile(szInGameFlagFileName);
	LogInfo(szInfoBuf, dwTickGlobal, dwTickGame, NumRestarts, NumAborts, NumRuns, NumKills, NumDeaths, NumChickens);
	WriteStats();
	dprintf(DEBUG_ALL, "-------------------------LEFT-GAME-------------------------------");

	// clean up linked lists
	EnterCriticalSection(&AutomationMutex);
	PURGELIST(MessageQueue, (MESSAGE *))
	LeaveCriticalSection(&AutomationMutex);
	EnterCriticalSection(&MainMutex);
	PURGELIST(ExcessPotions, (CDiabloItems *))
	PURGELIST(BeltPotions, (CDiabloItems *))
	LeaveCriticalSection(&MainMutex);
	EnterCriticalSection(&UtilMutex);
	PURGELIST(Movables, (CDiabloItems *))
	LeaveCriticalSection(&UtilMutex);

	ClearInventory();
	ChangeState(STATE_START_NEW_GAME);
	return; }

// is run approximately every tenth of a second
DWORD EXPORT OnGameTimerTick(VOID) {

	if (!fModuleActive) {
		Tick1 = 0;
		return TRUE; }	
	
	// This was an attempt to look at the main D2 window's message queue in order 
	// to be able to remove WM_KILLFOCUS messages
	//Peek();

	Tick1++;
	dwTickGlobal++;
	if (InGame()) {
		dwTickGame++; }

	if (InGame() && dwTickGame >= MaxTickGame && !(dwTickGame % 50)) {		// we're lost, bag it
		NumAborts++;
		dprintf(DEBUG_ALL, "ABORT: Game lasted longer than %d seconds, aborting", MaxTickGame / 10);
		ChangeState(STATE_EXIT_GAME); }

	if (InGame() &&
		dwTickGame >= DelayGameLoad / 2 &&
		state != STATE_EXIT_GAME &&
		state != STATE_START_ISSUED &&
		state != STATE_START_NEW_GAME) {
		CurrentLevel = GetCurrentLevel(); }

	if (InGame() && dwTickGame >= DelayGameLoad && !(dwTickGame % DelayOverheadUpdate)) {
		OverheadInfo(); }

	// keep switching weapons until it's right
	if (InGame() &&
		dwTickGame >= DelayGameLoad &&
		dwTickGame % 10 == 5 &&
		strlen(szWeaponPrimary) &&
		strlen(szWeaponCurrent)) {
		if (fWeaponDesired != fWeaponCurrent) {
			WeaponSwitch(); } }

	// drink potions that wind up in inventory/stash/cube rather than belt
	if (InGame() &&
		dwTickGame >= DelayGameLoad * 2 &&
		dwTickGame % 10 == 0 &&
		ExcessPotions->GetItemCount() > 0) {
		EnterCriticalSection(&MainMutex);
		dprintf(DEBUG_MISC, "%d excess potions in inventory, drinking one", ExcessPotions->GetItemCount());
		LinkedItem *linkeditem = ExcessPotions->GetFirstItem();
		CDiabloItems *item = (CDiabloItems *)linkeditem->lpData;
		if (item->m_bItemID) {
			DrinkStoredPotion(item->m_bItemID); }
		ExcessPotions->RemoveItem(linkeditem);
		LeaveCriticalSection(&MainMutex); }

	if (fDrinkHealingPotion) {
		if (dwTickHealingPotion++ > 50) {						// FIXME: make configurable
			dwTickHealingPotion = 0; } }

	if (InGame() && !InTown() && dwTickHealingPotion == 1 && fDrinkHealingPotion) {
		EnterCriticalSection(&MainMutex);
		if (!BeltPotions->GetItemCount()) {
			// This should never happen, unless:
			//	- player out of gold
			//	- belt filled with scrolls or other bunk potions
			//	- player with low Life gets seriously beat on
			dprintf(DEBUG_MISC, "Healing potion needed but none in belt!");
			fDrinkHealingPotion = FALSE;
			dwTickHealingPotion = 0; }
		else {
			dprintf(DEBUG_PLAYER_UPDATES, "Drinking healing potion");
			LinkedItem *linkeditem = BeltPotions->GetFirstItem();
			CDiabloItems *item = (CDiabloItems *)linkeditem->lpData;
			if (item->m_bItemID) {
				DrinkBeltPotion(item->m_bItemID); }
			BeltPotions->RemoveItem(linkeditem); }
		LeaveCriticalSection(&MainMutex); }


	switch (state) {

	// should never happen
	case STATE_UNKNOWN: {
		dprintf(DEBUG_ALL, "ABORT: Unknown state.  Disabling module");
		fModuleActive = FALSE;
		break; }
		
	case STATE_GAME_STARTED: {
		if (dwTickGame == DelayGameLoad && InGameReset()) {	// allow for game load and corpse pickup
			dprintf(DEBUG_MISC, "Starting up");
			dwTickWaypoint = 0;
			Tick1 = 0;
			SwitchWeapon(WEAPON_PRIMARY);
			if (CurrentLevel == 75) {						// Kurast Docks
				NumRuns++;
				bPointsIndex = 2;						// for path-finding in Durance 2
				SendD2HackitCommand("pickit deactivate");	// keep it off until Durance 3
				if (dwCorpseId) {
					dprintf(DEBUG_MISC, "Player corpse (ID %d) detected, snagging", dwCorpseId);
					Pause(500);
					RunToEntity(0, dwCorpseId);								// run to corpse
					InteractWithEntity(0, dwCorpseId);						// pick up corpse
					Pause(500); }
				dprintf(DEBUG_MISC, "Walking to Ormus");
				ChangeState(STATE_WALK_TO_ORMUS); }
			else {
				// This will be the place to use Gayak's fastwp
				NumAborts++;
				dprintf(DEBUG_ALL, "ABORT: Game not started in Act 3.  Please go to act 3 and restart.");
				// This will stop the bot immediately
				fWrongAct = TRUE;
				CreateFlagFile(szWrongActFlagFileName);
				ChangeState(STATE_EXIT_GAME); } }
		break; }
		
	case STATE_WALK_TO_ORMUS: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == DelayStep) {
				TakeStep(PathToOrmus[bOrmusPathIndex][0], PathToOrmus[bOrmusPathIndex][1]);
				Tick1 = 0;
				if (++bOrmusPathIndex >= ARRAY_SIZE(PathToOrmus)) {
					if (fRepairGame) {
						ChangeState(STATE_WALK_TO_HRATLI); }
					else {
						ChangeState(STATE_OPENING_STORE); } } } }
		break; }
	
	case STATE_OPENING_STORE: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == 1) {
				dprintf(DEBUG_MISC, "Opening Store with Ormus");
				OpenStore(dwOrmusId, wOrmusX, wOrmusY); }
			else if (Tick1 == DelayStep) {
				dprintf(DEBUG_MISC, "Walking closer to Ormus");
				TakeStep(wOrmusX, wOrmusY);
				Tick1 = 0; } }
			// we change state in response to 0x9c0b item drop to store packets
		break; }
		
	case STATE_STORE_OPEN: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == 10) {					// FIXME (increased from 3 to 10 for 0.9)
				if (fNeedToBuyPotions && !fAlreadyBoughtPotions && BestPotionID) {
					BuyItemFillup(BestPotionID, dwOrmusId);
					fAlreadyBoughtPotions = TRUE;
					fNeedToBuyPotions = FALSE; }
				Tick1 = 0;
				ChangeState(STATE_CLOSING_STORE); } }
		break; }
		
	case STATE_CLOSING_STORE: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == DelayStep) {
				dprintf(DEBUG_MISC, "Closing store with Ormus");
				CloseStore(dwOrmusId);
				SendString("{SPACE 5}");		// make SURE we closed store with Ormus
				dprintf(DEBUG_MISC, "Walking to stash");
				SendString("{NUMPADSUB}");		// reveal map/get stair position
				//DrawAutomapAct();				// still flaky!  causes crashes
				Tick1 = 0;
				ChangeState(STATE_WALK_TO_STASH); } }
		break; }
		
	case STATE_WALK_TO_STASH: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == DelayStep) {
				TakeStep(PathToStash[bStashPathIndex][0], PathToStash[bStashPathIndex][1]);
				Tick1 = 0;
				if (++bStashPathIndex >= ARRAY_SIZE(PathToStash)) {
					if (!(NumRuns % StashVisitGameNumber)) {
						dprintf(DEBUG_MISC, "Visiting stash");
						// make sure Ormus talk dialog is closed
						SendString("{SPACE 5}");
						ChangeState(STATE_OPEN_STASH); }
					else {
						dprintf(DEBUG_MISC, "Skipping stash visit");
						ChangeState(STATE_WALK_TO_WAYPOINT); } } } }
		break; }

	case STATE_OPEN_STASH: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == DelayStep)	{
				// make bloody well sure Ormus talk dialogue is closed
				SendString("{SPACE 5}");
				// we'd be much smarter if we detected this ID dynamically
				// however, it seems to always be 0x1d if we start game in Act 3
				dprintf(DEBUG_MISC, "Opening stash");
				OpenStash(0x1d); }					// FIXME
			// we exit this state when we get a 0x7710 stash open packet
			else if (Tick1 == DelayStep * 3) {
				dprintf(DEBUG_MISC, "Still waiting for stash to open, walking closer");
				TakeStep(PathToStash[ARRAY_SIZE(PathToStash) - 1][0], PathToStash[ARRAY_SIZE(PathToStash) - 1][1]);
				Tick1 = 0; } }			// loop until we get 0x7710 stash open
		break; }
	
	case STATE_CLOSE_STASH: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (Tick1 == 10) {						// FIXME
				// Move everything unid-ed to stash and cube
				// pickit MUST be disabled when we do this
				fMoveItems = TRUE; }				// FIXME: we should signal differently
			else if (Tick1 == 40) {					// FIXME
				// but how long do we wait for MoveItems() to finish?
				// 4 secs should be plenty... Mephisto can't drop so many items
				// at once that we'd need more time than this.
				dprintf(DEBUG_MISC, "Closing Stash");
				CloseStash();
				// we'll change state on 0x770c packet received
				Tick1 = 30;	} }						// skip mover commands on looping
		break; }

	case STATE_WALK_TO_WAYPOINT: {
		fMoveItems = FALSE;
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (CurrentLevel == 101) {				// Durance 2
				dprintf(DEBUG_MISC, "Arrived in Durance 2");
				Tick1 = 0;
				nextstate = STATE_FIND_STAIRS;
				ChangeState(STATE_CAST_PROTECTION); }
			else if (Tick1 == DelayStep) {
				dprintf(DEBUG_MISC, "Walking to waypoint");
				// we should be using the WpKurastDocksX and WpKurastDocksY from the waypoint entity info packet
				TakeStep(0x1426, 0x13ba);			// FIXME: Ugly constants
				Tick1 = 0;
				ChangeState(STATE_TAKE_WAYPOINT); } }
		break; }

	case STATE_TAKE_WAYPOINT: {
		if (InGameReset() && !TimedOutGettingToWaypoint()) {
			if (CurrentLevel == 101) {				// Durance 2
				dprintf(DEBUG_MISC, "Arrived in Durance 2");
				Tick1 = 0;
				nextstate = STATE_FIND_STAIRS;
				ChangeState(STATE_CAST_PROTECTION); }
			else if (Tick1 == DelayStep) {
				dprintf(DEBUG_MISC, "Taking waypoint to Durance 2");
				TakeWayPoint(WpIdKurastDocks, WAYPOINT_ACT3_9); }
			else if (Tick1 == DelayStep * 2) {
				dprintf(DEBUG_MISC, "Waypoint not taken, trying again");
				Tick1 = 0;
				ChangeState(STATE_WALK_TO_WAYPOINT); } }
		break; }

	case STATE_CAST_PROTECTION: {
		if (InGameReset()) {
			if (Tick1 == DelayBeforePrecast + QUICKSPELLDELAY) {
				if (ProtectionSkillIndex == 0) {
					dprintf(DEBUG_MISC, "Casting Protection Skills"); }
				if (strlen(ProtectionSkill[ProtectionSkillIndex]) > 2) {
					SelectSkillByName(ProtectionSkill[ProtectionSkillIndex], SKILL_RIGHT); }
				else {
					ProtectionSkillIndex = 0;
					Tick1 = 0;
					ChangeState(nextstate); } }
			else if (Tick1 == DelayBeforePrecast + QUICKSPELLDELAY * 2) {
				if (CurrentSkill[SKILL_RIGHT] != Skills[GetSkillIndexByName(ProtectionSkill[ProtectionSkillIndex])].Id) {
					Tick1 = DelayBeforePrecast + QUICKSPELLDELAY + 1; }
				else {
					if (ProtectionSkillIndex == 1 && nextstate == STATE_FIND_STAIRS) {
						// we will have gotten a 0x95 update with close to max mana after
						// first spell is cast -- won't work if only using 1 protection
						MaxMP = CurrentMP; }
					CastSkillOnSelfByName(ProtectionSkill[ProtectionSkillIndex]);
					if (++ProtectionSkillIndex > 7) {
						ProtectionSkillIndex = 0;
						Tick1 = 0;
						ChangeState(nextstate); }
					else {
						Tick1 = DelayBeforePrecast - 1; } } } }
		break; }

	case STATE_FIND_STAIRS: {
		if (InGameReset()) {
			if (dwTickStairs++ > 100) {
				dprintf(DEBUG_ALL, "ABORT: Maphack can't find the stairs down");
				NumAborts++;
				ChangeState(STATE_EXIT_GAME); }
			else if (Tick1 == QUICKSPELLDELAY && !fFindingPathToStairs) {
				if (StairsX && StairsY) {
					dprintf(DEBUG_MISC, "Maphack located the stairs down");
					fFindingPathToStairs = TRUE;	// make sure we don't double up these calls
					FindStairsPath();				// big mojo
					dprintf(DEBUG_MISC, "Dijkstra() built a path to the stairs");
					oldpx = PLAYERX; oldpy = PLAYERY;
					MaxHP = CurrentHP;
					dprintf(DEBUG_PLAYER_UPDATES, "Updated MaxHP (%d), MaxMP (%d)", MaxHP, MaxMP);
					SendString("{SPACE 2}");		// close waypoint menu if open
					dwTickStairs = 0;
					bStairsPathIndex = 0;			// 0 is player
					Tick1 = 0;
					fFindingPathToStairs = FALSE;
					ChangeState(STATE_TELEPORT_TOWARD_STAIRS); }
				else {
					Tick1 = 0; } } }
		break; }
		
	case STATE_TELEPORT_TOWARD_STAIRS: {
		if (InGameReset() && !TimedOutReachingStairs()) {
			if (Tick1 == QUICKSPELLDELAY) {
				if (Dist(PLAYERX, PLAYERY, StairsX, StairsY) <= DeltaTeleport) {
					dprintf(DEBUG_MISC, "We have arrived next to the stairs");
					dprintf(DEBUG_MISC, "StairsXY = (%.4X, %.4X)", StairsX, StairsY);
					TeleportTo(StairsX, StairsY);
					Tick1 = 0;
					ChangeState(STATE_TELEPORT_TO_STAIRS); }
				else if (Dist(PLAYERX, PLAYERY, StairsPath[bStairsPathIndex][0], StairsPath[bStairsPathIndex][1]) <= DeltaTeleport) {
					dprintf(DEBUG_PLAYER_MOVE, "Close to hop #%d, going straight there", bStairsPathIndex);
					Tick1 = (QUICKSPELLDELAY * 2) - 1; }
				else if (Dist(PLAYERX, PLAYERY, oldpx, oldpy) < 5) {
					dprintf(DEBUG_PLAYER_MOVE, "Poor progress, counter = %d", cNoTeleportProgress);
					if (++cNoTeleportProgress == 1) {
						TeleportToward(StairsX, StairsY, DeltaTeleport); }
					else if (cNoTeleportProgress == 2) {
						cNoTeleportProgress = 0;
						TeleportRandomly(); }
					Tick1 = 0; }
				else {
					cNoTeleportProgress = 0;
					Tick1 = 0;
					dprintf(DEBUG_PLAYER_MOVE, "Teleporting toward hop #%d", bStairsPathIndex);
					TeleportToward(StairsPath[bStairsPathIndex][0], StairsPath[bStairsPathIndex][1], DeltaTeleport / 2/*((rand() % 2) + 2)*/); } }
			if (Tick1 == QUICKSPELLDELAY * 2) {
				if (Dist(PLAYERX, PLAYERY, StairsPath[bStairsPathIndex][0], StairsPath[bStairsPathIndex][1]) < 3) {
					Tick1 = QUICKSPELLDELAY - 1; }
				else {
					Tick1 = 0;
					dprintf(DEBUG_PLAYER_MOVE, "Taking hop #%d to stairs, delay=%d, Tick1=%d", bStairsPathIndex, QUICKSPELLDELAY, Tick1);
					TeleportTo(StairsPath[bStairsPathIndex][0], StairsPath[bStairsPathIndex][1]); }
				if (++bStairsPathIndex == HopsToStairs) {
					ChangeState(STATE_TELEPORT_TO_STAIRS); } } }
		break; }
		
	case STATE_TELEPORT_TO_STAIRS: {
		// this state takes us from wherever the previous state ended, directly to the 
		// stairs to Durance 3.  It is intended to correct for undershoot in the previous
		// state, and might be removable if we could get the code above tighter
		if (InGameReset() && !TimedOutReachingStairs()) {
			if (Tick1 == QUICKSPELLDELAY || Tick1 == QUICKSPELLDELAY * 2) {
				dprintf(DEBUG_PLAYER_MOVE, "Teleporting to the stairs");
				TeleportToward(StairsX, StairsY, (int)Dist(PLAYERX, PLAYERY, StairsX, StairsY) / 2); }
			if (Tick1 == QUICKSPELLDELAY * 3) {
				dprintf(DEBUG_PLAYER_MOVE, "Walking to the stairs");
				TakeStep(StairsX, StairsY);
				Tick1 = 0;
				ChangeState(STATE_TAKE_STAIRS); } }
		break; }

	case STATE_TAKE_STAIRS: {
		if (InGameReset() && !TimedOutReachingStairs()) {
			if (CurrentLevel == 102) {					// Durance 3
				dprintf(DEBUG_MISC, "Arrived in Durance 3");
				SendD2HackitCommand("pickit activate");
				SelectSkillByName(PrimaryAttackSkill, PrimaryAttackHand);
				cNoTeleportProgress = 0;
				dprintf(DEBUG_PLAYER_MOVE, "Teleporting to Mephisto");
				nextstate = STATE_TELEPORT_TO_MEPH;
				Tick1 = 0;
				ChangeState(STATE_CAST_PROTECTION); }
			else if (Dist(PLAYERX, PLAYERY, StairsX, StairsY) > DeltaTeleport) {
				dprintf(DEBUG_MISC, "Too far from stairs");
				Tick1 = 0;
				ChangeState(STATE_TELEPORT_TO_STAIRS); }
			else if (Tick1 == DelayStep) {
				// this will repeat until arrival in Durance 3
				// aha!  we get it from a 0x09 packet!  Thanks, Gayak!
				TakeStairs(dwStairsObjID); }
			else if (Tick1 == DelayStep * 2) {
				dprintf(DEBUG_MISC, "Walking to the stairs");
				TakeStep(StairsX, StairsY);
				Tick1 = 0; } }
		break; }
		
	case STATE_TELEPORT_TO_MEPH: {
		if (InGameReset()) {
			if (CurrentLevel != 102) {			// band-aid for semi-broken algorithm above
				dprintf(DEBUG_ALL, "WARNING: Bug in STATE_TAKE_STAIRS has us thinking we're in Durance 3 when we're not.");
				Tick1 = 0;
				ChangeState(STATE_TAKE_STAIRS); }
			if (!fAlreadyRecoveredMana && Tick1 == 1 && MaxMP && (CurrentMP * 100 / MaxMP) < MinManaPercentBeforeAttackRun) {
				dprintf(DEBUG_PLAYER_UPDATES, "Mana < %d%%, waiting for mana recovery", MinManaPercentBeforeAttackRun);
				nextstate = state;
				ChangeState(STATE_RECOVER_MANA); }		// wait for mana recovery
			if (Tick1 == QUICKSPELLDELAY) {
				TeleportTo(PathToMeph[bMephPathIndex][0], PathToMeph[bMephPathIndex][1]);
				Tick1 = 0;
				if (++bMephPathIndex >= ARRAY_SIZE(PathToMeph)) {
					SendString("{SPACE 2}");		// close windows, hide automap
					ChangeState(STATE_ATTACK_MEPH); } } }
		break; }
		
	case STATE_RECOVER_MANA: {
		if (InGameReset()) {
			// we'll just loop and doing nothing until we're up to the right mana level
			if (MaxMP && (CurrentMP * 100 / MaxMP) >= MinManaPercentBeforeAttackRun) {
				dprintf(DEBUG_PLAYER_UPDATES, "Mana recovered to %d%%", MinManaPercentBeforeAttackRun);
				Tick1 = 0;
				fAlreadyRecoveredMana = TRUE;
				ChangeState(nextstate); } }
		break; }

	case STATE_ATTACK_MEPH: {
		if (InGameReset()) {
			if ((!wMephX || !wMephY) && Tick1 == QUICKSPELLDELAY && cMephMissing < 10) {
				cMephMissing++;
				dprintf(DEBUG_ALL, "Don't know where Mephisto is!  Trying harder...");
				TeleportToward(SafePlaceX, SafePlaceY, DeltaTeleport);
				Tick1 = 0; }
			else if ((!wMephX || !wMephY) && Tick1 == QUICKSPELLDELAY && cMephMissing >= 10) {	// FIXME
				dprintf(DEBUG_ALL, "ABORT: Don't know where Mephisto is!  Bailing...");
				Tick1 = DelayExitGame;
				NumAborts++;
				ChangeState(STATE_EXIT_GAME); }
			// We attack Mephisto until we receive a corpse packet for him, we die,
			// we exit the game (via 0x95 chicken code), or we abort
			// because MaxTickGame has been exceeded.
			//
			// Therefore, we don't change state here, we do it in OnGamePacketBeforeReceived
			else if (fDodging && !(Tick1 % QUICKSPELLDELAY) && fTelesafeAfterWait) {
				Tick1 = 0;
				Telesafe();	}
			else if (Tick1 == QUICKSPELLDELAY) {
				dprintf(DEBUG_MISC, "Attacking Mephisto at (%.4X, %.4X)", wMephX, wMephY);
				CastSkillOnLocationByName(PrimaryAttackSkill, PrimaryAttackHand, wMephX, wMephY); }
			else if (!(Tick1 % QUICKSPELLDELAY) && Tick1 <= SkillTimerTicks(PrimaryAttackSkill)) {
				// if Mephisto's got less than 50% of his max health, static field does nothing
				CastSkillOnLocationByName(iMephHealth > MephMinHealthToCastStatic ? "Static Field" : SecondaryAttackSkill, SecondaryAttackHand, wMephX, wMephY); }
			else if (Tick1 == SkillTimerTicks(PrimaryAttackSkill) + QUICKSPELLDELAY) {
				int DistResult = RangeToTarget(wMephX, wMephY);
				// we might want something here that says if range > ~ 100, bail
				// try to avoid attracting Maffer and Wyand
				if (Dist(PLAYERX, PLAYERY, SafePlaceX, SafePlaceY) > DeltaTeleport) {
					dprintf(DEBUG_MISC, "Too close to Wyand/Maffer, teleporting to safety");
					TeleportToward(SafePlaceX, SafePlaceY, DeltaTeleport);
					Tick1 = 0; }
				else if (DistResult != DISTANCE_IN_RANGE) {
					dprintf(DEBUG_MISC, "Mephisto too %s.  Teleporting %s",
						DistResult == DISTANCE_TOO_FAR ? "far" : "close",
						DistResult == DISTANCE_TOO_FAR ? "closer" : "away");
					TeleportAway(DistResult, DeltaTeleportAway, wMephX, wMephY);
					Tick1 = 0; }
				else {
					Tick1 = QUICKSPELLDELAY - 1; } } }
		break; }
		
	case STATE_MEPH_DEAD: {
		if (InGameReset()) {
			if (Tick1 == 1) {
				SwitchWeapon(WEAPON_PRIMARY); }
			else if (Tick1 == QUICKSPELLDELAY) {
				NumKills++;
				dprintf(DEBUG_MISC, "Mephisto is dead, teleporting to corpse");
				TeleportTo(wMephX, wMephY);				
				dprintf(DEBUG_MISC, "Waiting for item pickup"); }
			else if (Tick1 == QUICKSPELLDELAY + DelayPickup) {
				Tick1 = 0;
				ChangeState(STATE_EXIT_GAME); } }
		break; }
		
	case STATE_PLAYER_DEAD: {
		// oops... should never happen with 0x95 packet chicken code, but maybe
		// the player comes at Mephisto cursed, or Maffer shows up MSLE or something
		if (InGameReset()) {
			if (Tick1 >= DelayAfterDeath) {
				dprintf(DEBUG_MISC, "You seem to have died (oops).  We'll try to recover");
				NumDeaths++;
				Pause(dClick);
				ResurrectPlayer();
				Tick1 = 0;
				ChangeState(STATE_EXIT_GAME); } }
		break; }

	case STATE_EXIT_GAME: {
		if (InGameReset()) {
			if (Tick1 >= DelayExitGame) {
				LogInfo(szInfoBuf, dwTickGlobal, dwTickGame, NumRestarts, NumAborts, NumRuns, NumKills, NumDeaths, NumChickens);
				ExitGame();
				Tick1 = 0;
				ChangeState(STATE_START_NEW_GAME); } }
		else {
			Tick1 = 0;
			ChangeState(STATE_START_NEW_GAME); }
		break; }

	case STATE_START_NEW_GAME: {
		// start new game
		if (Tick1 > DelayStartNewGame && InGame()) {
			Tick1 = 0;
			ChangeState(STATE_EXIT_GAME); }
		if (Tick1 == DelayCreateGame - 1) {
			/*if (GameSocketDetected()) {*/
			if (thisgame || (thisgame && thisgame->player)) {
				dprintf(DEBUG_ALL, "WARNING: We're in STATE_START_NEW_GAME but still in game, re-exiting");
				Tick1 = 0;
				ChangeState(STATE_EXIT_GAME); } }
		if (Tick1 == DelayCreateGame) {
			srand(time(NULL));
			if (fInventoryFull) {
				dprintf(DEBUG_ALL, "Inventory full.  Mephbot exiting");
				CreateFlagFile(szInvFullFlagFileName);
				Tick1 = 0; }
			else if (fWrongAct) {
				dprintf(DEBUG_ALL, "ABORT: Game started in wrong act.  Please create game in act 3 and restart.  Mephbot exiting");
				CreateFlagFile(szWrongActFlagFileName);
				Tick1 = 0; }
			else {
				char szTextBuf[1024];
				if (fJustLoaded) {
					dprintf(DEBUG_MISC, "Performing chat actions");
					sprintf(szTextBuf, "{SLASH}join %s{ENTER}", RandString(8 + (rand() % 9)));
					SendString(szTextBuf);
					sprintf(szTextBuf, "{SLASH}away %s{ENTER}",
						strlen(szAwayMessage) ? szAwayMessage : RandString(8 + (rand() % 9)));
					SendString(szTextBuf); }
				dprintf(DEBUG_MISC, "Creating new game");
				for (int i = 0; i < 4; i++) {
					LeftClick(645, 460);						// Create game button
					SendString("{ESC}"); }
				LeftClick(645, 460);
				sprintf(szTextBuf, "%s%s{TAB}%s", szGameNamePrefix,
					RandString(5 + (rand() % 5)), RandString(8 + (rand() % 5)));
				SendString(szTextBuf);
				Pause(dClick);
				SendString("{ENTER}");
				Tick1 = 0;
				ChangeState(STATE_START_ISSUED); } }
		break; }
		
	case STATE_START_ISSUED: {
		if (Tick1 > MaxTickRestart && !InGame()) {
			dprintf(DEBUG_ALL, "WARNING: Game not started within configured interval, trying again");
			Tick1 = 0;
			ChangeState(STATE_START_NEW_GAME); }
		// we're looking for a 0x04 packet to change state
		break; }

	case STATE_WALK_TO_HRATLI: {
		if (InGameReset()) {
			if (Tick1 == DelayStep) {
				TakeStep(PathToHratli[bHratliPathIndex][0], PathToHratli[bHratliPathIndex][1]);
				Tick1 = 0;
				if (++bHratliPathIndex >= ARRAY_SIZE(PathToHratli)) {
					dprintf(DEBUG_MISC, "Opening Store with Hratli");
					ChangeState(STATE_REPAIR); } } }
		break; }
	
	case STATE_REPAIR: {
		if (InGameReset()) {
			if (dwTickGame > MaxTickGame) {
				Tick1 = 0;
				dprintf(DEBUG_ALL, "ABORT: Couldn't repair, gave up");
				NumAborts++;
				ChangeState(STATE_EXIT_GAME); }
			else if (Tick1 == DelayStep && dwHratliId) {
				OpenStore(dwHratliId, wHratliX, wHratliY); }
			else if (Tick1 == DelayStep * 2 && dwHratliId) {
				dprintf(DEBUG_MISC, "Walking closer to Hratli");
				TakeStep(wHratliX, wHratliY);
				Tick1 = 0; } }
		// we change state in response to 0x9c0b item drop to store packets
		break; }
		
	default: {
		dprintf(DEBUG_ALL, "ERROR: Entering STATE_UNKNOWN... Something strange is happening!");
		ChangeState(STATE_UNKNOWN);
		break; }
	}
	
	return TRUE; }


DWORD EXPORT OnGamePacketBeforeReceived(BYTE *aPacket, DWORD aLen) {

	if (DebugLevel & DEBUG_PACKETS) {
		DumpPacket(aPacket, aLen, 1); }

	if (!fModuleActive || !InGame()) {
		return aLen; }

	if (aPacket[0] == 0x04) {							// Act Change/Game Ready
		if (state == STATE_START_ISSUED) {
			dprintf(DEBUG_MISC, "Act changed/game ready");
			CreateFlagFile(szInGameFlagFileName);
			Tick1 = dwTickGame = 0;
			ChangeState(STATE_GAME_STARTED); }
		return aLen; }

/*
when i get a 0x09 it contains stair obj ID if the warp number is 67 or 68

						##
Act 3 Mephisto Down L	67
Act 3 Mephisto Down R	68

warp- WarpID----- ## XXXXX YYYYY
09 05 01 00 00 00 44 f2 44 6f 19 
*/
	if (aPacket[0] == 0x09 && aPacket[1] == 0x05) {						// Stairs/warp info
		if (aPacket[6] == 67 || aPacket[6] == 68) {						// Stairs down
			memcpy(&dwStairsObjID, &aPacket[2], 4);
			dprintf(DEBUG_MISC, "Got stairs object ID (%d)", dwStairsObjID); }
		return aLen; }

	if (aPacket[0] == 0x0c && aPacket[1] == 0x01) {					// unit health update
		DWORD monID = 0;
		memcpy(&monID, &aPacket[2], 4);
		if (dwMephId && monID == dwMephId) {							// Mephisto
			if (aPacket[7] == 0x00) {
				iMephHealth = (int)aPacket[8] * 100 / 0x80;				// percentage
				dprintf(DEBUG_UNIT_UPDATES, "Mephisto (MonID=%X) update Health=%d%%", monID, iMephHealth); }
			// Mephisto's getting weak, bring out the Alibabas and IST-IST-IST shield
			if (state == STATE_ATTACK_MEPH && fWeaponSwitch &&
				iMephHealth < MephWeaponSwitchHealth && fWeaponCurrent == WEAPON_PRIMARY && !fAlreadySwitchedWeapon) {
				dprintf(DEBUG_MISC, "Mephisto's getting weak, switching weapons");
				SwitchWeapon(WEAPON_SECONDARY);
				fAlreadySwitchedWeapon = TRUE; } }
		return aLen; }

	if (aPacket[0] == 0x12 && aPacket[1] == 0x00 &&
		aPacket[2] == 0x01 && aPacket[3] == 0x00 && aPacket[4] == 0x00 && aPacket[5] == 0x00 &&
		aPacket[14] == 0x00 && aPacket[15] == 0x00) {	// player death
		dprintf(DEBUG_ALL, "Player death packet received (0x12)");
		Tick1 = 0;
		ChangeState(STATE_PLAYER_DEAD);
		return aLen; }

	// some fruitful packet sniffing and analysis on 5 Aug 2002 yielded the solution
	// to all the gold inventory questions
	if (aPacket[0] == 0x1d && aPacket[1] == 0x0e) {		// inventory gold BYTE update
		fGoldQuantityKnown = TRUE;
		BYTE gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(BYTE));
		InventoryGold = (DWORD)gTmp;
		dprintf(DEBUG_MISC, "Got a 0x1d0e gold update: I'm carrying %d gold now.", InventoryGold);
		return aLen; }

	if (aPacket[0] == 0x1e && aPacket[1] == 0x0e) {		// inventory gold WORD update
		fGoldQuantityKnown = TRUE;
		WORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(WORD));
		InventoryGold = (DWORD)gTmp;
		dprintf(DEBUG_MISC, "Got a 0x1e0e gold update: I'm carrying %d gold now.", InventoryGold);
		return aLen; }

	if (aPacket[0] == 0x1f && aPacket[1] == 0x0e) {		// inventory gold DWORD update
		fGoldQuantityKnown = TRUE;
		DWORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(DWORD));
		InventoryGold = (DWORD)gTmp;
		dprintf(DEBUG_MISC, "Got a 0x1f0e gold update: I'm carrying %d gold now.", InventoryGold);
		return aLen; }

	if (aPacket[0] == 0x1d && aPacket[1] == 0x0f) {		// stash gold BYTE update
		fGoldQuantityKnown = TRUE;
		BYTE gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(BYTE));
		StashGold = (DWORD)gTmp;
		dprintf(DEBUG_MISC, "Got a 0x1d0f gold update: I have %d gold in stash now.", StashGold);
		return aLen; }

	if (aPacket[0] == 0x1e && aPacket[1] == 0x0f) {		// stash gold WORD update
		fGoldQuantityKnown = TRUE;
		WORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(WORD));
		StashGold = (DWORD)gTmp;
		dprintf(DEBUG_MISC, "Got a 0x1e0f gold update: I have %d gold in stash now.", StashGold);
		return aLen; }

	if (aPacket[0] == 0x1f && aPacket[1] == 0x0f) {		// stash gold DWORD update
		fGoldQuantityKnown = TRUE;
		DWORD gTmp;
		memcpy(&gTmp, &aPacket[2], sizeof(DWORD));
		StashGold = (DWORD)gTmp;
		dprintf(DEBUG_MISC, "Got a 0x1f0f gold update: I have %d gold in stash now.", StashGold);
		return aLen; }
	// end gold updates

	if (aPacket[0] == 0x23)	{							// skill active (left/right)
		int hand = (int)aPacket[6];
		CurrentSkill[hand] = aPacket[7] + (aPacket[8] << 8);
		dprintf(DEBUG_SKILLS, "Server says we have skill %s active now on %s-click",
			GetSkillNameById(CurrentSkill[hand]), CurrentSkill[hand] == SKILL_LEFT ? "left" : "right");
		return aLen; }

	// from repair.d2h by Gayak
	if (aPacket[0] == 0x2a && aPacket[1] == 0x01) {
		dprintf(DEBUG_MISC, "Equipment repaired");
		fRepairGame = FALSE;
		return aLen; }

	// check snarfed from pickit.d2h 0.84 by Gayak & ackmed
	if (aPacket[0] == 0x2c && aPacket[1] == 0x00) {
		if (aLen == 8 && aPacket[6] == 0x16) {
			dprintf(DEBUG_ALL, "Inventory Full!");
			if (fContinueWhenInventoryFull) {
				dprintf(DEBUG_ALL, "But we're going to keep running"); }
			else {
				dprintf(DEBUG_ALL, "Setting the inventory full flag");
				fInventoryFull = TRUE; } }
		return aLen; }

	if (fDodging) {
		// Contributed by Gayak <170@guyau.qc.ca>
		if (aPacket[0] == 0x4c && aPacket[1] == 0x01) {
			DWORD dwTemp;
			memcpy(&dwTemp, &aPacket[2], 4);
			if (dwTemp == dwMephId) {
				if (aPacket[8] == 0x14) {					// ice ball
					dprintf(DEBUG_MISC, "Mephisto is attacking with his ice ball, teleporting to safety.");
					fTelesafeAfterWait = TRUE; } }
			return aLen; }
		
		if (aPacket[0] == 0x6c) {
			DWORD dwTemp;
			memcpy(&dwTemp, &aPacket[1], 4);
			if (dwTemp == dwMephId) {
				dprintf(DEBUG_MISC, "Mephisto is attacking, teleporting to safety.");
				fTelesafeAfterWait = TRUE; }
			return aLen; }
		
		if (aPacket[0] == 0x15) {
			fTelesafeAfterWait = FALSE;
			return aLen; } }
	// End Gayak code

/*	
stairs down
                        ObjectID--- lid-- PosX- PosY-
03:47:15p  RECV:  51 02 01 00 00 00 7a 01 02 14 2d 14 00 00  (null)  00000z0000000
*/
	if (aPacket[0] == 0x51 && aPacket[1] == 0x02) {				// possible waypoint/stair info
		WORD ObjIdTemp;
		memcpy(&ObjIdTemp, &aPacket[6], 2);
		if (isWP(ObjIdTemp) && ObjIdTemp == 0x00ed) {			// Kurast Docks waypoint
			if (aPacket[13] == 0x00) {
				memcpy(&WpIdKurastDocks, &aPacket[2], 4);
				memcpy(&WpKurastDocksX, &aPacket[8], 2);				// PosX
				memcpy(&WpKurastDocksY, &aPacket[10], 2);				// PosY
				dprintf(DEBUG_MISC, "Got Kurast Docks waypoint ID (0x%X) at (%.4X, %.4X)", WpIdKurastDocks, WpKurastDocksX, WpKurastDocksY); } }
		if (isWP(ObjIdTemp) && ObjIdTemp == 0x0144) {			// Durance 2 Waypoint
			if (aPacket[13] == 0x00) {
				memcpy(&WpDurance2X, &aPacket[8], 2);					// PosX
				memcpy(&WpDurance2Y, &aPacket[10], 2);					// PosY
				dprintf(DEBUG_MISC, "Got Durance 2 waypoint location (%.4X, %.4X)", WpDurance2X, WpDurance2Y); } }
		return aLen; }
	
	if (aPacket[0] == 0x55) {									// NPC spawn
		DWORD npcID = 0;
		WORD tmpX = 0, tmpY = 0, check = 0;
		memcpy(&npcID, &aPacket[1], 4);
		memcpy(&check, &aPacket[5], 2);
		memcpy(&tmpX, &aPacket[7], 2);
		memcpy(&tmpY, &aPacket[9], 2);
		if (tmpX && tmpY) {										// no pos = 0
			if (check == 0x00ff) {								// Ormus
				dwOrmusId = npcID;
				wOrmusX = tmpX;
				wOrmusY = tmpY;
				dprintf(DEBUG_UNIT_MOVE, "Ormus (npcID=%X) spawned at (%.4X, %.4X)", dwOrmusId, wOrmusX, wOrmusY); }
			else if (check == 0x00fd) {							// Hratli
				dwHratliId = npcID;
				wHratliX = tmpX;
				wHratliY = tmpY;
				dprintf(DEBUG_UNIT_MOVE, "Hratli (npcID=%X) spawned at (%.4X, %.4X)", dwHratliId, wHratliX, wHratliY); } }
		return aLen; }

	if (aPacket[0] == 0x56) {										// monster spawn
		if (aPacket[5] == 0xf2 && aPacket[6] == 0x00) {				// Mephisto!!!
			memcpy(&dwMephId, &aPacket[1], 4);						// grab Monster ID
			memcpy(&wMephX, &aPacket[7], 2);							// PosX
			memcpy(&wMephY, &aPacket[9], 2);							// PosY
			dprintf(DEBUG_UNIT_MOVE, "Mephisto (monID=%X) spawned at (%.4X, %.4X)", dwMephId, wMephX, wMephY); }
		return aLen; }
	
	if ((aPacket[0] == 0x67 || aPacket[0] == 0x68 || aPacket[0] == 0x6d)) {	// Unit Move/status
		DWORD check1;
		WORD tmpX = 0, tmpY = 0;
		int p6d = (aPacket[0] == 0x6d);								// 0x6d is a bit different
		memcpy(&check1, &aPacket[1], 4);
		// eventually we'll be looking for Malah, Larzuk, oh boy...
		// i guess when we get to that point we should just track all NPCs
			memcpy(&tmpX, &aPacket[6-p6d], 2);						// Pos X
			memcpy(&tmpY, &aPacket[8-p6d], 2);						// Pos Y
		if (tmpX && tmpY) {
			if (dwOrmusId && check1 == dwOrmusId) {						// Ormus update
				wOrmusX = tmpX;
				wOrmusY = tmpY;
				dprintf(DEBUG_UNIT_MOVE, "Ormus (npcID=%X) moved to (%.4X, %.4X)", dwOrmusId, wOrmusX, wOrmusY); }
			else if (dwHratliId && check1 == dwHratliId) {				// Hratli update
				wHratliX = tmpX;
				wHratliY = tmpY;
				dprintf(DEBUG_UNIT_MOVE, "Hratli (npcID=%X) moved to (%.4X, %.4X)", dwHratliId, wHratliX, wHratliY); }
			else if (state != STATE_MEPH_DEAD && dwMephId && check1 == dwMephId) {	// Mephisto update
				wMephX = tmpX;
				wMephY = tmpY;
				// this is a yucky hack... our 0x69 detection really isn't working right
				if (state == STATE_MEPH_DEAD) {
					ChangeState(STATE_ATTACK_MEPH); }
				dprintf(DEBUG_UNIT_MOVE, "Mephisto (monID=%X) moved to (%.4X, %.4X)", dwMephId, wMephX, wMephY); } }
		return aLen; }
	
	if ((aPacket[0] == 0x69 && aPacket[5] == 0x09) && dwMephId) {		// corpse position
		DWORD monID = 0;
		memcpy(&monID, &aPacket[1], 4);								// snarf monsterID
		if (monID == dwMephId) {										// is it Mephisto?
			WORD tmpX = 0, tmpY = 0;
			memcpy(&tmpX, &aPacket[6], 2);
			memcpy(&tmpY, &aPacket[8], 2);
			if (tmpX && tmpY) {						// early 0x69s have null position field
				wMephX = tmpX;
				wMephY = tmpY;
				dprintf(DEBUG_MISC, "Mephisto (monID=%X) dead!  Corpse near (%.4X, %.4X)", dwMephId, wMephX, wMephY);
				if (state != STATE_PLAYER_DEAD) {
					Tick1 = 0;
					ChangeState(STATE_MEPH_DEAD); } } }
		return aLen; }

	if (aPacket[0] == 0x75 && InTown()) {							// player corpse
		memcpy(&dwCorpseId, &aPacket[1], sizeof(DWORD));
		return aLen; }

	if (aPacket[0] == 0x77 && aPacket[1] == 0x0c) {					// stash closed
		dprintf(DEBUG_MISC, "Stash Closed");
		fStashOpen = FALSE;
		if (state == STATE_CLOSE_STASH) {
			Tick1 = 0;
			ChangeState(STATE_WALK_TO_WAYPOINT); }
		return aLen; }

	if (aPacket[0] == 0x77 && aPacket[1] == 0x10) {					// stash open
		dprintf(DEBUG_MISC, "Stash opened");
		fStashOpen = TRUE;
		if (fCubeOpen) {
			dprintf(DEBUG_MISC, "Cube closed"); }
		fCubeOpen = FALSE;
		if (state == STATE_OPEN_STASH && !fAlreadyDepositing) {
			if (InventoryGold) {
				fAlreadyDepositing = TRUE;
				dprintf(DEBUG_MISC, "Depositing %d gold", InventoryGold);
				Pause(dClick);
				DepositGold(InventoryGold); }
			Tick1 = 0;
			ChangeState(STATE_CLOSE_STASH); }
		return aLen; }

	// FIXME: there is a 7711 we get as well when we open cube... why?
	// is this a signal that the stash is no longer available?
	if (aPacket[0] == 0x77 && aPacket[1] == 0x15) {					// cube opened
		dprintf(DEBUG_MISC, "Cube opened");
		fCubeOpen = TRUE;
		return aLen; }
	
	// from Infinite Pindlebot (pindlebot.d2h) by MozartMcLaus & FallNAngel
	// Originally from Jonathan Wu's Game Trigger (gt.d2h) (?)
	if (aPacket[0] == 0x95)	{							// player health/mana update
		CurrentHP = *((WORD *)(aPacket + 1)) & 0x0FFF;
		CurrentMP = (*((WORD *)(aPacket + 3)) & 0x0FFF) << 1;
		dprintf(DEBUG_PLAYER_UPDATES, "Life/Mana update (life=%d mana=%d)", CurrentHP, CurrentMP);
		if (!InTown() && CurrentHP == 0) {
			dprintf(DEBUG_ALL, "Player death packet (life=0) received");
			Tick1 = 0;
			ChangeState(STATE_PLAYER_DEAD);
			return aLen; }
		if (MaxHP && CurrentHP > MaxHP) {		// correct for incorrect settings
			MaxHP = CurrentHP; }				// which occasionally occur
		if (MaxMP && CurrentMP > MaxMP) {
			MaxMP = CurrentMP; }
		if (MaxHP != 0) {
			if (!InTown() && (MaxHP && (CurrentHP * 100 / MaxHP) < ChickenLifePercent)) {
				// kill game
				dprintf(DEBUG_ALL, "Life < %d%%, Chickening out", ChickenLifePercent);
				Chicken(); }
			else if (!InTown() && (CurrentHP * 100 / MaxHP) < HealingLifePercent) {
				fDrinkHealingPotion = TRUE; }
			else {
				fDrinkHealingPotion = FALSE;
				dwTickHealingPotion = 0; } }
		if (MaxMP != 0 && MaxHP !=0) {
			if (!InTown() &&
				((MaxHP && (CurrentHP * 100 / MaxHP) < ComboChickenLifePercent)) &&
				((MaxMP && (CurrentMP * 100 / MaxMP) < ComboChickenManaPercent))) {
				dprintf(DEBUG_ALL, "Life < %d%% and Mana < %d%%, Chickening out",
					ComboChickenLifePercent, ComboChickenManaPercent);
				Chicken(); } }
		return aLen; }
		
	if (aPacket[0] == 0x9c && aPacket[1] == 0x04) {
		CDiabloItems *item = parseItem(aPacket);
		char *itemcode = item->m_sCode.ToChar(0);
		// note any healing potions that somehow wind up in inventory rather than belt
		if (!strncmp(itemcode, "hp", 2)) {
			dprintf(DEBUG_MISC, "Extra %s potion in inventory, adding to list", itemcode);
			EnterCriticalSection(&MainMutex);
			ExcessPotions->AddItem(item);
			LeaveCriticalSection(&MainMutex); }
		// add to inventory
		else {
			EnterCriticalSection(&UtilMutex);
			AddItemToInventory(item);
			LeaveCriticalSection(&UtilMutex);
			fItemJustDroppedToStorage = TRUE; }
		delete itemcode;
		return aLen; }

	if (aPacket[0] == 0x9c && aPacket[1] == 0x0b) {		// item drop to store

		switch (state) {

		case STATE_OPENING_STORE: {
			dprintf(DEBUG_MISC, "Store opened with Ormus");
			Tick1 = 0;
			ChangeState(STATE_STORE_OPEN);
			break; }

/*		// Buy the first potion we see in the store
		case STATE_STORE_OPEN:
			if (!fAlreadyBoughtPotions && fNeedToBuyPotions) {
				CDiabloItems *item = parseItem(aPacket);
				char *itemname = item->m_sCode.ToChar(0);
				// Should work in norm or nightmare
				if (!strncmp(itemname, "hp", 2)) {
					dprintf(DEBUG_MISC, "Buying %s", itemname);
					BuyItemFillup(item->m_bItemID, dwOrmusId);
					fAlreadyBoughtPotions = TRUE;
					fNeedToBuyPotions = FALSE; }
				delete item; }
			break;*/

		// Select the best potion
		case STATE_STORE_OPEN: {
			CDiabloItems *item = parseItem(aPacket);
			char *itemname = item->m_sCode.ToChar(0);
			if (!strncmp(itemname, "hp", 2)) {
				if (itemname[2] > BestPotionType) {
					dprintf(DEBUG_MISC, "%s potion now ranked the best", itemname);
					BestPotionType = itemname[2];
					BestPotionID = item->m_bItemID; } }
			break; }

		case STATE_REPAIR: {
			dprintf(DEBUG_MISC, "Repaired equipment, restarting");
			Tick1 = 0;
			CloseStore(dwHratliId);
			SendString("{SPACE 5}");
			ChangeState(STATE_EXIT_GAME);
			break; }

		default: {
			break; }

			return aLen; } }

	if (aPacket[0] == 0x9c && aPacket[1] == 0x0e) {		// item drop to belt
		CDiabloItems *item = parseItem(aPacket);
		char *itemname = item->m_sCode.ToChar(0);
		if (!strncmp(itemname, "hp", 2)) {
			if (fNeedToBuyPotions && ++cNumPotionsInBelt >= 12 && state != STATE_STORE_OPEN) {
				dprintf(DEBUG_MISC, "12 or more potions in belt, potion buy will be skipped");
				fNeedToBuyPotions = FALSE; }
			dprintf(DEBUG_ITEMS, "Potion id %d detected in belt", item->m_bItemID);
			EnterCriticalSection(&MainMutex);
			BeltPotions->AddItem(item);
			LeaveCriticalSection(&MainMutex); }
		else {
			delete item; }
		delete itemname;
		return aLen; }

	// WOO HOO!!!! Finally figured out how to link to Nuttzy's PKTK code for item parsing!
	if (aPacket[0] == 0x9d && aPacket[1] == 0x06) {			// item drop to body
		CDiabloItems *item = parseItem(aPacket);
		// FIXME: we're not accounting properly for enhanced durability here
		dprintf(DEBUG_ITEMS, "Wearing item %s (%s): Durability = %d (%d%%)",
			item->m_sCode.ToChar(0),
			item->m_sBaseName.ToChar(0),
			item->m_iDurabilityCur,
			item->m_iDurabilityMax ? item->m_iDurabilityCur * 100 / item->m_iDurabilityMax : 101);
		if (item->m_iDurabilityMax > 0) {
			int DurPct = item->m_iDurabilityCur * 100 / item->m_iDurabilityMax;
			if (DurPct < 10) {
				dprintf(DEBUG_MISC, "Equipment (%s) falling into disrepair, launching repair game", item->m_sBaseName.ToChar(0));
				fRepairGame = TRUE;  } }
		// code to detect starting weapon/shield 
		if (fWeaponSwitch &&
			(state == STATE_START_ISSUED ||
			state == STATE_GAME_STARTED ||
			state == STATE_WALK_TO_ORMUS ) && !strlen(szWeaponPrimary)) {
			if (item->m_bBodyCode == BODY_WEAPON_1 ||
				item->m_bBodyCode == BODY_SHIELD_1 ||
				item->m_bBodyCode == BODY_WEAPON_2 ||
				item->m_bBodyCode == BODY_SHIELD_2) {
				dprintf(DEBUG_MISC, "Wielding %s %s as %s", ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0), BodyCodes[item->m_bBodyCode]);
				switch (item->m_bBodyCode) {
				case BODY_WEAPON_1: {
					sprintf(szWeaponPrimary, "%s %s", ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0));
					dprintf(DEBUG_MISC, "%s is my primary weapon", szWeaponPrimary);
					char *buf;
					buf = server->GetHackProfileString("mephbot-stats", "current_run", "RunName");
					strcpy(szRunName, buf);
					WriteStatString(szRunName, "PrimaryWeapon", szWeaponPrimary);
					break; }
				case BODY_SHIELD_1: {
					sprintf(szShieldPrimary, "%s %s", ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0));
					dprintf(DEBUG_MISC, "%s is my primary shield/left-hand weapon", szShieldPrimary);
					break; }
				default: {
					break; } } } }
		delete item;
		return aLen; }

/*
starting with scimitar in left hand 

Scimitar item ID 55 (55 00 00 00)

starting with scimitar in left hand in PRIMARY weapon, no shield, other tabs empty 

11:58:19p  SENT: 60 
11:58:19p  RECV:  97
                     vv       ITEM ID----    PLAYER ID-- x              x   x
11:58:19p  RECV:  9d 17 23 05 55 00 00 00 00 01 00 00 00 91 00 80 00 65 64 09 30 37 d6 06 02 06 01 00 53 58 58 74 f1 fc 07
                  0  1  2  3  4  5  6  7  8  9  10 11 12 13

ok, so if aPacket[13] & 0x80, item is switched OUT
       if aPacket[13] & 0x40, item is switched IN

weapon switched, now wielding nothing 

11:58:35p  SENT: 60 
11:58:36p  RECV:  97
11:58:36p  RECV:  9d 17 23 05 55 00 00 00 00 01 00 00 00 51 00 80 00 65 84 08 30 37 d6 06 02 06 01 00 53 58 58 74 f1 fc 07

weapon switched, now wielding the scimitar again 
*/

	if (aPacket[0] == 0x9d && aPacket[1] == 0x17) {				// items weapon switched
		CDiabloItems *item = parseItem(aPacket);
		if (aPacket[13] & 0x80) {								// switched OUT
			dprintf(DEBUG_MISC, "%s %s switched out (no longer using) (body code = %d)",
				ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0), item->m_bBodyCode); }
		else if (aPacket[13] & 0x40) {							// switched IN
			switch (item->m_bBodyCode) {
			case BODY_WEAPON_1:
				sprintf(szWeaponCurrent, "%s %s", ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0));
				if (!strcmp(szWeaponCurrent, szWeaponPrimary)) {
					fWeaponCurrent = WEAPON_PRIMARY; }
				else {
					fWeaponCurrent = WEAPON_SECONDARY; }
				break;
			case BODY_SHIELD_1:
				sprintf(szShieldCurrent, "%s %s", ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0));
				break;
			default:
				dprintf(DEBUG_ALL, "ERROR!!! Something really wierd is happening in 0x9d17 packets");
				break; }
			dprintf(DEBUG_MISC, "%s %s switched in (currently using) (body code = %d)",
				ItemQuality[item->m_bQuality], item->m_sBaseName.ToChar(0), item->m_bBodyCode); }
		else {
			dprintf(DEBUG_ALL, "WARNING: Got something unexpected in a 0x9d17 packet"); }
		delete item;
		return aLen; }

	return aLen; }


DWORD EXPORT OnGamePacketBeforeSent(BYTE *aPacket, DWORD aLen) {
	
	if (DebugLevel & DEBUG_PACKETS) {
		DumpPacket(aPacket, aLen, 0); }

	if (!fModuleActive || !InGame()) {
		return aLen; }
	
	// prevent item pickup when depositing gold
	if (aPacket[0] == 0x19 && state == STATE_OPEN_STASH) {
		return 0; }

/*	if (aPacket[0] == 0x19) {
		fItemJustDroppedToStorage = FALSE;
		return aLen; }*/

	// from repair.d2h by Gayak
	if (aPacket[0] == 0x2f) {
		DoRepair(aPacket, aLen);
		return aLen; }

	// (nicked in turn from Cigamit's shopbot.d2h)
	if (aPacket[0] == 0x03) {										// player run
		ClientWalkUpdate(aPacket, aLen);
		return aLen; }
	
/*	if (aPacket[0] == 0x60) {										// weapon switch
		fSwitchingWeapon = TRUE;
		return aLen; }*/

	//deny client sync with server while bot is running
	if (aPacket[0] == 0x5f) {
		return 0; }
	
	return aLen; }


//////////////////////////////////////////////////////////////////////
//
// Dll entry/exit
//
//////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	BOOL hResult = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		
		srand(time(NULL));

		// from maphack.cpp
		// I don't *think* I need this... maybe I should ask Mousepad...
		//DisableThreadLibraryCalls(hInstDLL);
		// Create server struct			
		server = new FUNCTIONENTRYPOINTS; 
		
		// Bind exported functions from server
		HMODULE hModule;
		hModule = (HMODULE)GetModuleHandle("D2HackIt");

		// for AutoIT replacement functions
		hD2Window = FindWindow("Diablo II", "Diablo II");

		// Macros make this look a lot nicer :)
		BIND_TO_SERVER(GamePrintInfo);
		BIND_TO_SERVER(GamePrintError);
		BIND_TO_SERVER(GamePrintVerbose);
		BIND_TO_SERVER(GamePrintString);
		BIND_TO_SERVER(GameCommandLine);
		BIND_TO_SERVER(GameSendPacketToServer);
		BIND_TO_SERVER(GameSendPacketToGame);
		BIND_TO_SERVER(GetFingerprint);
		BIND_TO_SERVER(Intercept);
		BIND_TO_SERVER(GetHackProfileString);
		BIND_TO_SERVER(SetHackProfileString);
		BIND_TO_SERVER(GetThisgameStruct);
		
		// set up maphack stuff
		return InstallPatches();
		break;
		
	case DLL_PROCESS_DETACH:
		// stop maphack stuff
		if (!lpReserved) RemovePatches();
		// kill client
		delete server;
		break;
	} 
	return hResult;
}


//////////////////////////////////////////////////////////////////////
//
// Stubfunctions for 'property get' functions.
//
//////////////////////////////////////////////////////////////////////
LPCSTR	EXPORT GetModuleAuthor()		{return ModuleAuthor;}
LPCSTR	EXPORT GetModuleWebsite()		{return ModuleWebsite;}
DWORD	EXPORT GetModuleVersion()		{return ModuleVersion;}
LPCSTR	EXPORT GetModuleEmail()			{return ModuleEmail;}
LPCSTR	EXPORT GetModuleDescription()	{return ModuleDescription;}

//////////////////////////////////////////////////////////////////////
//
// OnClientCommandLine
// -------------------------------------------------------------------
// The modules own extension of the command line interface. Any custom
// commands you add are parsed here.
//
// Return value should be TRUE, but it is not used at this 
// time.
//
// Arguments when we get here:
// argv[0]			Name of module
// argv[1]			Name of command (If supplied)
// argv[2 ... n]	The rest
//
// Syntax in the game: .<module> <arguments>
//
//////////////////////////////////////////////////////////////////////
BOOL EXPORT OnGameCommandLine(char *argv[], int argc) {
	// Check if user supplied anything at all, if not assume help...
	if (argc==1)
		argv[argc++]="help";
	
	MODULECOMMANDSTRUCT* mcs=ModuleCommands;
	
	while (mcs->szName) {
		if (!stricmp(mcs->szName, argv[1]))
			break;
		mcs++; }
	
	char *p,*t,*msg,*fMsg;
	fMsg=new char[256];
	//
	// Is this a built-in function ?
	if (mcs->szName) {
		//
		// If functions returns FALSE, show usage help
		if (!mcs->pFunc(argv, argc)) {
			t=new char[strlen(mcs->szUsage)+1];
			server->GamePrintInfo("Usage:");
			sprintf((char*)t, "%s", mcs->szUsage);
			if (strlen((char*)t)) {
				msg=p=t;
				while (*p != 0) {
					if (*p == '\n') {
						*(p++) = 0;
						sprintf(fMsg, "ÿc4.%s %s", argv[0], msg);
						server->GamePrintInfo((char*)fMsg);
						if (*p != 0)
							msg = p; }
					else
						p++; }
				sprintf(fMsg, "ÿc4.%s %s", argv[0], msg);
				server->GamePrintInfo((char*)fMsg); }
			delete t; } }
	else {
		// Unknown command, show catch-all help phrase.
		t=new char[128];
		sprintf(t, "Unknown command ÿc4'%s %s'ÿc0 - try ÿc4'.%s help'ÿc0 to get help.",
			argv[0], argv[1], argv[0]);
		server->GamePrintError(t);
		delete t; }

	delete fMsg;
	return TRUE; }


//////////////////////////////////////////////////////////////////////
//
// OnGameCommandHelp
// -------------------------------------------------------------------
// Our default help function.
//
// Syntax in the game: .<module> <arguments>
//
//////////////////////////////////////////////////////////////////////
BOOL PRIVATE OnGameCommandHelp(char **argv, int argc) {
	// If command line is longer than 2, show usage for 'help'
	if (argc > 2) return FALSE;
	
	char t[1024];
	sprintf(t, "Available commands for %s:", argv[0]);
	server->GamePrintInfo(t);
	
	// Loop through ModuleCommands[] and print their names
	for (int i=0; ModuleCommands[i].szName != NULL; i++) {
		sprintf(t, "ÿc4.%s %s", argv[0], ModuleCommands[i].szName);
		server->GamePrintInfo(t); }
	
	sprintf(t, "For help on a specific command, type ÿc4.%s <command> help", argv[0]);
	server->GamePrintInfo(t);
	return TRUE; }
