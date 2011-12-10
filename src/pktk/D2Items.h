// D2Items.h: interface for the CWeapon class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "MString.h"

struct CItemMods
{
	char*			m_ItemModCode ;
	int				m_ItemModParam ;
	int				m_ItemModMin ;
	int				m_ItemModMax ;
} ;



// NOTE: I would love to use C++ style classes, but explicit data initialization is not
//		allowed.  Therefore I can only use struct :(



//"name",minac,maxac,reqstr,block,durability,level,cost,"code",component,invwidth,invheight,
//		gemapplytype,"invfile","uniqueinvfile","setinvfile","type",bitfield1},
struct CArmor
{
	char*			m_Name ;
	unsigned int	m_MinAC ;
	unsigned int	m_MaxAC ;
	unsigned int	m_ReqStr ;
	unsigned int	m_Block ;
	unsigned int	m_Durability ;
	unsigned int	m_Level ;
	unsigned int	m_Cost ;
	char*			m_Code ;
	unsigned int	m_Component ;
	unsigned int	m_InvWidth ;
	unsigned int	m_InvHeight ;
	unsigned int	m_GemApplyType ;
	char*			m_InvFile ;
	char*			m_UniqueInvFile ;
	char*			m_SetInvFile ;
	char*			m_Type ;
	unsigned int	m_Bitfield1 ;
};
extern struct CArmor gArmorItems[] ;


//{"name","type","code",mindam,maxdam,1or2handed,2handed,2handmindam,2handmaxdam,minmisdam,
//	maxmisdam,speed,StrBonus,DexBonus,reqstr,reqdex,nodurability,level,cost,"wclass",
//	"2handedwclass",component,"hit class",invwidth,invheight,stackable,"invfile",
//	"uniqueinvfile","setinvfile","special",bitfield1},
struct CWeapon
{
	char*			m_Name ;
	char*			m_Type ;
	char*			m_Code ;
	unsigned int	m_MinDam ;
	unsigned int	m_MaxDam ;
	BOOL			m_1or2Handed ;
	BOOL			m_2Handed ;
	unsigned int	m_2HandMinDam ;
	unsigned int	m_2HandMaxDam ;
	unsigned int	m_MinMisDam ;
	unsigned int	m_MaxMisDam ;
	unsigned int	m_Speed ;
	unsigned int	m_StrBonus ;
	unsigned int	m_DexBonus ;
	unsigned int	m_ReqStr ;
	unsigned int	m_ReqDex ;
	BOOL			m_NoDurability ;
	unsigned int	m_Level ;
	unsigned int	m_Cost ;
	char*			m_WClass ;
	char*			m_2HandedWClass ;
	unsigned int	m_Component ;
	char*			m_HitClass ;
	unsigned int	m_InvWidth ;
	unsigned int	m_InvHeight ;
	BOOL			m_Stackable ;
	char*			m_InvFile ;
	char*			m_UniqueInvFile ;
	char*			m_SetInvFile ;
	char*			m_Special ;
	unsigned int	m_Bitfield1 ;
};
extern struct CWeapon gWeaponItems[] ;


//{"name",levelreq,cost,"code",component,invwidth,invheight,"invfile","type",stackable,
//		bitfield1},
struct CMisc
{
	char*			m_Name ;
	unsigned int	m_LevelReq ;
	unsigned int	m_Cost ;
	char*			m_Code ;
	unsigned int	m_Component ;
	unsigned int	m_InvWidth ;
	unsigned int	m_InvHeight ;
	char*			m_InvFile ;
	char*			m_Type ;
	BOOL			m_Stackable ;
	unsigned int	m_Bitfield1 ;
};
extern struct CMisc gMiscItems[] ;



CArmor* GetArmorRec( CString sCode) ;
CWeapon* GetWeaponRec( CString sCode) ;
CMisc* GetMiscRec( CString sCode) ;


