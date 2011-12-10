// maphack.cpp
//
// modified by syadasti <mike@gogulski.com>
//
//
// Based on Mousepad's Diablo II Maphack v4.4 (v1.09d)
// Derivative works to be distributed for free with full source and full credits (ie dont blame me)
// © 2001-2 mousepad - mousepad@forward.to
//
// the entire point of this file is to determine the x,y coordinates of the stairs down in
// durance 2


#define WIN32_LEAN_AND_MEAN
#include "logging.h"
#include "maphack.h"
#include "dijkstra.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>


char szProgTitle[] = "Mousepad's Diablo II Maphack v4.4 (v1.09d)";

#define CELLNO_MYSHRINES 1176
#define CELLNO_SHRINE 310
#define NUMOF_SHRINES 23

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )

//d2 structures etc

enum {UNITNO_PLAYER = 0, UNITNO_MONSTER = 1, UNITNO_OBJECT = 2, UNITNO_MISSILE = 3, UNITNO_ITEM = 4, UNITNO_ROOMTILE = 5};
//d2 ptrs

DWORD GetDllOffset(char *dll, int offset)
{
	HMODULE hmod = GetModuleHandle(dll);
	if (!hmod) return 0;
	if (offset < 0) {
		return (DWORD)GetProcAddress(hmod, (LPCSTR)-offset);
	}
	return ((DWORD)hmod)+offset;
}

DWORD GetDllOffset(int num)
{
	static char *dlls[] = {"D2CLIENT.DLL", "D2COMMON.DLL", "D2GFX.DLL", "D2WIN.DLL", "D2LANG.DLL", "D2CMP.DLL"};
	return GetDllOffset(dlls[num&0xff], num>>8);
}

enum {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2WIN, DLLNO_D2LANG, DLLNO_D2CMP};

enum {DLLBASE_D2CLIENT = 0x6FAA0000, DLLBASE_D2COMMON = 0x6FD40000, DLLBASE_D2GFX = 0x6FA70000, DLLBASE_D2WIN = 0x6F8A0000, DLLBASE_D2LANG = 0x6FC10000, DLLBASE_D2CMP = 0x6FDF0000};

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|(( ((b1)<0)?(b1):(b1)-DLLBASE_##a1 )<<8))

//NOTE :- reference vars buggy

#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
// typedef UnitPlayer * D2CLIENT_pPlayerUnit_t;
// D2CLIENT_pPlayerUnit_t *p_D2CLIENT_pPlayerUnit = ...
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);

#define D2PtrsStart p_D2CLIENT_pDrlgAct

D2VARPTR(D2CLIENT, pDrlgAct, DrlgAct *, 0x6FBB0BE4)
D2VARPTR(D2CLIENT, pPlayerUnit, UnitPlayer *, 0x6FBC63F8)

D2VARPTR(D2CLIENT, pAutomapLayer, AutomapLayer *, 0x6FBBBC00)
D2VARPTR(D2CLIENT, fAutomapSize, DWORD, 0x6FBBBBEC)
D2VARPTR(D2CLIENT, fAutomapOn, DWORD, 0x6FBC48DC)
D2VARPTR(D2CLIENT, xAutomap, int, 0x6FBBBC24)
D2VARPTR(D2CLIENT, yAutomap, int, 0x6FBBBC28)

D2FUNCPTR(D2COMMON, InitDrlgLevel, void __stdcall, (DrlgLevel *drlglevel), -0x2716)
D2FUNCPTR(D2COMMON, GetDrlgLevel, DrlgLevel * __stdcall, (DrlgMisc *drlgmisc, DWORD levelno), -0x2715)

D2FUNCPTR(D2CLIENT, RecvCommand07, void __fastcall, (BYTE *cmdbuf), 0x6FAB1C80)
D2FUNCPTR(D2CLIENT, RecvCommand08, void __fastcall, (BYTE *cmdbuf), 0x6FAB1CF0)
D2FUNCPTR(D2CLIENT, DrawAutomapRoom, void __fastcall, (DrlgRoom1 *room1, DWORD clipflag, AutomapLayer *layer), 0x6FAC6E70)

D2FUNCPTR(D2CLIENT, NewAutomapCell, AutomapCell * __fastcall, (), 0x6FAC5700)
D2FUNCPTR(D2CLIENT, AddAutomapCell, void __fastcall, (AutomapCell *cell, AutomapCell **node), 0x6FAC6A40)
D2FUNCPTR(D2GFX, DrawAutomapCell, void __stdcall, (CellContext *context, DWORD xpos, DWORD ypos, DWORD unknown1, DWORD unknown2), -0x275D)

D2FUNCPTR(D2CMP, InitCellFile, void __stdcall, (void *cellfile, CellFile **outptr, char *srcfile = "?", DWORD lineno = 0, DWORD filever = -1, char *filename = "?"), -0x2728)
D2FUNCPTR(D2CMP, DeleteCellFile, void __stdcall, (CellFile *cellfile), -0x2730)

D2FUNCPTR(D2COMMON, GetObjectTxt, ObjectTxt * __stdcall, (DWORD objno), -0x2982)
D2FUNCPTR(D2COMMON, GetItemTxt, ItemTxt * __stdcall, (DWORD itemno), -0x2968)
D2FUNCPTR(D2COMMON, GetLevelTxt, LevelTxt * __stdcall, (DWORD levelno), -0x2987)

D2FUNCPTR(D2COMMON, GetUnitStat, int __stdcall, (UnitAny *unit, DWORD statno), -0x2917)
D2FUNCPTR(D2COMMON, GetUnitState, int __stdcall, (UnitAny *unit, DWORD stateno), -0x28F7)
D2FUNCPTR(D2GFX, GetHwnd, HWND __stdcall, (), -0x272B)
D2FUNCPTR(D2CLIENT, SetExitApp, void __fastcall, (DWORD flag), 0x6FAA2000)

D2ASMPTR(D2CLIENT, InitAutomapLayer, 0x6FAC5884)
D2ASMPTR(D2CLIENT, InitAutomapLayer_END, 0x6FAC594F)
D2ASMPTR(D2CLIENT, OverrideCell_ORIG, 0x6FBB9BEC)
D2ASMPTR(D2COMMON, LightingHack_ORIG, -0x2749)
D2ASMPTR(D2COMMON, MonsterNameHack_ORIG, -0x2956)

#define D2PtrsEnd D2COMMON_MonsterNameHack_ORIG

inline void RelocD2Ptrs()
{
	DWORD *p = (DWORD *)&D2PtrsStart;
	do {
		*p++ = GetDllOffset(*p);
	} while (p <= (DWORD *)&D2PtrsEnd);
}

#define D2CLIENT_pDrlgAct (*p_D2CLIENT_pDrlgAct)
#define D2CLIENT_pPlayerUnit (*p_D2CLIENT_pPlayerUnit)

#define D2CLIENT_pAutomapLayer (*p_D2CLIENT_pAutomapLayer)
#define D2CLIENT_fAutomapSize (*p_D2CLIENT_fAutomapSize)
#define D2CLIENT_fAutomapOn (*p_D2CLIENT_fAutomapOn)
#define D2CLIENT_xAutomap (*p_D2CLIENT_xAutomap)
#define D2CLIENT_yAutomap (*p_D2CLIENT_yAutomap)

#define D2CLIENT_xMapShake (*p_D2CLIENT_xMapShake)
#define D2CLIENT_yMapShake (*p_D2CLIENT_yMapShake)

#define D2COMMON_nWeaponsTxt (*p_D2COMMON_nWeaponsTxt)
#define D2COMMON_nArmorTxt (*p_D2COMMON_nArmorTxt)

//memory helpers

#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9
#define INST_JMPR 0xeb
#define INST_RET 0xc3

#define DB(x) __asm _emit (x)
#define DW(x) __asm _emit (x)&0xff __asm _emit (x)>>8&0xff
#define DD(x) __asm _emit (x)&0xff __asm _emit (x)>>8&0xff __asm _emit (x)>>16&0xff __asm _emit (x)>>24&0xff

DWORD VirtualProtect(DWORD pAddress, DWORD len, DWORD prot)
{
	DWORD oldprot = 0;
	VirtualProtect((void *)pAddress, len, prot, &oldprot);
	return oldprot;
}

void ReadLocalBYTES(DWORD pAddress, void *buf, int len)
{
	ReadProcessMemory(GetCurrentProcess(), (void *)pAddress, buf, len, 0);
}

void WriteLocalBYTES(DWORD pAddress, void *buf, int len)
{
	DWORD oldprot = VirtualProtect(pAddress, len, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(GetCurrentProcess(), (void *)pAddress, buf, len, 0);
	VirtualProtect(pAddress, len, oldprot);
}

void FillLocalBYTES(DWORD pAddress, BYTE ch, int len)
{
	BYTE *buf1 = new BYTE[len];
	memset(buf1, ch, len);
	WriteLocalBYTES(pAddress, buf1, len);
	delete buf1;
}

void InterceptLocalCode(BYTE inst, DWORD pOldCode, DWORD pNewCode, int len = 5)
{
	BYTE *buf1 = new BYTE[len];
	buf1[0] = inst;
	*(DWORD *)(buf1+1) = pNewCode-(pOldCode+5);
	memset(buf1+5, INST_NOP, len-5);
	WriteLocalBYTES(pOldCode, buf1, len);
	delete buf1;
}


//automap stuff

void __declspec(naked) __fastcall InitAutomapLayer(DWORD levelno)
{
	__asm {
//ecx = level no
		mov eax,D2CLIENT_InitAutomapLayer_END
		mov byte ptr [eax],INST_RET
		pushad
		xor ebp,ebp
		call D2CLIENT_InitAutomapLayer
		popad
		mov byte ptr [eax],0x8b
		ret
	}
}

DWORD GetPresetUnitObjCell(DWORD objno, DWORD levelno)
{
	if (objno == 397) return 318; //chests
	if (objno == 152) return 300; //orifice
	if (objno == 460) return 1468; //drehya = frozen anya
	if ((objno == 402) && (levelno == 46)) return 0; //canyon/arcane waypoint
	if ((objno == 267) && (levelno != 75) && (levelno != 103)) return 0; //bank not act3,4 town
	if (objno >= 574) { //random
		static WORD rndobjcells[9] = { //rnd obj no, new obj no, shrine min, shrine max (not inclusive)
			CELLNO_MYSHRINES+0,  //574 = 310 (shrine), 2,  6
			CELLNO_MYSHRINES+7,  //575 = 310 (shrine), 7,  7
			CELLNO_MYSHRINES+0,  //576 = 310 (shrine), 8,  11
			CELLNO_MYSHRINES+12, //575 = 310 (shrine), 12, 12
			CELLNO_MYSHRINES+0,  //578 = 310 (shrine), 1,  5
			CELLNO_MYSHRINES+14, //579 = 310 (shrine), 14, 14
			0, //580 = chest
			0, //581 = chest
			0, //582 = arcane symbol
		};
		return rndobjcells[objno-574];
	}
	ObjectTxt *obj = D2COMMON_GetObjectTxt(objno);
	DWORD cellno = obj->nCellNo;
	return cellno;
}

DrlgRoom2 *GetRoomTileOtherRoom2(DrlgRoom2 *room2, DWORD roomtileno)
{
	RoomTile *roomtile = room2->pRoomTiles;
	while (roomtile) {
		if (*roomtile->nNum == roomtileno) {
			return roomtile->pRoom2;
		}
		roomtile = roomtile->pNext;
	}
	return 0;
}

WORD StairsX, StairsY;

void GrabStairsPos(DrlgRoom2 *room, int x, int y) {
	WORD roomno;	
	roomno = *(room->nPresetType2No);
	
	if (room->pDrlgLevel->nLevelNo == 101) {
		if (roomno >= 788 && roomno <= 791) {
			// adjust this constant
			StairsX = (WORD)x + 5;
			StairsY = (WORD)y + 5;
			dprintf(DEBUG_PLAYER_MOVE, "Maphack gave us stairs position (%.4X, %.4X)", StairsX, StairsY); } } }

VOID GrabRoomLocation(DrlgRoom2 *room2, int x, int y) {
	if (room2 && room2->pRoomTiles && room2->pRoomTiles->nNum) {
		dprintf(DEBUG_PLAYER_MOVE, "room tile %d at (%.4X, %.4X)", *(room2->pRoomTiles->nNum), x, y); }

	// ideally
	// determine if tile is a wall or other solid object, reject if so
	// add to linked list
}

void DrawAutomapRoomCell(int xpos, int ypos, WORD cellno, DrlgRoom2 *room2, int xoffset = 0, int yoffset = 0)
{
	AutomapCell *cell = D2CLIENT_NewAutomapCell();
	cell->nCellNo = cellno;
	xpos = (room2->xPos*5)+xpos;
	ypos = (room2->yPos*5)+ypos;
	cell->xPixel = (((xpos-ypos)*16)/10)+1+xoffset;
	cell->yPixel = (((ypos+xpos)*8)/10)-3+yoffset;
	GrabStairsPos(room2, xpos, ypos);
	//if (room2->pPresetUnits->nUnitType == UNITNO_ROOMTILE) {
		//GrabRoomLocation(room2, xpos, ypos);// }
	D2CLIENT_AddAutomapCell(cell, &D2CLIENT_pAutomapLayer->pObjects);
}

void DrawAutomapRoom2b(DrlgRoom2 *room2)
{
	D2CLIENT_DrawAutomapRoom(room2->pRoom1, 1, D2CLIENT_pAutomapLayer);

	PresetUnit *unit = room2->pPresetUnits;
	while (unit) {
		//GrabRoomLocation(unit, unit->xPos*5, unit->yPos*5);
		//dprintf(DEBUG_PLAYER_MOVE, "Unit type %X at (%.4X, %.4X)", unit->nUnitType, unit->xPos, unit->yPos);
		//dprintf(DEBUG_PLAYER_MOVE, "global coordinates (%.4X, %.4X)", unit->xPos + 5 * room2->xPos, unit->yPos + 5 * room2->yPos);
		int cellno = 0, xoffset = 0, yoffset = 0;
		switch (unit->nUnitType) {
			case UNITNO_MONSTER:
				if (unit->nTxtFileNo == 256) cellno = 300; //izual
				break;

			case UNITNO_OBJECT:
				cellno = GetPresetUnitObjCell(unit->nTxtFileNo, room2->pDrlgLevel->nLevelNo);
				break;

			case UNITNO_ROOMTILE:
				DrlgRoom2 *otherroom2 = GetRoomTileOtherRoom2(room2, unit->nTxtFileNo);
				if (otherroom2) {
					//change pos here because better for minimap
					cellno = -(int)(otherroom2->pDrlgLevel->nLevelNo);
					xoffset = +8;
					yoffset = -28;
				}
				break;
		}
		if (cellno) {
			DrawAutomapRoomCell(unit->xPos, unit->yPos, cellno, room2, xoffset, yoffset);
		}
		unit = unit->pNext;
	}
}

void DrawAutomapRoom2(DrlgRoom2 *room2)
{
	if (room2->nPresetType == 2) {
		DWORD presetno = *room2->nPresetType2No;
		if (presetno >= 38 && presetno <= 39) return; //Act 1 - Swamp Fill 1&2
		if (presetno >= 401 && presetno <= 405) return; //Act 2 - Desert Fill Bone 1&2, Desert Fill Wagon 1, Desert Fill Berms 1&2
		if (presetno == 836) return; //Act 4 - Lava X
		if (presetno == 863) return; //Act 5 - Town
	}
	if (room2->pRoom1) {
		DrawAutomapRoom2b(room2);
	} else {
		BYTE cmdbuf[6];
		*(WORD *)(cmdbuf+1) = (WORD)room2->xPos;
		*(WORD *)(cmdbuf+3) = (WORD)room2->yPos;
		cmdbuf[5] = (BYTE)room2->pDrlgLevel->nLevelNo;
		D2CLIENT_RecvCommand07(cmdbuf);
		DrawAutomapRoom2b(room2);
		D2CLIENT_RecvCommand08(cmdbuf);
	}
}

/*BOOL seenunit(PresetUnit *unit, DWORD x, DWORD y) {
	BOOL seen = FALSE;
	for (int i = 0; i <= PointsIndex; i++) {
		if (Points[i].x == 5 * x + unit->xPos &&
			Points[i].y == 5 * y + unit->yPos) {
			seen = TRUE;
			break; } }
	return seen; }*/


/*VOID DumpPresetLocations(PresetUnit *unit, DWORD x, DWORD y) {
	if (!seenunit(unit, x, y)) {
		Points[PointsIndex].x = 5 * x + unit->xPos;
		Points[PointsIndex].y = 5 * y + unit->yPos;
		PointsIndex++;
		//dprintf(DEBUG_PLAYER_MOVE, "Unit: (%.4X, %.4X) in room (%.4X, %.4X)", 5*x+unit->xPos, 5*y+unit->yPos, 5*x, 5*y);
	}
	if (unit->pNext) {
		DumpPresetLocations(unit->pNext, x, y); }
}*/

LinkedList *roomlist = new LinkedList;

#define MAPOFFSET	+30

BOOL seen(DrlgRoom2 *room) {
	for (BYTE i = 0; i <= bPointsIndex; i++) {
		if (5 * room->xPos + MAPOFFSET == Points[i].x &&
			5 * room->yPos + MAPOFFSET == Points[i].y) {
			return TRUE; } }
	return FALSE; }

VOID DumpRoomLocations(DrlgRoom2 *room) {
	if (!seen(room)) {
		Points[bPointsIndex].x = 5 * room->xPos + MAPOFFSET;
		Points[bPointsIndex].y = 5 * room->yPos + MAPOFFSET;
		bPointsIndex++;
		dprintf(DEBUG_PLAYER_MOVE, "Room: (%.4X, %.4X)", 5 * room->xPos + MAPOFFSET, 5 * room->yPos + MAPOFFSET);
		/*if (room->pPresetUnits) {
			DumpPresetLocations(room->pPresetUnits, room->xPos, room->yPos); }*/
		if (room->pNext) {
			DumpRoomLocations(room->pNext); } }
}

void DrawAutomapLevel(DrlgLevel *pDrlgLevel) {
	
	DrlgRoom2 *currroom = pDrlgLevel->pRoom2First;
	while (currroom) {
		// this is the meat of the detection.  preset rooms 788 through 791 are the "stairs down"
		// rooms in durance 2
		//if (*(currroom->nPresetType2No) >= 788 && *(currroom->nPresetType2No) <= 791) {
		if (pDrlgLevel->nLevelNo == 101) {
			DrawAutomapRoom2(currroom);
			DumpRoomLocations(currroom); }
		currroom = currroom->pNext; } }

DrlgLevel *GetUnitDrlgLevel(UnitAny *unit)
{
	return unit->pPos->pRoom1->pRoom2->pDrlgLevel;
}

void DrawAutomapLevel()
{
	DrawAutomapLevel(GetUnitDrlgLevel(D2CLIENT_pPlayerUnit));
}

DWORD CurrentLevel = 0;

DWORD GetCurrentLevel() {
	if (InGame())
		// make GODDAMN SURE we don't try to walk through a null pointer here
		if (D2CLIENT_pPlayerUnit &&
			D2CLIENT_pPlayerUnit->pPos &&
			D2CLIENT_pPlayerUnit->pPos->pRoom1 &&
			D2CLIENT_pPlayerUnit->pPos->pRoom1->pRoom2 &&
			D2CLIENT_pPlayerUnit->pPos->pRoom1->pRoom2->pDrlgLevel &&
			D2CLIENT_pPlayerUnit->pPos->pRoom1->pRoom2->pDrlgLevel->nLevelNo)
			//return GetUnitDrlgLevel(D2CLIENT_pPlayerUnit) ? GetUnitDrlgLevel(D2CLIENT_pPlayerUnit)->nLevelNo : 0;
			//return GetUnitDrlgLevel(D2CLIENT_pPlayerUnit)->nLevelNo;
			return D2CLIENT_pPlayerUnit->pPos->pRoom1->pRoom2->pDrlgLevel->nLevelNo;
		else
			return (DWORD)0;
	else
		return (DWORD)0; }

void DrawAutomapAct()
{
	static BYTE actlvls[] = {1, 40, 75, 103, 109, 133};
	if (D2CLIENT_pPlayerUnit == 0 || GetUnitDrlgLevel(D2CLIENT_pPlayerUnit) == 0)
		return;
	DWORD currlvl = GetUnitDrlgLevel(D2CLIENT_pPlayerUnit)->nLevelNo;
	CurrentLevel = currlvl;
	DWORD act = 0;
	do {} while (currlvl >= actlvls[++act]);
	
	for (DWORD lvl = actlvls[act-1]; lvl < actlvls[act]; lvl++) {
	//DWORD lvl=101;
		DrlgLevel *pDrlgLevel = D2COMMON_GetDrlgLevel(D2CLIENT_pDrlgAct->pDrlgMisc, lvl);
		if (!pDrlgLevel->pRoom2First) {
			D2COMMON_InitDrlgLevel(pDrlgLevel);
		}
		InitAutomapLayer(lvl);
		DrawAutomapLevel(pDrlgLevel);
	}

	InitAutomapLayer(currlvl);
}

void D2ExitGame(DWORD flag)
{
	D2CLIENT_SetExitApp(flag);
	SendMessage(D2GFX_GetHwnd(), WM_CLOSE, 0, 0);
}



BYTE fShowVisuals = 1;

void __declspec(naked) LightingPatch_ASM()
{
	__asm {
//edx = ptr red
//[esp+0x0c] = ptr green
//[esp+0x10] = ptr blue
//return eax = intensity
		cmp [fShowVisuals],0
		je lightold

		mov eax,0xff
		mov [edx],al
		mov esi,[esp+0x0c]
		mov [esi],al
		mov esi,[esp+0x10]
		mov [esi],al
		pop esi
		pop esi
		ret 0x08
lightold:
//original code
		push ecx
		call D2COMMON_LightingHack_ORIG
		ret
	}
}

//keydown stuff

BYTE vkRevealAct = VK_SUBTRACT;
BYTE vkRevealLevel = -1;

void __fastcall KeydownPatch(BYTE keycode, BYTE repeat)
{
	if (keycode == (BYTE)-1) return;
	if (keycode == vkRevealAct) DrawAutomapAct();
}

void __declspec(naked) KeydownPatch_ASM()
{
	__asm {
//edi = ptr to (hwnd, msg, wparam, lparam)
		mov cl,[edi+0x08] //nVirtKey (wparam)
		mov dl,[edi+0x0c+3] //lKeyData (lparam)
		and dl,0x40 //bit 30 of lKeyData (lparam)
		call KeydownPatch
//original code
		test byte ptr [edi+0x0c+3],0x40 //bit 30 of lKeyData (lparam)
		ret
	}
}


//patches stuff

void PatchCALL(DWORD addr, DWORD param, DWORD len)
{
	InterceptLocalCode(INST_CALL, addr, param, len);
}

void PatchJMP(DWORD addr, DWORD param, DWORD len)
{
	InterceptLocalCode(INST_JMP, addr, param, len);
}

void PatchFILL(DWORD addr, DWORD param, DWORD len)
{
	FillLocalBYTES(addr, (BYTE)param, len);
}

void PatchVALUE(DWORD addr, DWORD param, DWORD len)
{
	WriteLocalBYTES(addr, &param, len);
}

struct {
	void (*func)(DWORD, DWORD, DWORD);
	DWORD addr;
	DWORD param;
	BYTE len;
	DWORD mask;
	BYTE *oldcode;

} aD2Patches[] = {
	{PatchCALL,   DLLOFFSET(D2CLIENT, 0x6FAB8763),   (DWORD)LightingPatch_ASM, 6, -1},
	{PatchCALL,   DLLOFFSET(D2CLIENT, 0x6FACEF77),   (DWORD)KeydownPatch_ASM, 7, -1},
};

void KillCellAssert()
{
	static BYTE data1[] = {0x33,0xc0,0x5f,0xeb,0x31};
	WriteLocalBYTES(GetDllOffset(DLLOFFSET(D2CMP, 0x6FDF2724)), data1, sizeof(data1));
	static BYTE data2[] = {0xeb,0x03};
	WriteLocalBYTES(GetDllOffset(DLLOFFSET(D2CMP, 0x6FDFC189)), data2, sizeof(data2));
}

BOOL InstallPatches()
{
	if (!GetModuleHandle("D2CLIENT.DLL")) if (!LoadLibrary("D2CLIENT.DLL")) return FALSE;

	RelocD2Ptrs();

	for (int i = 0; i < ARRAYSIZE(aD2Patches); i++)
		aD2Patches[i].addr = GetDllOffset(aD2Patches[i].addr);

	for (int j = 0; j < ARRAYSIZE(aD2Patches); j++) {
		aD2Patches[j].oldcode = new BYTE[aD2Patches[j].len];
		ReadLocalBYTES(aD2Patches[j].addr, aD2Patches[j].oldcode, aD2Patches[j].len);
		aD2Patches[j].func(aD2Patches[j].addr, aD2Patches[j].param, aD2Patches[j].len);
	}

	VirtualProtect(D2CLIENT_InitAutomapLayer_END, 1, PAGE_EXECUTE_READWRITE);
	KillCellAssert();

	return TRUE;
}

void RemovePatches() {

	for (int i = 0; i < ARRAYSIZE(aD2Patches); i++) {
		WriteLocalBYTES(aD2Patches[i].addr, aD2Patches[i].oldcode, aD2Patches[i].len);
		delete aD2Patches[i].oldcode; } 
}



