// packethelper.cpp ackmed@gotwalls.com
// most functions modified by syadasti <mike@gogulski.com> for mephbot
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "packethelper.h"
#include "logging.h"
#include "automation.h"
#include "skills.h"

// syadasti
VOID PRIVATE RunToEntity(DWORD type, DWORD id) {
	BYTE packet[17];
	memset(&packet, 0, 17);
	packet[0] = 0x04;
	memcpy(&packet[1], &type, 4);
	memcpy(&packet[5], &id, 4);
	SendPacketToServer(packet, 9); }

// ackmed
BOOL TakePortal(DWORD wpEntityID) {
	InteractWithEntity(2, wpEntityID);
	InteractWithEntity(2, wpEntityID);
	return true; }

BOOL TakeWayPoint(DWORD wpEntityID, DWORD Dest) {

	// first interact with entity
	InteractWithEntity(2, wpEntityID);

	BYTE packet[9];
	memset(&packet, 0, 9);
	packet[0] = 0x49;
	memcpy(&packet[1], &wpEntityID, 4);
	memcpy(&packet[5], &Dest, 4);
	SendPacketToServer(packet, 9);
	return true; }


BOOL InteractWithEntity(DWORD type, DWORD EntityID) {
	
	BYTE packet[9];
	memset(&packet, 0, 9);
	packet[0] = 0x13;
	memcpy(&packet[1], &type, 4);
	memcpy(&packet[5], &EntityID, 4);
	SendPacketToServer(packet, 9);
	return true; }

BOOL StopInteractWithEntity(DWORD type,DWORD EntityID) {

	BYTE packet[9];
	memset(&packet, 0, 9);
	packet[0] = 0x30;
	memcpy(&packet[1], &type, 4);
	memcpy(&packet[5], &EntityID, 4);
	SendPacketToServer(packet, 9);
	return true; }

BOOL TakeStep(WORD x, WORD y) {
	dprintf(DEBUG_PLAYER_MOVE, "Taking step to (%.4X, %.4X)", x, y);
	
	BYTE packet[5];
	memset(&packet, 0, 5);
	packet[0] = 0x03;
	memcpy(&packet[1], &x, 2);
	memcpy(&packet[3], &y, 2);
	SendPacketToServer(packet, 5);
	ForceClientViewLocation(x, y, 0);
	return true; }

BOOL OpenStore(DWORD npcID, WORD x, WORD y) {
	
	// from Infinite Pindlebot by MozartMcLaus and FallNAngel
	BYTE packet[17];
	RunToEntity(1, npcID);

	// click vendor
	memset(&packet, 0, 17);
	packet[0] = 0x59;
	packet[1] = 0x01;
	memcpy(&packet[5], &npcID, 4);
	memcpy(&packet[9], &x, 2);
	memcpy(&packet[13], &y, 2);
	SendPacketToServer(packet, 17);

	InteractWithEntity(1, npcID);
	
	memset(&packet, 0, 17);
	packet[0] = 0x2f;
	packet[1] = 0x01;
	memcpy(&packet[5], &npcID, 4);
	SendPacketToServer(packet, 9);

	Pause(50);

	// trade
	memset(&packet, 0, 17);
	packet[0] = 0x38;
	packet[1] = 0x01;
	memcpy(&packet[5], &npcID, 4);
	SendPacketToServer(packet, 13);

	return true; }

// from berto
BOOL BuyItem(DWORD itemID, DWORD npcID) {
	BYTE packet[17];

	memset(&packet, 0, 17);
	packet[0] = 0x32;
	memcpy(&packet[1], &npcID, 4);
	memcpy(&packet[5], &itemID, 4);
	SendPacketToServer(packet, 17);
	return true; }

BOOL CloseStore(DWORD npcID) {
	StopInteractWithEntity(1, npcID);
	return (StopInteractWithEntity(1, npcID)); }	// YES do it twice.


// forces the client view to goto x, y
// type
//		0 = hard cut to the location
//		1 = faded cut to location (like when taking a waypoint)


BOOL ForceClientViewLocation(WORD x, WORD y, BYTE type) {
	BYTE packet[11];
	
	memset(&packet, 0, 11);
	packet[0] = 0x5f;
	memcpy(&packet[1], &x, 2);
	memcpy(&packet[3], &y, 2);
	SendPacketToServer(packet, 5);

	memset(&packet, 0, 11);
	packet[0] = 0x15;
	packet[2] = 0x01;
	memcpy(&packet[6], &x, 2);
	memcpy(&packet[8], &y, 2);
	packet[10] = type;

	server->GameSendPacketToGame(packet,11);
	return true;
}

// sends the sound message that are trigged by the number pad
BOOL SendSoundToServer(WORD sndID) {
	BYTE packet[3];

	memset(&packet, 0, 3);
	packet[0] = 0x3f;
	memcpy(&packet[1], &sndID, 2);
	SendPacketToServer(packet, 3);
	return true; }

// fakes a sound message from another player or self
BOOL SendSoundToClient(DWORD pID, WORD sndID) {
	BYTE packet[8];

	memset(&packet, 0, 8);
	packet[0] = 0x2c;
	memcpy(&packet[2], &pID, 4);
	memcpy(&packet[6], &sndID, 2);
	server->GameSendPacketToGame(packet, 8);
	return true; }

// stuff below added by syadasti

VOID PRIVATE ResurrectPlayer(VOID) {
	BYTE packet[1];
	memset(&packet, 0, 1);
	packet[0] = 0x41;
	SendPacketToServer(packet, 1); }

VOID OpenStash(DWORD id) {
	InteractWithEntity(2, id); }

/*
deposit 143500 to stash (65 left in inv) 
03:49:35a  SENT: 4f 14 00 02 00 8c 30
*/

VOID PRIVATE DepositGold(DWORD amount) {
	BYTE packet[7];
	WORD g1 = 0, g2 = 0;
	memset(&packet, 0, 7);
	// build deposit packet
	packet[0] = 0x4f;
	packet[1] = 0x14;
	g2 = amount / 65536;
	g1 = amount % 65536;
	memcpy(&packet[3], &g2, 2);
	memcpy(&packet[5], &g1, 2);
	SendPacketToServer(packet, 7); }

/*
03:49:38a  SENT: 4f 12 00 00 00 00 00 
*/

VOID PRIVATE CloseStash(VOID) {
	BYTE packet[7];
	// build "close stash" packet
	memset(&packet, 0, 7);
	packet[0] = 0x4f;
	packet[1] = 0x12;
	// ahhh.. have to do it twice!
	SendPacketToServer(packet, 7);
	Pause(100);
	SendPacketToServer(packet, 7); }			// make sure

VOID WeaponSwitch(VOID) {
	BYTE packet[1];
	memset(&packet, 0, 1);
	packet[0] = 0x60;
	Pause(200);
	SendPacketToServer(packet, 1);
	Pause(200); }

VOID PRIVATE OverheadPkt(const char *aMsg) {
	BYTE OverheadPacket[4096];
	memset(&OverheadPacket, 0, 4096);
	OverheadPacket[0] = 0x26;												// chat message
	OverheadPacket[1] = 0x05;												// overhead
	memcpy(&OverheadPacket[4], &PLAYERID, sizeof(DWORD));					// playerID
	memcpy(&OverheadPacket[11], aMsg, strlen(aMsg));
	server->GameSendPacketToGame(OverheadPacket, strlen(aMsg) + 12); }
	
BOOL PRIVATE BuyItemFillup(DWORD id, DWORD npcid) {
	BYTE packet[17];
	memset(&packet, 0, 17);
	packet[0] = 0x32;
	memcpy(&packet[1], &npcid, 4);
	memcpy(&packet[5], &id, 4);
	packet[12] = 0x80;						// set flag to do a shift-right-click multi-buy
	SendPacketToServer(packet, 17);
	return true; }

VOID PRIVATE TakeStairs(DWORD id) {
	dprintf(DEBUG_MISC, "Taking stairs (StairObjID=%X)", id);
	RunToEntity(5, id);
	Pause(200);
	InteractWithEntity(5, id);
	return; }

// 20 <DWORD id > WORDx 00 00 WORDy 00 00
// 20 id id id id xx xx 00 00 yy yy 00 00
// potins in inventory/stash only (maybe cube)
VOID PRIVATE DrinkStoredPotion(DWORD id) {
	BYTE packet[13];
	memset(&packet, 0, 13);
	packet[0] = 0x20;
	memcpy(&packet[1], &id, sizeof(DWORD));
	memcpy(&packet[5], &PLAYERX, sizeof(WORD));
	memcpy(&packet[9], &PLAYERY, sizeof(WORD));
	dprintf(DEBUG_MISC, "Drinking potion id %08X from stash/cube/inventory", id);
	SendPacketToServer(packet, 13);
	Pause(100); }

// 26 [DWORD id] 00 00 00 00 00 00 00 00
VOID PRIVATE DrinkBeltPotion(DWORD id) {
	BYTE packet[13];
	memset(&packet, 0, 13);
	packet[0] = 0x26;
	memcpy(&packet[1], &id, sizeof(DWORD));
	dprintf(DEBUG_MISC, "Drinking potion id %08X from belt", id);
	SendPacketToServer(packet, 13);
	Pause(100); }

VOID PRIVATE CastSkillOnLocation(int hand, WORD x, WORD y) {
	BYTE packet[5];
	memset(&packet, 0, 5);
	packet[0] = hand == SKILL_RIGHT ? 0x0c : 0x05;
	memcpy(&packet[1], &x, 2);
	memcpy(&packet[3], &y, 2);
	dprintf(DEBUG_SKILLS, "Casting %s at location (%.4X, %.4X)", GetSkillNameById(CurrentSkill[hand]), x, y);
	SendPacketToServer(packet, 5);

	// make skill use visible to client
	// From JED's scripter.d2h by smoke
	BYTE rec_packet[15];
	BYTE dummy = 1;
	rec_packet[0] = 0x4d;
	rec_packet[1] = rec_packet[13] = rec_packet[14] = 0x00;
	memcpy(rec_packet + 2, &thisgame->player->PlayerID, 4);
	memcpy(rec_packet + 6, &CurrentSkill[hand], 2);
	memcpy(rec_packet + 8, &dummy, 1);
	memcpy(rec_packet + 9, &x, 2);
	memcpy(rec_packet + 11, &y, 2);
	
	// Send the player attack packet to the game...
	server->GameSendPacketToGame(rec_packet, 15);

	return; }

VOID PRIVATE SelectSkill(int id, int hand) {
	if (CurrentSkill[hand] != id) {
		//dprintf(DEBUG_ALL, "CurrentSkill[hand] = %d, id = %d, hand = %d", CurrentSkill[hand], id, hand);
		BYTE packet[9];
		memset(&packet, 0, 9);
		packet[0] = 0x3c;
		packet[1] = id & 0xFF;
		packet[2] = (id & 0xFF00) >> 8;
		packet[4] = hand == SKILL_RIGHT ? 0x00 : 0x80;		// Left-click skill flag
		memset(&packet[5], 0xff, 4);
		dprintf(DEBUG_SKILLS, "Selecting %s-click skill %s",
			hand == SKILL_RIGHT ? "right" : "left", GetSkillNameById(id));
		SendPacketToServer(packet, 9);
		Pause(200); } }

// from repair.d2h by Gayak
VOID PRIVATE DoRepair(BYTE *aPacket, DWORD aLen) {
	DWORD npc;
	BYTE packet[17];
	memset(&packet, 0, 17);
	packet[0] = 0x35;
	memcpy(&npc, &aPacket[5], 4);
	memcpy(&packet[1], &npc, 4);
	packet[16] = 0x80;
	SendPacketToServer(packet, 17);
	return; }

// (nicked in turn from Cigamit's shopbot.d2h)
VOID PRIVATE ClientWalkUpdate(BYTE *aPacket, DWORD aLen) {
	// Client walk update, I think this is Icky's findings, not sure, but if it's
	// he should have the thanks for it:)
	BYTE packet[16];
	memset(&packet, 0, 16);
	packet[0] = 0x0f;
	packet[6] = 0x17;
	memcpy (packet + 2, &PLAYERID, 4);
	memcpy (packet + 7, aPacket + 1, 4);
	memcpy (packet + 12, &PLAYERX, 2);
	memcpy (packet + 14, &PLAYERY, 2);
	server->GameSendPacketToGame(packet, 16);
	return; }

VOID OpenCube(DWORD id) {
	BYTE packet[13];
	memset(&packet, 0, 13);
	packet[0] = 0x20;
	memcpy(&packet[1], &id, sizeof(DWORD));
	memcpy(&packet[5], &PLAYERX, sizeof(DWORD));
	memcpy(&packet[9], &PLAYERY, sizeof(DWORD));
	SendPacketToServer(packet, 13);
	return; }

VOID CloseCube(VOID) {
	BYTE packet[7];
	memset(&packet, 0, 7);
	packet[0] = 0x4f;
	packet[1] = 0x17;
	SendPacketToServer(packet, 7);
	return; }

VOID PickUpItemFromInventory(CDiabloItems *item) {
	BYTE packet[5];
	memset(&packet, 0, 5);
	packet[0] = 0x19;
	memcpy(&packet[1], &item->m_bItemID, sizeof(DWORD));
	SendPacketToServer(packet, 5);
	return; }

VOID DropItemToStorage(CDiabloItems *item, int where, LOCATIONSTRUCT *pos) {
	BYTE packet[17];
	memset(&packet, 0, 17);
	packet[0] = 0x18;
	memcpy(&packet[1], &item->m_bItemID, sizeof(DWORD));
	packet[5] = pos->x;
	packet[9] = pos->y;
	packet[13] = (where == ITEM_STASH) * 4 + (where == ITEM_CUBE) * 3;
	SendPacketToServer(packet, 17);
	return; }



