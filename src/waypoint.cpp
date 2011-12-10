#define WIN32_LEAN_AND_MEAN
// waypoint.cpp ackmed@gotwalls.com
#include "waypoint.h"

WPSTRUCT allWP[] = {
	{ 0,true,true,true ,{0,0},0x0077,WAYPOINT_ACT1_1,"rogue encampment" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_2,"cold plains" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_3,"stony field" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_4,"dark wood" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_5,"black marsh" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_6,"outer cloister" },
	{ 0,true,true,false,{0,0},0x009d,WAYPOINT_ACT1_7,"jail level 1" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_8,"inner cloister" },
	{ 0,true,true,false,{0,0},0x0077,WAYPOINT_ACT1_9,"catacombs level 2" },
//9
	{ 0,true,true,true ,{0,0},0x009c,WAYPOINT_ACT2_1,"lut gholein" },
	{ 0,true,true,false,{0,0},0x0143,WAYPOINT_ACT2_2,"sewers level 2" },
	{ 0,true,true,false,{0,0},0x009c,WAYPOINT_ACT2_3,"dry hills" },
	{ 0,true,true,false,{0,0},0x009c,WAYPOINT_ACT2_4,"halls of the dead 2" },
	{ 0,true,true,false,{0,0},0x009c,WAYPOINT_ACT2_5,"far oasis" },
	{ 0,true,true,false,{0,0},0x009c,WAYPOINT_ACT2_6,"lost city" },
	{ 0,true,true,false,{0,0},0x0120,WAYPOINT_ACT2_7,"palace cellar level 1" },
	{ 0,true,true,false,{0,0},0x0192,WAYPOINT_ACT2_8,"arcane sanctuary" },
	{ 0,true,true,false,{0,0},0x0192,WAYPOINT_ACT2_9,"canyon of the magi" },
//9+9=18
	{ 0,true,true,true ,{0,0},0x00ed,WAYPOINT_ACT3_1,"kurast docks" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_2,"spider forest" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_3,"great marsh" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_4,"flayer jungle" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_5,"lower kurast" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_6,"kurast bazzur" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_7,"upper kurast" },
	{ 0,true,true,false,{0,0},0x00ed,WAYPOINT_ACT3_8,"travincal" },
	{ 0,true,true,false,{0,0},0x0144,WAYPOINT_ACT3_9,"durance of hate level 2" },
//18+9=27	
	{ 0,true,true,true ,{0,0},0x018e,WAYPOINT_ACT4_1,"the pandemonium fortress" },
	{ 0,true,true,false,{0,0},0x00ee,WAYPOINT_ACT4_2,"city of the damned" },
	{ 0,true,true,false,{0,0},0x00ee,WAYPOINT_ACT4_3,"river of flame" },
//27+3=30
	{ 0,true,true,true ,{0,0},0x01ad,WAYPOINT_ACT5_1,"harrogath" },
	{ 0,true,true,false,{0,0},0x01f0,WAYPOINT_ACT5_2,"frigid highlands" },
	{ 0,true,true,false,{0,0},0x01ff,WAYPOINT_ACT5_3,"arreat plateau" },
	{ 0,true,true,false,{0,0},0x01ff,WAYPOINT_ACT5_4,"crystalline passage" },
	{ 0,true,true,false,{0,0},0x01ff,WAYPOINT_ACT5_5,"glacial trail" },
	{ 0,true,true,false,{0,0},0x01f0,WAYPOINT_ACT5_6,"halls of pain" },
	{ 0,true,true,false,{0,0},0x01f0,WAYPOINT_ACT5_7,"frozen tundra" },
	{ 0,true,true,false,{0,0},0x01ff,WAYPOINT_ACT5_8,"the ancients way" },
	{ 0,true,true,false,{0,0},0x01ee,WAYPOINT_ACT5_9,"worldstone keep level 2" },
	{ 2,false,true,false,{0,0},0x003c,PORTAL_ACT5_1,"nihlathaks temple" },
	{ 2,false,true,true,{0,0},0x003c,PORTAL_ACT5_2,"harrogath portal" },
//  {entityID,isaWP,isStale,isTown,{pos},lid,wpNum,name}
//30+12=42
};


BOOL isWP(WORD lid) {
	for(int i = 0; i < WAYPOINT_TOTAL; i++) {
		if(lid == allWP[i].lid) {
			return true;
		}
	}
	return false;
}

WPSTRUCT *getWPbyName(char *name) {
	
	for(int i = 0; i < WAYPOINT_TOTAL;i++) {
		if(strcmp(name, allWP[i].name) == 0) { // name matches
			return (&allWP[i]);
		}
	}
	return NULL;
}
