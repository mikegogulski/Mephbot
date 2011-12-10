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
#include "logging.h"

VOID WriteLog(char *szFile, char *aMsg) {
	if (hLogFile && ftell(hLogFile) >= 0) {
		fprintf(hLogFile, "%s\n", aMsg);
		fflush(hLogFile); }
	// FIXME: we should handle the "failed to open file" exception here, but 
	// definitely NOT with dprintf!
	return; }

// what a beast this became... used to be a single little #define!
VOID dprint(char *aMsg1) {
	char TimeBuf[9], DateBuf[9];
	char dprintMsgBuf[4096], dprintMsgBuf2[4096];

	memset(dprintMsgBuf, 0, 4096);
	memset(dprintMsgBuf2, 0, 4096);
//#define MAX_DPRINT	163

	// draft handling for on-screen printing of very large messages
	//for (int i = 0; i <= strlen(aMsg1) / MAX_DPRINT; i++) {
		//strncpy(buf, aMsg1 + (i * MAX_DPRINT), MAX_DPRINT);
	if (InGame() && fDebugToScreen) {
		sprintf(dprintMsgBuf2, "mephbot: %82s%s\0", " ", aMsg1 /*+ (i * MAX_DPRINT)*/);
		server->GamePrintInfo(dprintMsgBuf2); }
	_strtime(TimeBuf);
	_strdate(DateBuf);
	sprintf(dprintMsgBuf, "%s %s %08d mephbot: %s\0", DateBuf, TimeBuf, dwTickGlobal, aMsg1);
	WriteLog(szLogFileName, dprintMsgBuf); //}
	return; }

VOID dprintf(int flags, const char *fmt, ...) {
	if (fDebug) {
		if (DebugLevel == DEBUG_ALL || flags == DEBUG_ALL || flags & DebugLevel) {
			va_list args;
			char dprintfBuf[4096];
			memset(dprintfBuf, 0, 4096);
			va_start(args, fmt);
			vsprintf(dprintfBuf, fmt, args);
			dprint(dprintfBuf);
			va_end(args);
			delete args; } }
	return; }


VOID DumpPacket(BYTE *aPacket, DWORD aLen, int flag) {
	// from thohell & druttis's sniffer.d2h module
	char t[4096];
	memset(t, 0, 4096);
	sprintf(t, flag ? "RECV:" : "SENT:");
	for(int i = 0; i < aLen; i++)
		sprintf(t, "%s %.2x", t, aPacket[i]);
	if (DebugLevel & DEBUG_PACKETS_ASCII) {
		sprintf(t, "%s ", t);
		for(i=0; i < aLen; i++)
			sprintf(t, "%s%c", t, aPacket[i] > 31 && aPacket[i] < 128 ? aPacket[i] : '_'); }
	dprintf(DEBUG_PACKETS, t);
	
	if (DebugLevel & DEBUG_PACKETS_PKTIDENT) {
		// from masterste0's pktident.d2h module
		char text[256];
		char c[5];
		memset(text, 0, 256);
		sprintf(c, "%.2x%.2x\0", aPacket[0], aPacket[1]);	
		strcpy(text, server->GetHackProfileString("pktident", flag ? "recv" : "send", c));
		if (strlen(text) > 4) {
			dprintf(DEBUG_PACKETS, text); }
		else {
			sprintf(c,"%.2x\0",aPacket[0]);	
			strcpy(text, server->GetHackProfileString("pktident", flag ? "recv" : "send", c));
			if (strlen(text) > 4) {
				dprintf(DEBUG_PACKETS, text); } } }
	
	return; }

VOID SetUpLogInfo(char *buf, DWORD tglobal, DWORD tgame, DWORD restarts, DWORD aborts, DWORD runs, DWORD kills, DWORD deaths, DWORD chickens) {
	sprintf(buf,
		"tGlobal=~%d:%02d:%02d.%d tGame=~%02d:%02d.%d Restarts=%02d Aborts=%02d Runs=%02d Kills=%02d Deaths=%02d Chickens=%02d",
		tglobal / 10 / 60 / 60,		// hr
		(tglobal / 10 / 60) % 60,	// min
		(tglobal / 10) % 60,		// sec
		tglobal % 10,				// tenths
		(tgame / 10 / 60) % 60,		// min
		(tgame / 10) % 60,			// sec
		tgame % 10,					// tenths
		restarts, aborts, runs, kills, deaths, chickens);
		return; }
		
VOID LogInfo(char *buf, DWORD tglobal, DWORD tgame, DWORD restarts, DWORD aborts, DWORD runs, DWORD kills, DWORD deaths, DWORD chickens) {
	SetUpLogInfo(buf, tglobal, tgame, restarts, aborts, runs, kills, deaths, chickens);
	dprintf(DEBUG_ALL, buf);
	return; }
