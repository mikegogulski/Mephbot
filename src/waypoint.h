// waypoint.h ackmed@gotwalls.com
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "miscstructs.h"

#define WAYPOINT_ACT1_1		0x01
#define WAYPOINT_ACT1_2		0x03
#define WAYPOINT_ACT1_3		0x04
#define WAYPOINT_ACT1_4		0x05
#define WAYPOINT_ACT1_5		0x06
#define WAYPOINT_ACT1_6		0x1b
#define WAYPOINT_ACT1_7		0x1d
#define WAYPOINT_ACT1_8		0x20
#define WAYPOINT_ACT1_9		0x23

#define WAYPOINT_ACT2_1		0x28
#define WAYPOINT_ACT2_2		0x30
#define WAYPOINT_ACT2_3		0x2a
#define WAYPOINT_ACT2_4		0x39
#define WAYPOINT_ACT2_5		0x2b
#define WAYPOINT_ACT2_6		0x2c
#define WAYPOINT_ACT2_7		0x34
#define WAYPOINT_ACT2_8		0x4a
#define WAYPOINT_ACT2_9		0x2e

#define WAYPOINT_ACT3_1		0x4b
#define WAYPOINT_ACT3_2		0x4c
#define WAYPOINT_ACT3_3		0x4d
#define WAYPOINT_ACT3_4		0x4e
#define WAYPOINT_ACT3_5		0x4f
#define WAYPOINT_ACT3_6		0x50
#define WAYPOINT_ACT3_7		0x51
#define WAYPOINT_ACT3_8		0x53
#define WAYPOINT_ACT3_9		0x65

#define WAYPOINT_ACT4_1		0x67	// the pandemonium Fortress
#define WAYPOINT_ACT4_2		0x6a	// city of the damned
#define WAYPOINT_ACT4_3		0x6b	// river of flame

#define WAYPOINT_ACT5_1		0x6d	// harrogath
#define WAYPOINT_ACT5_2		0x6f	// frigid highlands
#define WAYPOINT_ACT5_3		0x70
#define WAYPOINT_ACT5_4		0x71
#define WAYPOINT_ACT5_5		0x73
#define WAYPOINT_ACT5_6		0x7b
#define WAYPOINT_ACT5_7		0x75
#define WAYPOINT_ACT5_8		0x76
#define WAYPOINT_ACT5_9		0x81
#define PORTAL_ACT5_1		0x1c
#define PORTAL_ACT5_2		0x1d


#define WAYPOINT_TOTAL		41
// structs


typedef struct waypoint_t {
	DWORD			entityID;
	BOOL			isaWP;
	BOOL			isStale;
	BOOL			isTown;
	LOCATIONSTRUCT	pos;
	WORD			lid;
	WORD			wpNum;
	char			name[32];
} WPSTRUCT;

extern WPSTRUCT allWP[];

BOOL isWP(WORD lid);
WPSTRUCT *getWPbyName(char *name);