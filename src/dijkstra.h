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
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// 512 produces a stack overflow on object initialization
#define MAX			256		// maximum number of units in Durance 2 after taking waypoint
#define INFINITY	INT_MAX


typedef struct point_t {
	DWORD x;
	DWORD y;
} myPOINT;


#define PLAYER	0			// need to make sure we initalize the array with these first
#define STAIRS	1

#define DEBUG_DIJKSTRA	DEBUG_SOLVE

extern myPOINT Points[];

extern WORD StairsPath[][2];
extern BYTE bStairsPathIndex;
extern BYTE bPointsIndex;
extern int HopsToStairs;

extern VOID FindStairsPath(VOID);