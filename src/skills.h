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
#include "mephbot.h"

//{ "skill", Id, Delay, FastCast },
typedef struct skillstruct_t {
	char *Name;
	int Id;					// skill number
	int Delay;				// frames between completion of cast and beginning of next
	BOOL fFastCast;			// Affected by fast cast?
} SKILLSTRUCT;

#define	MAX_SKILLS	31

#define SKILL_RIGHT					0
#define SKILL_LEFT					1

extern SKILLSTRUCT Skills[MAX_SKILLS];
extern int CurrentSkill[];

extern int GetSkillIndexByName(char *);
extern int GetSkillIndexById(int);
extern char *GetSkillNameById(int);
extern VOID SelectSkillByName(char *, int);
extern VOID CastSkillOnSelfByName(char *);
extern VOID CastSkillOnLocationByName(char *, int, WORD, WORD);
extern int FastCastPctToTickDelay(int);
extern int SkillDelayTicks(char *, int);
extern int SkillTimerTicks(char *);
