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
#include "util.h"
#include "logging.h"
#include "miscstructs.h"
#include "pktk/DiabloItems.h"
#include "packethelper.h"
#include "automation.h"
//#include <iphlpapi.h>   //iphlpapi.lib

BOOL IsIn(char x, char *s) {
	for (int i = 0; i < strlen(s) && s[i] != '\0'; i++) {
		if (x == s[i]) {
			return TRUE; } }
	return FALSE; }

VOID StripWhitespace(char *out, char *in) {
	int j = 0;
	for (int i = 0; i < strlen(in) && in[i] != '\0'; i++) {
		if (in[i] != ' ' && in[i] != '\t' && in[i] != '\r' && in[i] != '\n') {
			out[j] = in[i];
			j++; } }
	out[j] = '\0'; }

VOID ReadStatDword(char *run, char *key, DWORD &variable)  {
	char *retval;
	int ret;

	retval = server->GetHackProfileString("mephbot-stats", run, key);
	if (strlen(retval)) {
		ret = atoi(retval);
		memcpy(&variable, &ret, sizeof(int));
		dprintf(DEBUG_CONFIG, "ReadStatDword(): %s=%d", key, variable); }
	return; }

VOID WriteStatDword(char *run, char *key, DWORD value) {
	char ultoaBuf[64];

	dprintf(DEBUG_CONFIG, "WriteStatDword(): %s=%d", key, value);
	server->SetHackProfileString("mephbot-stats", run, key, ultoa(value, ultoaBuf, 10)); }

VOID ReadStatString(char *run, char *key, char *variable) {
	char *retval;
	retval = server->GetHackProfileString("mephbot-stats", run, key);
	if (retval && strlen(retval)) {
		strcpy(variable, retval);
		dprintf(DEBUG_CONFIG, "ReadStatString(): %s=%s", key, variable); }
	return; }

VOID WriteStatString(char *run, char *key, char *variable) {
	dprintf(DEBUG_CONFIG, "WriteStatString(): %s=%s", key, variable);
	server->SetHackProfileString("mephbot-stats", run, key, variable);
	return; }

VOID ReadConfigInt(char *key, int &variable)  {
	char *retval;
	int ret;

	retval = server->GetHackProfileString("mephbot", "default", key);
	if (retval && strlen(retval)) {
		ret = atoi(retval);
		memcpy(&variable, &ret, sizeof(int));
		dprintf(DEBUG_CONFIG, "ReadConfigInt(): %s=%d", key, variable); }
	else {
		dprintf(DEBUG_ALL, "WARNING: Config variable %s unset!", key); }
	return; }

VOID ReadConfigString(char *key, char *variable) {
	char *retval;

	retval = server->GetHackProfileString("mephbot", "default", key);
	if (retval && strlen(retval)) {
		strcpy(variable, retval);
		dprintf(DEBUG_CONFIG, "ReadConfigString(): %s=%s", key, variable); }
	else {
		dprintf(DEBUG_ALL, "WARNING: Config variable %s unset!", key); }
	return; }

char RandChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

char *RandString(int len) {
	char *ret = new char[len];

	for (int i = 0; i < len; i++) {
		ret[i] = RandChars[rand() % strlen(RandChars)]; }
	ret[len] = NULL;
	return ret; }

DWORD Dist2(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return (((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))); }

DWORD Dist(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return (DWORD)sqrt((double)Dist2(x1, y1, x2, y2)); }

VOID CreateFlagFile(char *aFname) {
	FILE *TempFD;
	dprintf(DEBUG_FILE_ACCESS, "Touching flag file %s", aFname);
	if ((TempFD = fopen(aFname, "w+"))) {
		dprintf(DEBUG_FILE_ACCESS, "Touch succeeded");
		fclose(TempFD); }
	else
		dprintf(DEBUG_ALL, "WARNING: Unable to touch flag file %s", aFname); }

VOID RemoveFlagFile(char *aFname) {
	dprintf(DEBUG_FILE_ACCESS, "Removing flag file %s", aFname);
	unlink(aFname); }

char *toBinary(BYTE b) {
	char *ret = new char[9];
	sprintf(ret, "%c%c%c%c%c%c%c%c",
		b & 128 ? '1' : '0',
		b & 64 ? '1' : '0',
		b & 32 ? '1' : '0',
		b & 16 ? '1' : '0',
		b & 8 ? '1' : '0',
		b & 4 ? '1' : '0',
		b & 2 ? '1' : '0',
		b & 1 ? '1' : '0');
	return ret; }

// adapted from http://homepage2.nifty.com/spw/hint/GetTcpTable.html
/*
BOOL ChatSocketDetected(VOID) {
	DWORD d=0 ;
	unsigned int count = 0;
	unsigned char *p;
		
	GetTcpTable(NULL, &d, FALSE);
	PMIB_TCPTABLE pTcpTable = (PMIB_TCPTABLE)new char[d] ;
	if (pTcpTable) {
		if (GetTcpTable( (PMIB_TCPTABLE)pTcpTable, &d, FALSE) == NO_ERROR) {
			dprintf(DEBUG_ALL, "Got TCP Table");
			for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
				p = (unsigned char *)&(pTcpTable->table[i].dwRemotePort);
				unsigned int remote_port = *(p + 0) * 256 + *(p + 1);
				// we might do better to look for the TCP4000 D2 connection
				// 6112 bnet protocol, 5 is ESTABLISHED
				if (remote_port == 6112 && pTcpTable->table[i].dwState == 5) {
					dprintf(DEBUG_ALL, "Found a Battle.net connection");
					count++; } } }
		delete [] pTcpTable; }
	dprintf(DEBUG_ALL, "Total of %d Battle.net connections detected", count);
	return (count == 2); }

BOOL GameSocketDetected(VOID) {
	DWORD d=0 ;
	unsigned int count = 0;
	unsigned char *p;
		
	GetTcpTable(NULL, &d, FALSE);
	PMIB_TCPTABLE pTcpTable = (PMIB_TCPTABLE)new char[d] ;
	if (pTcpTable) {
		if (GetTcpTable( (PMIB_TCPTABLE)pTcpTable, &d, FALSE) == NO_ERROR) {
			dprintf(DEBUG_ALL, "Got TCP Table");
			for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
				p = (unsigned char *)&(pTcpTable->table[i].dwRemotePort);
				unsigned int remote_port = *(p + 0) * 256 + *(p + 1);
				// 4000 d2 game protocol, 5 is ESTABLISHED
				if (remote_port == 4000 && pTcpTable->table[i].dwState == 5) {
					//dprintf(DEBUG_ALL, "Found a D2 game connection");
					count++; } } }
		delete [] pTcpTable; }
	dprintf(DEBUG_ALL, "Total of %d D2 game connections detected", count);
	return (count > 0); }

// this is/was an attempt to snoop around inside D2's message queues...
// didn't really work.
VOID Peek(VOID) {

	MSG msg;
	char buf[16384] = "";

	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && strlen(buf) < 16000) {
		sprintf(buf, "%s %X", msg.message); }
	if (strlen(buf) > 2) {
		dprintf(DEBUG_ALL, "Peek: %s", buf); } }
*/
#define	STASH_WIDTH			6
#define	STASH_HEIGHT		8
#define CUBE_WIDTH			3
#define	CUBE_HEIGHT			4
#define	INVENTORY_WIDTH		10
#define	INVENTORY_HEIGHT	4
#define BELT_WIDTH			4
#define	BELT_HEIGHT			4

CDiabloItems *StashContents[STASH_WIDTH][STASH_HEIGHT];
CDiabloItems *CubeContents[CUBE_WIDTH][CUBE_HEIGHT];
CDiabloItems *InventoryContents[INVENTORY_WIDTH][INVENTORY_HEIGHT];
CDiabloItems *BeltContents[BELT_WIDTH][BELT_HEIGHT];
LinkedList *Movables = new LinkedList;

VOID ClearInventory(VOID) {
	int i = 0, j = 0;

	for (i = 0; i < STASH_WIDTH; i++) {
		for (j = 0; j < STASH_HEIGHT; j++) {
			StashContents[i][j] = (CDiabloItems *)NULL; } }
	
	for (i = 0; i < CUBE_WIDTH; i++) {
		for (j = 0; j < CUBE_HEIGHT; j++) {
			CubeContents[i][j] = (CDiabloItems *)NULL; } }
	
	for (i = 0; i < INVENTORY_WIDTH; i++) {
		for (j = 0; j < INVENTORY_HEIGHT; j++) {
			InventoryContents[i][j] = (CDiabloItems *)NULL; } }
	
	for (i = 0; i < BELT_WIDTH; i++) {
		for (j = 0; j < BELT_HEIGHT; j++) {
			BeltContents[i][j] = (CDiabloItems *)NULL; } } }

BOOL StashLocationFree(int x, int y) {
	return (StashContents[x][y] == (CDiabloItems *)NULL); }

BOOL CubeLocationFree(int x, int y) {
	return (CubeContents[x][y] == (CDiabloItems *)NULL); }

LOCATIONSTRUCT *FindOpenStashLocation(int Width, int Height) {
	int i, j, k, m;
	BOOL fBad = TRUE;
	
	dprintf(DEBUG_SOLVE, "Trying to place %dx%d item in stash", Width, Height);
	for (i = 0; i <= STASH_WIDTH - Width; i++) {
		for (j = 0; j <= STASH_HEIGHT - Height; j++) {
			fBad = TRUE;
			if (StashLocationFree(i, j)) {
				dprintf(DEBUG_SOLVE, "Stash (%d, %d) looks promising...", i, j);
				fBad = FALSE;
				for (k = i; k < i + Width; k++) {
					for (m = j; m < j + Height; m++) {
						dprintf(DEBUG_SOLVE, "Trying (%d, %d)", k, m);
						if (!StashLocationFree(k, m)) {
							dprintf(DEBUG_SOLVE, "Sigh... (%d, %d) is used", k, m);
							fBad = TRUE; } } }
				if (!fBad) {
					LOCATIONSTRUCT *ret = new LOCATIONSTRUCT;
					ret->x = i;
					ret->y = j;
					return ret; } } } }
	
	dprintf(DEBUG_SOLVE, "No open stash location available for %dx%d item", Width, Height);	
	return (LOCATIONSTRUCT *)NULL; }

LOCATIONSTRUCT *FindOpenCubeLocation(int Width, int Height) {
	int i, j, k, m;
	BOOL fBad = TRUE;
	
	dprintf(DEBUG_SOLVE, "Trying to place %dx%d item in cube", Width, Height);
	for (i = 0; i <= CUBE_WIDTH - Width; i++) {
		for (j = 0; j <= CUBE_HEIGHT - Height; j++) {
			fBad = TRUE;
			if (CubeLocationFree(i, j)) {
				dprintf(DEBUG_SOLVE, "Cube (%d, %d) looks promising...", i, j);
				fBad = FALSE;
				for (k = i; k < i + Width; k++) {
					for (m = j; m < j + Height; m++) {
						if (!CubeLocationFree(k, m)) {
							dprintf(DEBUG_SOLVE, "Sigh... (%d, %d) is used", k, m);
							fBad = TRUE; } } }
				if (!fBad) {
					LOCATIONSTRUCT *ret = new LOCATIONSTRUCT;
					ret->x = i;
					ret->y = j;
					return ret; } } } }

	dprintf(DEBUG_SOLVE, "No open cube location available for %dx%d item", Width, Height);
	return (LOCATIONSTRUCT *)NULL; }

BOOL GetItemDimensions(int &w, int &h, CDiabloItems *item) {
	if (item->m_pArmor) {
		w = item->m_pArmor->m_InvWidth;
		h = item->m_pArmor->m_InvHeight; }
	else if (item->m_pWeapon) {
		w = item->m_pWeapon->m_InvWidth;
		h = item->m_pWeapon->m_InvHeight; }
	else if (item->m_pMisc) {
		w = item->m_pMisc->m_InvWidth;
		h = item->m_pMisc->m_InvHeight; }
	else {												// something is wrong
		dprintf(DEBUG_ALL, "ERROR: Item neither Armor nor Weapon nor Misc! (Item ID %d)", item->m_bItemID);
		return FALSE; }
	return TRUE; }

VOID AddItemToInventory(CDiabloItems *item) {
	int Width = 1, Height = 1;
	char *itemcode = item->m_sCode.ToChar(0);
	
	if (!strncmp(itemcode, "box", 3)) {					// Horadric Cube
		CubeId = item->m_bItemID; }

	if (item->m_bWhere == ITEM_INV) {
		if (!item->m_isIdentified) {
			dprintf(DEBUG_ITEMS, "Movable item %s ID %d in inventory", itemcode, item->m_bItemID);
			EnterCriticalSection(&UtilMutex);
			Movables->AddItem(item);
			LeaveCriticalSection(&UtilMutex); }
		else if (item->m_pMisc) {
				if (!strncmp(item->m_pMisc->m_Type, "rune", 4) ||
					!strncmp(item->m_pMisc->m_Type, "gem", 3)) {
					dprintf(DEBUG_ITEMS, "Movable item %s ID %d in inventory", itemcode, item->m_bItemID);
					EnterCriticalSection(&UtilMutex);
					Movables->AddItem(item);
					LeaveCriticalSection(&UtilMutex); } } }

	if (!GetItemDimensions(Width, Height, item)) {
		return; }
		
	for (int i = 1; i <= Width; i++) {
		for (int j = 1; j <= Height; j++) {
			switch (item->m_bWhere) {
				
			case ITEM_WEARING: {								// do nothing
				return;
				break; }
				
			case ITEM_INV: {
				dprintf(DEBUG_ITEMS, "%s dropped to inventory (%d, %d)", itemcode, item->m_bPosX + i - 1, item->m_bPosY + j - 1);
				InventoryContents[item->m_bPosX + i - 1][item->m_bPosY + j - 1] = item; 
				break; }
				
			case ITEM_CUBE: {
				dprintf(DEBUG_ITEMS, "%s dropped to cube (%d, %d)", itemcode, item->m_bPosX + i - 1, item->m_bPosY + j - 1);
				CubeContents[item->m_bPosX + i - 1][item->m_bPosY + j - 1] = item; 
				break; }
				
			case ITEM_STASH: {
				dprintf(DEBUG_ITEMS, "%s dropped to stash (%d, %d)", itemcode, item->m_bPosX + i - 1, item->m_bPosY + j - 1);
				StashContents[item->m_bPosX + i - 1][item->m_bPosY + j - 1] = item;
				break; }
				
			default: {
				dprintf(DEBUG_ALL, "ERROR: Got a wacky m_bWhere (%d) for item code %s ID %d", item->m_bWhere, itemcode, item->m_bItemID);
				return;
				break; } } } }
	
	delete itemcode; }

VOID MoveSingleItem(CDiabloItems *item, int where, LOCATIONSTRUCT *pos) {
	switch (where) {
	case ITEM_CUBE: {
		if (CubeId) {
			OpenCube(CubeId);
			while (!fCubeOpen) {
				dprintf(DEBUG_ITEMS, "Waiting for cube to open");
				Sleep(10); }
			PickUpItemFromInventory(item);
			fItemJustDroppedToStorage = FALSE;
			DropItemToStorage(item, ITEM_CUBE, pos);
			while (!fItemJustDroppedToStorage) {
				Sleep(5); }
			CloseCube();
			OpenStash(0x1d);					// FIXME: only works if cube is in stash
			while (fCubeOpen) {
				dprintf(DEBUG_ITEMS, "Waiting for cube to close");
				Sleep(10); } }
		break; }
	case ITEM_STASH: {
		PickUpItemFromInventory(item);
		fItemJustDroppedToStorage = FALSE;
		DropItemToStorage(item, ITEM_STASH, pos);
		while (!fItemJustDroppedToStorage) {
			Sleep(5); }
		break; }
	default: {
		dprintf(DEBUG_ALL, "ERROR: Invalid \"where\" parameter passed to MoveSingleItem() (%d)", where);
		return;
		break; } } }

CRITICAL_SECTION UtilMutex;

VOID MoveItems(VOID) {

	if (!Movables || !Movables->GetItemCount()) {
		return; }

	LinkedItem *linkeditem;

	Movables->SortList(Movables->GetFirstItem());

	for (linkeditem = Movables->GetFirstItem(); linkeditem; linkeditem = Movables->GetNextItem(linkeditem)) {
		if (!linkeditem->lpData) {
			dprintf(DEBUG_ALL, "ERROR: Null pointer in Movables list");
			continue; }
		CDiabloItems *item = (CDiabloItems *)linkeditem->lpData;
	
		char *itemcode = item->m_sCode.ToChar(0);
		LOCATIONSTRUCT *dest;
		int Width = 1, Height = 1;
		
		if (!GetItemDimensions(Width, Height, item)) {
			continue; }
		if (dest = FindOpenStashLocation(Width, Height)) {
			dprintf(DEBUG_MISC, "Moving %s (%dx%d) to Stash (%d, %d)", itemcode, Width, Height, dest->x, dest->y);
			MoveSingleItem(item, ITEM_STASH, dest);
			Pause(50);
			delete dest; }
		else if (dest = FindOpenCubeLocation(Width, Height)) {
			dprintf(DEBUG_MISC, "Moving %s (%dx%d) to Cube (%d, %d)", itemcode, Width, Height, dest->x, dest->y);
			MoveSingleItem(item, ITEM_CUBE, dest);
			Pause(50);
			delete dest; }
		else {
			dprintf(DEBUG_MISC, "No room for %s (%dx%d)", itemcode, Width, Height); }

		delete itemcode; } }

DWORD WINAPI UtilThreadProc(LPVOID lpParameter) {
    THREAD_DATA *tdUtil = (THREAD_DATA *)lpParameter;
	HWND h = tdUtil->hD2Window;

	InitializeCriticalSection(&UtilMutex);

	while(tdUtil->Active) {
		Sleep(5);
		if (fMoveItems) {
			MoveItems();
			fMoveItems = FALSE; } }
	
	DeleteCriticalSection(&UtilMutex);
	dprintf(DEBUG_ALL, "Utility thread exiting.");
	return 0; }
