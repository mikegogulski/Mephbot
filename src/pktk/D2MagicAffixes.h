// D2MagicAffixes.h: interface for the D2MagicAffixes class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "MString.h"

#include "D2Items.h"

// the statID for some damge statIDs
#define STATS_ITEM_MAXDAMAGE_PERCENT	17
#define STATS_SECONDARY_MINDAMAGE		23
#define STATS_SECONDARY_MAXDAMAGE		24

#define STATID_BITLENGTH				9
#define DMGPERCENT_BITLENGTH			9
#define NUM_STATIDS						323


struct D2MagicAffixes  
{
	char*			m_Name ;
	int				m_Version ;
	int				m_LevelReq ;
	CItemMods		m_ItemMods[3] ;
};
extern struct D2MagicAffixes gMagicPrefix[] ;
extern struct D2MagicAffixes gMagicSuffix[] ;


struct D2MagicStats  
{
	int				m_ID ;
	int				m_SaveBits ;
	int				m_SaveAdd ;
//cn-just added
	char*			m_Desc ;
	BOOL			m_PrintDebug ;
};
extern struct D2MagicStats gMagicStats[] ;


struct D2RareNames  
{
	char*			m_Name ;
};
extern struct D2RareNames gRarePrefix[] ;
extern struct D2RareNames gRareSuffix[] ;


class CMagicMods
{
public:
	BOOL		m_isPrefix ;
	int			m_Modifier ;
	CItemMods	m_ItemMods[3] ;
//	int			m_iNumParams ;

	CMagicMods() ;
} ;

