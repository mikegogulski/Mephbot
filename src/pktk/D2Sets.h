// D2Sets.h: interface for the CWeapon class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "MString.h"

#include "D2Items.h"

#define ITEMS_PER_SET	6


struct CSetItems
{
	char*			m_Code ;
	char*			m_Name ;
	int				m_LevelReq ;
	CItemMods		m_ItemMods[7] ;
};

struct CSet
{
	char*			m_SetName ;
	int				m_LevelReq ;
	int				m_NumItems ;
	CSetItems		m_SetItems[ITEMS_PER_SET] ;
	CItemMods		m_PartialSetMods[4] ;
	CItemMods		m_FullSetMods[6] ;
};
extern struct CSet gSetItems[] ;

CSetItems* GetSetItemRec( CString sCode) ;

