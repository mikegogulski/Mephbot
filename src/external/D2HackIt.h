//////////////////////////////////////////////////////////////////////
// D2HackIt.h
// -------------------------------------------------------------------
// Main incude file for D2HackIt.
//
// <thohell@home.se>
//////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <tlhelp32.h>
#include "LinkedList\LinkedList.h"

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */


#define	EXPORT			__declspec( dllexport ) __cdecl 
#define PRIVATE			__cdecl

//////////////////////////////////////////////////////////////////////
// Version history.
// -------------------------------------------------------------------
// 0.01 2001-09-22 Bare debug-dll
// 0.02 2001-09-24 Added pattern search code
// 0.03 2001-09-25 Added a few exported function
// 0.04 2001-09-25 Started the client/server API
// 0.05 2001-09-25 Ini-file handling
// 0.06 2001-09-26 Pattern now search for patterns, ordinals & offsets
// 0.07 2001-09-26 Changed the way some GamePrint* functions behave
// 0.08 2001-09-30 Added PSAPI support for NT 4.0
// 0.09 2001-10-01 Removed PSAPI code again, it's buggy
// 0.10 2001-10-10 Started adding game-hooks for events.
// 0.11 2001-10-10 Command line interface started
// 0.12 2001-10-10 Added .load/.unload/.list of modules
// 0.13 2001-10-10 Added maphack 4.1 compatibility
// 0.14 2001-10-10 Added binding & dispatching of client event handlers
// 0.15 2001-10-11 Added GameSendPacketToServer
// 0.16 2001-10-11 Rewrote GamePrintText, the old one didn't wrap properly
// 0.17 2001-10-11 Added much better help functions for commands
// 0.18 2001-10-11 Added .send
// 0.19 2001-10-11 Added commandline inteface for modules.
// 0.20 2001-10-11 Killed a few bugs in the commandline code.
// 0.21 2001-10-12 GameSendPacketToServer now works in single as well
// 0.22 2001-10-14 Rewrote the tokenizer to operate on a copy of the data
// 0.23 2001-10-14 Added GameCommandLine to send commands from modules
// 0.24 2001-10-14 Added FormatString for parsing of controlcodes
// 0.25 2001-10-14 Added .say
// 0.26 2001-10-14 Added SetHackProfileString
// 0.27 2001-10-14 Added hook for GamePlayerInfoIntercept
// 0.28 2001-10-14 Added OnGameJoin/OnGameLeave events
// 0.29 2001-10-15 Interim release, some features nerfed/non working
// 0.30 2001-10-18 Added SHOW_REGS() for debugging the server
// 0.31 2001-10-18 Moved GamePacketReceivedIntercept to work with GrabIt
// 0.32 2001-10-18 Removed a bug where game would crash at b.net login
// 0.33 2001-11-10 Fixed a bug where game would crash when speaking :)
// 0.34 2001-11-13 Fixed so you can cancel incoming packets by returning 0
// 0.35 2001-11-13 Minor fixups
// 0.36 2001-11-14 Fixed GameSendPacketToGame (Thanks to TechWarrior)
// 0.37 2001-11-14 Enabled .received to send packets to game
// 0.38 2001-11-14 Fixed .say
// 0.39 2001-11-14 Added .overhead
// 0.40 2001-11-14 Fixed colorcodes to actually work (Try .say $$1YAY!)
// 0.41 2001-11-15 Export GameSendPacketToGame for modules
// 0.42 2001-11-17 Added OnGameTimerTick event for modules
// 0.43 2001-11-20 Added D2Loader support, still a bit buggy
// 0.44 2001-11-21 Rewrote load/unload of clients to be more reliable
// 0.45 2001-11-21 Removed bugs from D2Loader support and OnTimerTick
//////////////////////////////////////////////////////////////////////
#define __SERVERVERSION__		MAKELONG(0,45);

// Sane defaults for prompts
#define DEFAULTINFOPROMPT		"ÿc0(ÿc3INFOÿc0)"
#define DEFAULTERRORPROMPT		"ÿc0(ÿc1ERRORÿc0)"
#define DEFAULTVERBOSEPROMPT	"ÿc0(ÿc2VERBOSEÿc0)"

#define MAXPROMPTLENGTH			0x20



#include "Structs.h"
#include "CommonStructs.h"

#define LOADERMAGIC			0xD1AB101D

// ServerStartStop.cpp
BOOL	PRIVATE ServerStart(HANDLE hModule);
BOOL	PRIVATE ServerStop(void);


// MemorySearchFunctions.cpp
BOOL	PRIVATE PatternEquals(LPBYTE buf, LPWORD pat, DWORD plen);
LPVOID	PRIVATE PatternSearch(LPBYTE buf, DWORD blen, LPWORD pat, DWORD plen);
VOID	PRIVATE MakeSearchPattern(LPCSTR pString, LPWORD pat);
DWORD	PRIVATE GetMemoryAddressFromPattern(LPSTR szDllName, LPCSTR szSearchPattern, DWORD offset);
DWORD	PRIVATE GetBaseAddress(LPSTR szModuleName);
DWORD	PRIVATE GetImageSize(LPSTR szModuleName);
void	PRIVATE SetMemToolType(void);

// psapi.cpp
DWORD	PRIVATE GetBaseAddress_psapi(LPSTR szModuleName);
DWORD	PRIVATE GetImageSize_psapi(LPSTR szModuleName);
BOOL	PRIVATE FindImage_psapi(LPSTR ModuleName, MODULEINFO* mi);

// toolhelp.cpp
DWORD	PRIVATE GetBaseAddress_toolhelp(LPSTR szModuleName);
DWORD	PRIVATE GetImageSize_toolhelp(LPSTR szModuleName);
BOOL	PRIVATE FindImage_toolhelp(LPSTR ModuleName, MODULEENTRY32* lpme);

// GameSendPacketToServer.cpp
BOOL	EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);

// GameSendPacketToGame.cpp
BOOL	EXPORT GameSendPacketToGame(LPBYTE buf, DWORD len);

// IniFileHandlers.cpp
LPSTR	EXPORT GetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName);
BOOL	EXPORT SetHackProfileString(LPCSTR lpHackName, LPCSTR lpSectionName, LPCSTR lpKeyName, LPCSTR lpValue);
BOOL	EXPORT GetFingerprint(LPCSTR szModule, LPCSTR szFingerprintName, FINGERPRINTSTRUCT &fps);

// GamePrintFunctions.cpp
BOOL	EXPORT GamePrintInfo(LPCSTR buf);
BOOL	EXPORT GamePrintError(LPCSTR buf);
BOOL	EXPORT GamePrintVerbose(LPCSTR buf);
BOOL	EXPORT GamePrintString(LPCSTR buf);

// GamePacketReceived.cpp
DWORD __fastcall GamePacketReceivedIntercept(BYTE* aPacket, DWORD aLength);
void GamePacketReceivedInterceptSTUB();

// GamePacketSent.cpp
DWORD __fastcall GamePacketSentIntercept(BYTE* aPacket, DWORD aLength);
void GamePacketSentInterceptSTUB();

// GameSendPacketToServer.cpp
BOOL EXPORT GameSendPacketToServer(LPBYTE buf, DWORD len);

// OtherExportedFunctions.cpp
PTHISGAMESTRUCT EXPORT GetThisgameStruct(void);

// HelperFunctions.cpp
#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9
VOID* PRIVATE d2memcpy(DWORD lpDest, DWORD lpSource, int len);
BOOL  EXPORT Intercept(int instruction, DWORD lpSource, DWORD lpDest, int len);

// FormatString.cpp
DWORD PRIVATE FormatString(LPSTR aString);

// GameCommandLine.cpp
typedef struct gamecommandstruct_t
{
	char*	szName;
	BOOL	(PRIVATE *pFunc)(char** argv, int argc);
	char*	szUsage;
} GAMECOMMANDSTRUCT;
extern GAMECOMMANDSTRUCT GameCommands[];
GAMECOMMANDSTRUCT* PRIVATE FindGameCommand(char* name);

BOOL EXPORT  GameCommandLine(char* buf);
BOOL PRIVATE GameCommandLineHelp(char** argv, int argc);
BOOL PRIVATE GameCommandLineVersion(char** argv, int argc);
BOOL PRIVATE GameCommandLineCredits(char** argv, int argc);
BOOL PRIVATE GameCommandLineLoad(char** argv, int argc);
BOOL PRIVATE GameCommandLineUnload(char** argv, int argc);
BOOL PRIVATE GameCommandLineList(char** argv, int argc);
BOOL PRIVATE GameCommandLineSet(char** argv, int argc);
BOOL PRIVATE GameCommandLineSend(char** argv, int argc);
BOOL PRIVATE GameCommandLineReceive(char** argv, int argc);
BOOL PRIVATE GameCommandLineSendkey(char** argv, int argc);
BOOL PRIVATE GameCommandLineSay(char** argv, int argc);
BOOL PRIVATE GameCommandLineOverhead(char** argv, int argc);
BOOL PRIVATE GameCommandLineCrash(char** argv, int argc);
BOOL PRIVATE GameCommandLineTest(char** argv, int argc);		// Test function


// GamePlayerInfoIntercept.cpp
VOID __fastcall GamePlayerInfoIntercept(PLAYERINFOSTRUCT* pis);
VOID			GamePlayerInfoInterceptSTUB();

// TickThread.cpp
DWORD WINAPI	TickThread(LPVOID lpParameter);

// Debug helpers
typedef struct regs_t
{
	DWORD eax;
	DWORD ebx;
	DWORD ecx;
	DWORD edx;
	DWORD esp;
	DWORD ebp;
	DWORD esi;
	DWORD edi;
} REGS;

#define SHOW_REGS(x)	REGS __regs;\
	fep->GamePrintInfo("Debug: ÿc4" x);\
	__asm {mov __regs.eax, eax}\
	__asm {mov __regs.ebx, ebx}\
	__asm {mov __regs.ecx, ecx}\
	__asm {mov __regs.edx, edx}\
	__asm {mov __regs.esp, esp}\
	__asm {mov __regs.ebp, ebp}\
	__asm {mov __regs.esi, esi}\
	__asm {mov __regs.edi, edi}\
	char __regs_text[128];\
	sprintf(__regs_text, "eax: ÿc4%.8xÿc0 ebx: ÿc4%.8xÿc0 ecx: ÿc4%.8xÿc0 edx: ÿc4%.8xÿc0", __regs.eax, __regs.ebx, __regs.ecx, __regs.edx);\
	fep->GamePrintInfo(__regs_text);\
	sprintf(__regs_text, "esp: ÿc4%.8xÿc0 ebp: ÿc4%.8xÿc0 esi: ÿc4%.8xÿc0 edi: ÿc4%.8xÿc0", __regs.esp, __regs.ebp, __regs.edi, __regs.edi);\
	fep->GamePrintInfo(__regs_text);\

#ifdef __cplusplus
}		             /* Assume C declarations for C++ */
#endif  /* __cplusplus */
