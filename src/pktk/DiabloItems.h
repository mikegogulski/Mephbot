// DiabloItems.h : interface of the CInvDumpView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "MString.h"

#include "D2Items.h"
#include "D2MagicAffixes.h"
#include "Mouse.h"

/*
9c 00 = monster drop and come within sight range notice
9c 01 = me pick up from ground and float
9c 03 = stuff is here on ground when you come into range of it
*/

/*
*00 = new item on ground (from monster drop or walk with in range)
*01 = pick up to cursor 
02 = player (or me) dropped item to ground
*03 = old item on ground 
04 = put into inventory (including stash) 
05 = me pick from inv							// 9D
06 = player wearing item						// 9D
0B = in store 
0C = removed from store (e.g., when you buy an item) 
0E = put into belt 
*/

#define MSG_NEWITEM		0x00
#define MSG_TOCURSOR	0x01
#define MSG_PDROPPED	0x02
#define MSG_OLDITEM		0x03
#define MSG_TOINV		0x04	//9C		// this is cube and stash too
#define MSG_FROMINV		0x05
#define MSG_TOWORN		0x06	//9D
//7 - something to do with trading?
#define MSG_FROMWORN	0x08
#define MSG_SWAPWORN	0x09	//9D	//swap is to drop one item on top of another
//A
#define MSG_INSTORE		0x0B
#define MSG_BOUGHT		0x0C
#define	MSG_SWAPINV		0x0D
#define MSG_TOBELT		0x0E
#define MSG_FROMBELT	0x0F
#define MSG_SWAPBELT	0x10
//11
#define MSG_FROMMERC	0x12
#define MSG_TOSOCKET	0x13	//9D
//14
#define MSG_AUTOPOS		0x15		// when the pots slide down; when item fills socket
									// also done when identified
//16
#define MSG_WEAPSWITCH	0x17	//9D



// NPC drop item????  figure out act 5 quests; imbue, socket, personalize



// this field matches the "component" field in the 3 txt files
//weapons.txt
#define TYPE_WEAPON		0x05
//cn ???
// damn!!!  The buckler I started with on TestAss is 0x06!?!?!
// must have to do with holding the item or something?
#define TYPE_BOW		0x06	// xbows are 0x05
//armor.txt
#define TYPE_HELM		0x00
#define TYPE_ARMOR		0x01
#define TYPE_SHIELD		0x07
#define TYPE_PHEAD		0x0A	// only for necro's perserved heads
//armor.txt -OR- misc.txt
#define TYPE_MISC		0x10	// have to use the isMisc flag to determine if armor or misc
//	torch=0x07 in misc.txt, but is unused



// these are in the 9th byte of the 9D message and state what the ownerid is for
#define OACT_PLAYER		0x00		// normal for what player's are wearing
#define OACT_MERC		0x01		// my merc has this item (can't know other mercs :(  )
#define OACT_INSOCK		0x04		// ownerid is for the item that this item is socketed in



// general location of item
#define	POS_STORED		0x00
#define	POS_WORN		0x01
#define	POS_BELT		0x02
#define	POS_GROUND		0x03
#define	POS_TOHAND		0x04
// 5??
#define	POS_INSOCKET	0x06



// where on the body this is being worn
#define BODY_NOTWORN	0x00
#define BODY_HEAD		0x01
#define BODY_AMMY		0x02
#define BODY_BODY		0x03
#define BODY_WEAPON		0x04
#define BODY_SHEILD		0x05
#define BODY_LRING		0x06
#define BODY_RRING		0x07
#define BODY_BELT		0x08
#define BODY_BOOT		0x09
#define BODY_GLOVES		0x0A
#define BODY_WEAPON2	0x0B
#define BODY_SHEILD2	0x0C



// if player has item, more specific as to where
#define ITEM_WEARING	0x00		// includes pots in belt
#define ITEM_INV		0x01
#define ITEM_CUBE		0x04
#define ITEM_STASH		0x05



//CN - does that gem thing really work?  what about crafted?  personalized?
// quality of the item
#define	QUAL_CRAFTED	0x00
#define	QUAL_INFERIOR	0x01
#define	QUAL_NORMAL		0x02
#define	QUAL_SUPERIOR	0x03
#define	QUAL_MAGIC		0x04
#define	QUAL_SET		0x05
#define	QUAL_RARE		0x06
#define	QUAL_UNIQUE		0x07
/*
00 = AR 
01 = Max Dmg 
02 = AC 
03 = AR + Max Dmg 
04 = Durability 
05 = Durability + AR 
06 = Durability + Max Dmg 
07 = Durability + AC 
*/


// screen coordinates for picture placement
#define PICPOSLEN			29			// the length in X or Y of one square

//PKTK
#define PICPOS_PKTK_HEADX		91
#define PICPOS_PKTK_HEADY		5
#define PICPOS_PKTK_BODYX		91
#define PICPOS_PKTK_BODYY		76
#define PICPOS_PKTK_BELTX		91
#define PICPOS_PKTK_BELTY		177
#define PICPOS_PKTK_WEAPONX		12
#define PICPOS_PKTK_WEAPONY		9
#define PICPOS_PKTK_HANDX		12
#define PICPOS_PKTK_HANDY		149
#define PICPOS_PKTK_SHIELDX		171
#define PICPOS_PKTK_SHIELDY		9
#define PICPOS_PKTK_FOOTX		171
#define PICPOS_PKTK_FOOTY		149
#define PICPOS_PKTK_RINGLEFTX	64
#define PICPOS_PKTK_RINGLEFTY	217
#define PICPOS_PKTK_AMULETX		104
#define PICPOS_PKTK_AMULETY		217
#define PICPOS_PKTK_RINGRIGHTX	142
#define PICPOS_PKTK_RINGRIGHTY	217



//INV-MANAGER
#define PICPOS_INVX			242
#define PICPOS_INVY			297
#define PICPOS_CUBEX		612
#define PICPOS_CUBEY		168
#define PICPOS_STASHX		24
#define PICPOS_STASHY		11
#define PICPOS_INBELTX		612
#define PICPOS_INBELTY		392

#define PICPOS_HEADX		356
#define PICPOS_HEADY		6
#define PICPOS_BODYX		356
#define PICPOS_BODYY		77
#define PICPOS_BELTX		356
#define PICPOS_BELTY		219
#define PICPOS_WEAPONX		259
#define PICPOS_WEAPONY		39
#define PICPOS_HANDX		261
#define PICPOS_HANDY		179
#define PICPOS_SHIELDX		451
#define PICPOS_SHIELDY		39
#define PICPOS_FOOTX		451
#define PICPOS_FOOTY		179
#define PICPOS_RINGLEFTX	332
#define PICPOS_RINGLEFTY	179
#define PICPOS_AMULETX		370
#define PICPOS_AMULETY		179
#define PICPOS_RINGRIGHTX	410
#define PICPOS_RINGRIGHTY	179


#define PICPOS_INBELTX		612
#define PICPOS_INBELTY		392

#define PICPOS_WEAPON2X		564
#define PICPOS_WEAPON2Y		39
#define PICPOS_SHIELD2X		673
#define PICPOS_SHIELD2Y		39

#define PICPOS_MERCHEADX	88
#define PICPOS_MERCHEADY	255
#define PICPOS_MERCBODYX	87
#define PICPOS_MERCBODYY	327
#define PICPOS_MERCWEAPONX	14
#define PICPOS_MERCWEAPONY	298
#define PICPOS_MERCSHIELDX	156
#define PICPOS_MERCSHIELDY	298


#define PICPOS_MAXWEAPONX	56
#define PICPOS_MAXWEAPONY	112
#define PICPOS_MAXBELTY		28

#define	ITEMOWNER_FAILED	0
#define	ITEMOWNER_PLAYER	1
#define	ITEMOWNER_GROUND	2
#define	ITEMOWNER_ERROR		3



// this class stores the info to place items pics in the proper spots
class CDiabloItems
{
public:

	// sniffed from the packet
	unsigned char	m_sPacketMsg[100] ;
	BitFields	m_ItemBits ;

	CString		m_sItemName ;

	DWORD		m_bMessageID,		// 9C or 9D
				m_bActionID,		// what are we doing with this item
				m_bMessageLen,		// bytes in the message
				m_bComponent,		// the basic type of item so we can look in the right file
				m_bWhere,			// wearing/belt/stash/inventory/cube
				m_bBodyCode,		// body code
				m_bItemID,			// the ID of the item (for this game only)
				m_bPosX,			// really just small ints that say where in the cube/stash/inv
				m_bPosY,
				m_bQuality,			// normal/rare/unique ect.
				m_bQuantity,		// number of arrows or scrolls in tomb, ect.
				m_bDurability,
				m_bDurabilityMax,
				m_bPlayerID ;		// pid; will be item# for a socketed gem


	// the rest is for displaying the item


	CString		m_sFileName ;		// of the picture for this item
	int			m_iLeft ;			// coordinates to draw this item
	int			m_iTop ;
	int			m_iRight ;
	int			m_iBottom ;

//	CPtrList	m_InfoLines ;	// lines for the info box

	int			m_iOwnerType ;		// on player, on ground, unknown
	int			m_iSocketsPlaced ;
//	CPtrList	m_pSocketItems ;


//cn - new
	//FLAGS
	BOOL		m_isMiscItem ;
	BOOL		m_isEar ;
	BOOL		m_isInSocket ;
	BOOL		m_isSocketed ;
	BOOL		m_hasMagicStats ;
	BOOL		m_isIdentified ;
	BOOL		m_isPersonalized ;
	BOOL		m_isRuneWord ;

	BOOL		m_isMercItem ;
	BOOL		m_isStackable ;
	BOOL		m_hasDurability ;
	BOOL		m_isIndestructable ;

	CString		m_sCode ;
	CString		m_sBaseName ;

	DWORD		m_iDefense ;
	DWORD		m_iDurabilityMax ;
	DWORD		m_iDurabilityCur ;
	DWORD		m_iQuantity ;
	int			m_iUsedSockets ;
	int			m_iNumSockets ;

	CString		m_ModDesc[25] ;
	int			m_iNumModDesc ;

	CString		m_sSpecialName ;


	CArmor*		m_pArmor ;
	CWeapon*	m_pWeapon ;
	CMisc*		m_pMisc ;




	// stores debug information in case something about this item pops up
	BOOL		m_bDebug ;
//	CStringList	m_sDebugTriggers ;

public:
	CDiabloItems() ;
	~CDiabloItems() ;
	void LoadItemAttributes( const unsigned char *pkt_data) ;
	void AddInfoLine( CString sText, int iColor) ;
	CString PrintItemHex() ;
	void PrintDebug(CString sDebugLine, BOOL bTriggerDebug=TRUE) ;
	void DumpDebugInfo() ;
	
protected:
//	CPoint getCoordinates() ;

	void CommonByteParsing() ;
	void ParseFlags() ;
	void ParsePos() ;

	void PrintEarInfo() ;
	void GetBaseItem() ;
	void GetBaseAttributes() ;
	void GetImage() ;


	void GetMiscAttributes() ;
	void GetInferiorAttributes() ;
	void GetSuperiorAttributes() ;
	void GetMagicAttributes() ;
	void GetSetAttributes() ;
	void GetRareAttributes() ;
	void GetUniqueAttributes() ;

	BOOL GetDurability() ;
	void GetMagicValues() ;
} ;



CDiabloItems* parseItem( const unsigned char *pkt_data) ;


