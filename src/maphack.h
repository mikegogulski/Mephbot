#define WIN32_LEAN_AND_MEAN
#include "mephbot.h"

//////////////////////////////////////////////////////////////////////////////
// stuff abstraced from maphack.cpp by Mousepad
//////////////////////////////////////////////////////////////////////////////

extern DWORD PRIVATE GetCurrentLevel();
extern VOID PRIVATE DrawAutomapAct();
extern VOID D2ExitGame(DWORD);

struct DrlgRoom2;
struct DrlgLevel;
struct DrlgMisc;
struct DrlgAct;

struct PresetUnit {
	DWORD nUnitType; //+00
	DWORD nTxtFileNo; //+04
	DWORD _1[1];
	DWORD xPos; //+0c
	DWORD yPos; //+10
	DWORD _2[2];
	PresetUnit *pNext; //+1c
};

struct RoomTile {
	DrlgRoom2 *pRoom2; //+00
	DWORD *nNum; //+04
	DWORD _1[3];
	RoomTile *pNext; //+14
};

struct DrlgRoom1 {
	DWORD _1[14];
	DrlgRoom2 *pRoom2; //+38
};

struct DrlgRoom2 {
	DrlgLevel *pDrlgLevel; //+00
	DWORD xPos; //+04
	DWORD yPos; //+08
	DWORD _1[4];
	DWORD nPresetType; //+1c
	DWORD *nPresetType2No; //+20
	DWORD _2[41];
	RoomTile *pRoomTiles; //+c8
	PresetUnit *pPresetUnits; //+cc
	DWORD _3[5];
	DrlgRoom1 *pRoom1; //+e4
	DrlgRoom2 *pNext; //+e8
};

struct DrlgLevel {
	DrlgMisc *pDrlgMisc; //+00
	DWORD nLevelNo; //+04
	DWORD _1[10];
	DrlgRoom2 *pRoom2First; //+30
};

struct DrlgMisc {
	DrlgLevel *pLevelFirst; //+00
	DWORD _1[2];
	DrlgAct *pDrlgAct; //+0c
	DWORD _2[249];
	DWORD nStaffTombLvl; //+3f4
	DWORD nBossTombLvl; //+3f8
};

struct DrlgAct {
	DWORD _1[2];
	DrlgMisc *pDrlgMisc; //+08
};

struct ObjectTxt {
	BYTE _1[0x188];
	BYTE nParm0; //+188
	BYTE _2[0x43];
	DWORD nCellNo; //+1cc
};

struct LevelTxt {
	BYTE _1[0x216];
	wchar_t szName[40]; //+216
};

struct ItemTxt {
	BYTE _1[0x144];
	union {
		DWORD dwCode; //+144
		char szCode[4];
	};
	BYTE _2[0x6b];
	BYTE nType; //+1b3
	BYTE _3[4];
	BYTE fQuest; //+1b8
};

struct AutomapCell {
	DWORD fSaved; //+00
	WORD nCellNo; //+04
	WORD xPixel; //+06
	WORD yPixel; //+08
	WORD _1[1];
	AutomapCell *pLess; //+0c
	AutomapCell *pMore; //+10
};

struct AutomapLayer {
	DWORD _1[2];
	AutomapCell *pFloors; //+08
	AutomapCell *pWalls; //+0c
	AutomapCell *pObjects; //+10
	AutomapCell *pExtras; //+14
};

struct GfxCell {
};

struct CellFile {
};

struct CellContext {
	GfxCell *hCell; //+00
	CellFile *pCellFile; //+04
	DWORD nCellNo; //+08
};

struct UnitAny {
	DWORD nUnitType; //+00
	DWORD nTxtFileNo; //+04
	DWORD nUnitId; //+08
	DWORD eMode; //+0c
	DWORD _1[10];
	struct {
		DWORD xpos1; //+00
		DWORD ypos1; //+04
		DWORD xpos2; //+08
		DWORD ypos2; //+0c
		DWORD _1[3];
		DrlgRoom1 *pRoom1; //+1c
	} *pPos; //+38
	DWORD _2[13];
};

struct UnitPlayer : UnitAny {
};

struct UnitMonster : UnitAny {
	struct {
		WORD fBoss:1; //+00
		WORD fMinion:1;
		WORD fChamp:1;
		wchar_t szName[1]; //+02
		DWORD _2[50];
		struct {
			DWORD _3[2];
			DWORD anEnchants[9]; //+08
		} *pEnchants; //+cc
	} *pMonsterData; //+70
	DWORD _4[29];
	DWORD fFlags; //+e8
};

struct ObjectData {
	DWORD _[1];
	BYTE nShrineNo; //+04
};

struct UnitObject : UnitAny {
	ObjectData *pObjectData; //+70
};

struct UnitItem : UnitAny {
	struct {
		DWORD nQuality;
	} *pItemData; //+70
};


