// D2Uniques.h: interface for the CWeapon class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "MString.h"

#include "D2Items.h"

struct CUnique
{
	char*			m_Name ;
	char*			m_Type ;
	char*			m_Code ;
	int				m_LevelReq ;
	CItemMods		m_ItemMods[10] ;
};
extern struct CUnique gUniqueItems[] ;

CUnique* GetUniqueRec( CString sCode) ;
