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
#include "skills.h"
#include "util.h"
#include "packethelper.h"

//{ "skill", Id, Delay (frames), FastCast },
SKILLSTRUCT Skills[MAX_SKILLS] = {
	{ "INVALID SKILL ERROR", 0, 0, FALSE },
	{ "Attack", 0, 0, FALSE },
	{ "Throw", 2, 0, FALSE },
	{ "Left Hand Throw", 4, 0, FALSE },
	{ "Left Hand Swing", 5, 0, FALSE },
	{ "Fire Bolt", 36, 0, TRUE },
	{ "Charged Bolt", 38, 0, TRUE },
	{ "Ice Bolt", 39, 0, TRUE },
	{ "Frozen Armor", 40, 0, TRUE },
	{ "Inferno", 41, 0, FALSE },
	{ "Static Field", 42, 0, TRUE },
	{ "Telekinesis", 43, 0, TRUE },
	{ "Frost Nova", 44, 0, TRUE },
	{ "Ice Blast", 45, 0, TRUE },
	{ "Blaze", 46, 0, TRUE },
	{ "Fire Ball", 47, 0, TRUE },
	{ "Nova", 48, 0, TRUE },
	{ "Lightning", 49, 0, FALSE },
	{ "Shiver Armor", 50, 0, TRUE },
	{ "Fire Wall", 51, 35, TRUE },
	{ "Enchant", 52, 0, TRUE },
	{ "Chain Lightning", 53, 0, FALSE },
	{ "Teleport", 54, 0, TRUE },
	{ "Glacial Spike", 55, 0, TRUE },
	{ "Meteor", 56, 50, TRUE },
	{ "Thunder Storm", 57, 0, TRUE },
	{ "Energy Shield", 58, 0, TRUE },
	{ "Blizzard", 59, 45, TRUE },
	{ "Chilling Armor", 60, 0, TRUE },
	{ "Hydra", 62, 50, TRUE },
	{ "Frozen Orb", 64, 25, TRUE } };

/*
Casting speed table for the sorceress
from http://www.planetdiablo.com/library/concept-castingspeed.htm

Fast Cast %		0-8%	9-19%	20-36%	37-62%	63-104%	105-199%	200+% 
Frames Per Cast	13		12		11		10		9		8			7 
*/

int FastCastPctToFpc(int pct) {
	if (pct <= 8)
		return 13;	//.52 sec, 5.2 ticks	6 ticks
	if (pct <= 19)
		return 12;	//.48 sec, 4.8 ticks	5 ticks
	if (pct <= 36)
		return 11;	//.44 sec, 4.4 ticks	5 ticks
	if (pct <= 62)
		return 10;	//.40 sec, 4.0 ticks	4 ticks
	if (pct <= 104)
		return 9;	//.36 sec, 3.6 ticks	4 ticks
	if (pct <= 199)
		return 8;	//.32 sec, 3.2 ticks	4 ticks
	if (pct >= 200)
		return 7;	//.28 sec, 2.8 ticks	3 ticks
	// NOTREACHED
	return 13; }

int FpcToTickDelay(int fpc) {
	return (int)(10 * fpc / 25); }

int FastCastPctToTickDelay(int pct) {
	return FpcToTickDelay(FastCastPctToFpc(pct)); }

int SkillDelayTicks(char *Skill, int fastcastpct) {
	int skillindex = GetSkillIndexByName(Skill);
	if (Skills[skillindex].fFastCast) {
		return FpcToTickDelay(FastCastPctToFpc(fastcastpct)); }
	else {
		return FpcToTickDelay(FastCastPctToFpc(0)); } }

int SkillTimerTicks(char *Skill) {
	return FpcToTickDelay(Skills[GetSkillIndexByName(Skill)].Delay); }

int CurrentSkill[2] = {0, 0};

// allow "Frozen Orb", "FrozenOrb", "frozen orb", "fRoZENorb", etc.
int GetSkillIndexByName(char *name) {
	char tmp[64], tmp1[64];
	for (int i = 0; i < MAX_SKILLS; i++) {
		StripWhitespace(tmp, Skills[i].Name);
		StripWhitespace(tmp1, name);
		if (!stricmp(tmp1, tmp)) {
			return i; }	}
	return 0; }

int GetSkillIndexById(int id) {
	for (int i = 0; i < MAX_SKILLS; i++) {
		if (Skills[i].Id == id) {
			return i; } }
	return 0; }

char *GetSkillNameById(int id) {
	for (int i = 0; i < MAX_SKILLS; i++) {
		if (Skills[i].Id == id) {
			return Skills[i].Name; } }
	return NULL; }

VOID SelectSkillByName(char *name, int hand) {
	SelectSkill(Skills[GetSkillIndexByName(name)].Id, hand);
	return; }

VOID CastSkillOnSelfByName(char *name) {
	CastSkillOnLocationByName(name, SKILL_RIGHT, PLAYERX, PLAYERY);
	return; }

VOID CastSkillOnLocationByName(char *name, int hand, WORD x, WORD y) {
	if (strlen(name) > 2) {
		SelectSkillByName(name, hand);
		CastSkillOnLocation(hand, x, y); }
	return; }
