// packethelper.h ackmed@gotwalls.com
// large modifications by syadasti <mike@gogulski.com>
#pragma once
#define WIN32_LEAN_AND_MEAN
#include "mephbot.h"
#include "logging.h"
#include "pktk\DiabloItems.h"
#include "miscstructs.h"

// functions
BOOL TakePortal(DWORD wpEntityID);
BOOL TakeWayPoint(DWORD wpEntityID, DWORD Destination);
BOOL InteractWithEntity(DWORD type,DWORD EntityID);
BOOL StopInteractWithEntity(DWORD type,DWORD EntityID);
BOOL TakeStep(WORD x, WORD y);
BOOL OpenStore(DWORD npcID,WORD x, WORD y);
BOOL BuyItem(DWORD itemID, DWORD npcID);
BOOL BuyItemFillup(DWORD itemID, DWORD npcID);
BOOL CloseStore(DWORD npcID);
BOOL ForceClientViewLocation(WORD x, WORD y, BYTE type);
BOOL SendSoundToServer(WORD sndID);
BOOL SendSoundToClient(DWORD pID, WORD sndID);
//VOID CastRightClickSkillOnSelf(VOID);
//VOID CastRightClickSkillOnLocation(WORD, WORD);
VOID CastSkillOnLocation(int, WORD, WORD);
VOID WeaponSwitch(VOID);
VOID OpenStash(DWORD);
VOID DepositGold(DWORD);
VOID CloseStash(VOID);
VOID OverheadPkt(const char *);
VOID SelectSkill(int, int);
VOID TakeStairs(DWORD);
VOID DrinkStoredPotion(DWORD);
VOID DrinkBeltPotion(DWORD);
VOID DoRepair(BYTE *, DWORD);
VOID ClientWalkUpdate(BYTE *, DWORD);
VOID MoveSingleItem(CDiabloItems *, int, LOCATIONSTRUCT *);
VOID OpenCube(DWORD);
VOID PickUpItemFromInventory(CDiabloItems *);
VOID DropItemToStorage(CDiabloItems *, int, LOCATIONSTRUCT *);
VOID CloseCube(VOID);
VOID ResurrectPlayer(VOID);
VOID RunToEntity(DWORD, DWORD);