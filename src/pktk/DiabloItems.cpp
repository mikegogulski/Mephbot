// DiabloItems.cpp
//
/////////////////////////////

#include "MString.h"


#include "DiabloItems.h"

#include "D2MagicAffixes.h"
#include "D2Uniques.h"
#include "D2Sets.h"
#include "../logging.h"



CDiabloItems::CDiabloItems()
{
	m_iSocketsPlaced = 0 ;

	m_bDebug = FALSE ;
	m_isMercItem = FALSE ;
	m_isStackable = FALSE ;
	m_hasDurability = FALSE ;
	m_isIndestructable = FALSE ;

	m_iNumModDesc = 0 ;

	m_pArmor = NULL ;
	m_pWeapon = NULL ;
	m_pMisc = NULL ;

	m_iDefense = 0 ;
	m_iDurabilityCur = 0 ;
	m_iDurabilityMax = 0 ;
	m_iQuantity = 0 ;
	m_iUsedSockets = 0 ;
	m_iNumSockets = 0 ;
}


CDiabloItems::~CDiabloItems()
{
}


// print the bytes that make up an item
CString CDiabloItems::PrintItemHex()
{
	CString sOutput, sTmp ;
	for (DWORD count=0; count<m_bMessageLen; count++) {
		if (m_sPacketMsg[count] < 0x10)
			sOutput += "0" ;
		sTmp.Format( "%X ", m_sPacketMsg[count]);
		sOutput += sTmp ;
	}
	return sOutput ;
}

void CDiabloItems::CommonByteParsing()
{
	m_bMessageID =	m_sPacketMsg[ 0] ;
	m_bActionID =	m_sPacketMsg[ 1] ;
	m_bMessageLen =	m_sPacketMsg[ 2] ;
	m_bComponent =	m_sPacketMsg[ 3] ;
	// 4,5,6,7
	memcpy(&m_bItemID,&m_sPacketMsg[ 4],sizeof m_bItemID);	// m_bItemID
	
	//cn
	// for 9C, I'm thinking there is no "owner id"
	// for 9D, the first byte is some kind of action and the next 4 are the owner ID
	//		at least the next 2 are any way (probably 4 though)
	//	04=socketed; 00=wearing; 01=merc wearing (only for your own merc *sigh*)
	
	
	// if it's 9C, then it is obvious who the owner is; for 9D, get the owner
	if ( m_bMessageID == 0x9D) {
		DWORD	bOwnerAction = m_sPacketMsg[8] ;
		memcpy(&m_bPlayerID,&m_sPacketMsg[ 9],sizeof m_bPlayerID);	// m_bItemID
		m_isMercItem = ( bOwnerAction == OACT_MERC) ;
	}

}


void CDiabloItems::ParseFlags()
{
// etheral unrepairable?

	// flags
	BOOL isMeWearing =	m_ItemBits.GetField(1, "isMeWearing") ;		// 64
	BOOL isJustBought =	m_ItemBits.GetField(1, "isJustBought") ;	// 65
	if (m_ItemBits.GetField(1)){} //PrintDebug("U1 != 0") ;						// 66
	m_isInSocket =		m_ItemBits.GetField(1, "isInSocket") ;		// 67
	m_isIdentified =	m_ItemBits.GetField(1, "isIdentified") ;	// 68
	if (m_ItemBits.GetField(1)){} // PrintDebug("WAS ISETHERAL") ;				// 69
	BOOL isSwitchIn =	m_ItemBits.GetField(1, "isSwitchIn");		// 70
	BOOL isSwitchOut =	m_ItemBits.GetField(1, "isSwitchOut");		// 71
	BOOL isBroken =		m_ItemBits.GetField(1, "isBroken") ;		// 72
	if (m_ItemBits.GetField(1)){} // PrintDebug("U2 != 0") ;						// 73
//cn
	BOOL PotionX =		m_ItemBits.GetField(1, "PotionX") ;			// 74
	m_isSocketed =		m_ItemBits.GetField(1, "isSocketed") ;		// 75
	if (m_ItemBits.GetField(1)){} // PrintDebug("U3 != 0") ;						// 76
	BOOL isJustGen =	m_ItemBits.GetField(1, "isJustGen") ;		// 77
// was 1 on Thudergod's Vigor; could have something to do with 1.08 vs 1.09?
//cn - set back to true after the release
	if (m_ItemBits.GetField(2)){} // PrintDebug("U4 != 0", FALSE) ;				// 78,79
	m_isEar =			m_ItemBits.GetField(1, "isEar") ;			// 80
	BOOL isStartItem =	m_ItemBits.GetField(1, "isStartItem") ;		// 81
	if (m_ItemBits.GetField(3)){} // PrintDebug("U5 != 0") ;						// 82-84
	m_isMiscItem =		m_ItemBits.GetField(1, "isMiscItem") ;		// 85
	BOOL isEtheral =	m_ItemBits.GetField(1, "isEtheral") ;		// 86
	m_hasMagicStats =	m_ItemBits.GetField(1, "hasMagic") ;		// 87
	m_isPersonalized =	m_ItemBits.GetField(1, "isPersonal") ;		// 88
	BOOL isGamble =		m_ItemBits.GetField(1, "isGamble") ;		// 89
// this could be isRuneWord or something (the first bit)
	m_isRuneWord =		m_ItemBits.GetField(1, "isRuneWord") ;		// 90
	if (m_ItemBits.GetField(5)){} // PrintDebug("U8 != 0") ;						// 90-95

	if (m_ItemBits.GetField(10)){} // PrintDebug("the 0,1,100 field", FALSE) ;
}


void CDiabloItems::ParsePos()
{
	DWORD locationType = m_ItemBits.GetField(3, "locationType");					// 106-108	



	// on ground or something, not a player's or in a shop
	if ((locationType == POS_GROUND) || (locationType == 5)) {
		DWORD areaX = m_ItemBits.GetField(16, "areaX");								// 109-124
		DWORD areaY = m_ItemBits.GetField(16, "areaY");								// 125-140


	// mine, someone else's, in a socket, or a shop item
	} else {

		m_bBodyCode = m_ItemBits.GetField(4, "m_bBodyCode");				// 109-112


		m_bPosX = m_ItemBits.GetField(4, "m_bPosX");						// 113-116
		m_bPosY = m_ItemBits.GetField(4, "m_bPosY");						// 117-120
		m_bWhere = m_ItemBits.GetField(3, "m_bWhere");					// 121-123	




	}
}


void CDiabloItems::GetBaseItem()
{
	// item code
	DWORD itemcode = m_ItemBits.GetField(32, "itemcode");					// 124-155	// 141-172
//	dprintf(DEBUG_ALL, "itemcode = %X", itemcode);
	char stmp[5] ;
	sprintf( stmp, "%.4s", &itemcode) ;

	char tmp[80] ;
	sprintf( tmp, "%.4s", &itemcode) ;
	m_sCode = stmp ;
	m_sCode.Remove(' ') ;



	// got the code, now find the item name and other info from the MPQ tables
	BOOL	bGotOne = FALSE ;


	// head, body, shield, necro's preserved heads
	if (( m_bComponent == TYPE_HELM) || ( m_bComponent == TYPE_ARMOR) ||
		( m_bComponent == TYPE_SHIELD) || ( m_bComponent == TYPE_PHEAD)) {
		m_pArmor = GetArmorRec( m_sCode) ;
		bGotOne = m_pArmor != NULL ;

	// any kind of weapon, including thrown potions
	} else if (( m_bComponent == TYPE_WEAPON) || ( m_bComponent == TYPE_BOW)) {
		m_pWeapon = GetWeaponRec( m_sCode) ;
		bGotOne = m_pWeapon != NULL ;

	// belts, boots, gloves (the rest of armors.txt and most of misc.txt)
	} else if (( m_bComponent == TYPE_MISC) && (!m_isMiscItem)) {
		m_pArmor = GetArmorRec( m_sCode) ;
		bGotOne = m_pArmor != NULL ;
	// all the things in misc.txt
	} else {
		m_pMisc = GetMiscRec( m_sCode) ;
		bGotOne = m_pMisc != NULL ;
	}


	// hmmmm... didn't hit above, so do it the hard way; you would think this is not needed,
	//	but sometimes the components are wrong... I've seen shields with TYPE_BOW :(
	if ( !bGotOne) {

		m_pMisc = GetMiscRec( m_sCode) ;
		bGotOne = m_pMisc != NULL ;

		if (!bGotOne) {
			m_pArmor = GetArmorRec( m_sCode) ;
			bGotOne = m_pArmor != NULL ;
		}

		if (!bGotOne) {
			m_pWeapon = GetWeaponRec( m_sCode) ;
			bGotOne = m_pWeapon != NULL ;
		}
	}
}


void CDiabloItems::GetBaseAttributes()
{
	// set some params now that we know what we have

	// armors.txt
	if ( m_pArmor != NULL) {
		m_sBaseName = m_pArmor->m_Name ;
		m_hasDurability = TRUE ;
		m_sFileName.Format("%s.jpg", m_pArmor->m_InvFile) ;
		m_iBottom = m_pArmor->m_InvHeight * PICPOSLEN ;
		m_iRight = m_pArmor->m_InvWidth * PICPOSLEN ;
		if (m_bQuality == QUAL_UNIQUE) {
			if ( m_pArmor->m_UniqueInvFile != "")
				m_sFileName.Format("%s.jpg", m_pArmor->m_UniqueInvFile) ;
		} else if ( m_bQuality == QUAL_SET) {
			if ( m_pArmor->m_SetInvFile != "")
				m_sFileName.Format("%s.jpg", m_pArmor->m_SetInvFile) ;
		}

	// weapons.txt
	} else if (m_pWeapon != NULL) {
		m_sBaseName = m_pWeapon->m_Name ;
		m_isStackable = m_pWeapon->m_Stackable ;
		m_hasDurability = !m_pWeapon->m_NoDurability ;
		m_sFileName.Format("%s.jpg", m_pWeapon->m_InvFile) ;
		m_iBottom = m_pWeapon->m_InvHeight * PICPOSLEN ;
		m_iRight = m_pWeapon->m_InvWidth * PICPOSLEN ;
		if (m_bQuality == QUAL_UNIQUE) {
			if ( m_pWeapon->m_UniqueInvFile != "")
				m_sFileName.Format("%s.jpg", m_pWeapon->m_UniqueInvFile) ;
		} else if ( m_bQuality == QUAL_SET) {
			if ( m_pWeapon->m_SetInvFile != "")
				m_sFileName.Format("%s.jpg", m_pWeapon->m_SetInvFile) ;
		}

	// misc.txt
	} else {
		m_sBaseName = m_pMisc->m_Name ;
		m_isStackable = m_pMisc->m_Stackable ;
		m_sFileName.Format("%s.jpg", m_pMisc->m_InvFile) ;
		m_iBottom = m_pMisc->m_InvHeight * PICPOSLEN ;
		m_iRight = m_pMisc->m_InvWidth * PICPOSLEN ;
	}
}


void CDiabloItems::GetMiscAttributes()
{
	// if it had the misc item flag, it is really a junky misc item or gold
	m_bQuality = 1 ;
	if ( m_sBaseName == "gold") {
		DWORD goldsize = m_ItemBits.GetField(1);
		DWORD goldAmount = m_ItemBits.GetField(goldsize?32:12);
		CString stemp ;
		stemp.Format("%d %s", goldAmount, m_sBaseName) ;
		m_sBaseName = stemp ;
	}

	m_sFileName.Format("%s.jpg", m_pMisc->m_InvFile) ;
}


void CDiabloItems::GetImage()
{
	//always seems to equal 2 for jewlry
	m_ItemBits.GetField(2);
	DWORD image = m_ItemBits.GetField(4, "image");

	// jewlry
	if ((m_sCode == "rin") || (m_sCode == "amu"))
		m_sFileName.Format("%s%d.jpg", m_pMisc->m_InvFile, image+1) ;

	// jewels
	else if (m_sCode == "jew")
		m_sFileName.Format("invjw%d.jpg", image+1) ;

	// for charms, need to do a little math to get the right pic
	else if (m_sCode.Left(2) == "cm") {
		int i = atoi( m_sCode.Right(1)) ;
		i += (3*image) ;
		m_sFileName.Format("invch%d.jpg", i) ;

	// everything else (shouldn't happen)
	} else
		m_sFileName.Format("%s.jpg", m_pMisc->m_InvFile) ;
}


void CDiabloItems::GetInferiorAttributes()
{
	// find out just how crappy it is!
	DWORD	iQualityType = m_ItemBits.GetField(3, "iQualityType") ;			// 170-172	// 187-189
	switch (iQualityType) {
	case 0: m_sBaseName = "Crude " + m_sBaseName ; break ;
	case 1: m_sBaseName = "Cracked " + m_sBaseName ; break ;
	case 2: m_sBaseName = "Damaged " + m_sBaseName ; break ;
	case 3: m_sBaseName = "Low Quality " + m_sBaseName ; break ;
	}
}


void CDiabloItems::GetSuperiorAttributes()
{
	// what special stuff will this superior do? - doesn't really matter - we'll find out
	//	from the magical itemstatid's later in the record
	DWORD	iQualityType = m_ItemBits.GetField(3, "iQualityType") ;			// 170-172	// 187-189
	m_sBaseName = "Superior " + m_sBaseName ;
}


void CDiabloItems::GetMagicAttributes()
{
	// get the prefix
	DWORD	iPrefix = m_ItemBits.GetField(11, "iPrefix") ;			// 170-172	// 187-189

	// if there is a prefix, do it up!
	if (iPrefix > 0) {
		// not sure why this is, but I've seen it on several items
		if (iPrefix == 601) {
			iPrefix = 75 ;		// the prefix Crimson
		}


		// prevent further errors if the affix is fuxored
		CString	stmp ;
		if (iPrefix > 600) {
//			PrintDebug("PREFIX LIMIT") ;
			stmp.Format("???????? %s", m_sBaseName) ;

		} else {
			stmp.Format("%s %s", gMagicPrefix[ iPrefix-1].m_Name, m_sBaseName) ;
		}

		m_sBaseName = stmp ;
	}


	// get the suffix
	DWORD	iSuffix = m_ItemBits.GetField(11, "iSuffix") ;			// 170-172	// 187-189


	// if there is a suffix, do it up!
	if (iSuffix > 0) {
		// not sure why this is, but I've seen it on one item
		if (iSuffix == 688) {
			iSuffix = 51 ;		// of Pestilence
		} else if ( iSuffix == 722) {
			iSuffix = 32 ;		// of Shock
		} else if ( iSuffix == 734) {
			iSuffix = 161 ;		// of Thorns
		} else if ( iSuffix == 732) {
			iSuffix = 105 ;
		}
		CString	stmp ;

		// prevent further errors if the affix is fuxored
		if (iSuffix > 675) {
//			PrintDebug("SUFFIX LIMIT") ;
			stmp.Format("%s of ????????", m_sBaseName) ;

		} else {
			stmp.Format("%s %s", m_sBaseName, gMagicSuffix[ iSuffix-1].m_Name) ;
		}
		m_sBaseName = stmp ;
	}
}




void CDiabloItems::GetSetAttributes()
{
	CSetItems*	pSetItem = GetSetItemRec( m_sCode) ;
	m_sSpecialName = pSetItem->m_Name ;

	DWORD	iSetID = m_ItemBits.GetField(12, "SetID") ;
}


void CDiabloItems::GetRareAttributes()
{

/*
Here's how this works.  A rare can have up to 6 modifiers (some mods have multiple params).
  Actually it has slots for 3 prefixes and 3 suffixes wired into the rec.  We alternate
  looking for a prefix and then a suffix.  Preceeding the affix is a single bit and if =1
  then that means the next 11 bits will be the affix.  If =0 then skip this affix.  I
  think that even if we hit the last affix we need to keep alternating until all 6 slots
  have been looked at.
*/

	int iPrefixName = m_ItemBits.GetField(8, "Rare Prefix") -155 ;
	int iSuffixName = m_ItemBits.GetField(8, "Rare Suffix") ;

	CString sTmp ;
	sTmp.Format("%s %s", gRarePrefix[ iPrefixName], gRareSuffix[ iSuffixName]) ;
	m_sSpecialName = sTmp ;



	BOOL bGetPrefix = TRUE ;
	for (int i=0; i<6; i++) {
		// let's see if we are out of the affix who's turn it is
		BOOL bGetNext = m_ItemBits.GetField(1, "bDoGetNext") ;
		if (bGetNext) {
			// out of prefixes
			if (bGetPrefix) {
				int iPrefix = m_ItemBits.GetField(11, "Prefix") ;
				// not sure why this is, but I've seen it on several items
				if (iPrefix == 601) {
					iPrefix = 75 ;		// the prefix Crimson

				// saftey precaution
				} else if (iPrefix > 600) {
//cn - consider putting this back to true!
//					PrintDebug("PREFIX LIMIT in GetRareAttributes", FALSE) ;
				}

			// out of suffixes
			} else {
				int iSuffix = m_ItemBits.GetField(11, "Suffix") ;
				// not sure why this is, but I've seen it on one item
				if (iSuffix == 688) {
					iSuffix = 51 ;		// the suffix of Pestilence

				// saftey precaution
				} else if (iSuffix > 675) {
//cn - consider putting this back to true!
//					PrintDebug("SUFFIX LIMIT in GetRareAttributes", FALSE) ;
				}
			}
		}

		// change the toggle to the other affix
		bGetPrefix = !bGetPrefix ;
	}
}


void CDiabloItems::GetUniqueAttributes()
{
	CUnique*	pUnique = GetUniqueRec( m_sCode) ;
	if (pUnique) {									// check added by syadasti
		m_sSpecialName = pUnique->m_Name ; }

	DWORD	iUniqueID = m_ItemBits.GetField(12, "UniqueID") ;
}




void CDiabloItems::GetMagicValues()
{
	DWORD iStatID = 0 ;

	// loop through all the stats until we hit the end
	while (iStatID != 0x1FF) {
		// make sure we don't get more bits then the length of the message
		if ((m_ItemBits.m_Pos/8)+9 > m_bMessageLen) {
//			PrintDebug("pre-empt m_bMessageLen exceeded") ;
			break ;
		}

		// get the next stat
		iStatID = m_ItemBits.GetField( STATID_BITLENGTH, "StatID") ;
		// this is the normal exit point
		if (iStatID == 0x1FF) {
			break ;

		// make sure this isn't some garbage number
		} else if (iStatID > NUM_STATIDS) {
//			PrintDebug("STATID VAL LIMIT") ;
			break ;
		}


//cn-WTF???
//
// poison damage looks wrong
//


		BOOL	bDone = FALSE ;
		while (!bDone) {

			// get the number of bits we need for the param value
			int iNumBits = gMagicStats[ iStatID].m_SaveBits ;


			// adjust the number of bits for special cases
			if ( gMagicStats[ iStatID].m_SaveAdd == 20) {



				// #110 (p len) it does NOT need an extra bit; also for #75 (max dur%),
				//   #119 (ac%), #121 (demon dam%), #122 (undead dam%), #1 (+energy),
				//   #154 (stam dr)
				if ((iStatID != 110) && (iStatID != 75) && (iStatID != 119) && 
					(iStatID != 121) && (iStatID != 122) && (iStatID != 1)  && 
					(iStatID != 154))
					iNumBits++ ;

			// i've only tested this on one item, so not sure
			} else if ( gMagicStats[ iStatID].m_SaveAdd == 10) {


				// statid #31 (AC%) does NOT need an extra bit; #77 (mana%);
				if ((iStatID != 31) && (iStatID != 77))
					iNumBits++ ;
			}

			// if its the secondary min/max then recalculate the number of bits to get
			if (( iStatID == STATS_SECONDARY_MINDAMAGE) || ( iStatID == STATS_SECONDARY_MAXDAMAGE))
				iNumBits = 1+ gMagicStats[ iStatID].m_SaveBits ;

			// get the value of the param
			int iMod = m_ItemBits.GetField( iNumBits, "Mod Value") ;

			// adjust the value by the SaveAdd vield
			if (( gMagicStats[ iStatID].m_SaveAdd == 20) && (iMod > 100))
				iMod -= 100 ;
//			else if (( gMagicStats[ iStatID].m_SaveAdd == 10) && (iMod > 30))
//				iMod -= 30 ;
			else {
				iMod -= gMagicStats[ iStatID].m_SaveAdd ;
if (( gMagicStats[ iStatID].m_SaveAdd == 10) && (iMod > 30) && (iMod > 54))
{} //PrintDebug("saveadd = 10 and iMod > 30", FALSE) ;
			}


			// get extra mod value for damage if needed
			if (( iStatID ==17) || ( iStatID ==18))
				iMod= m_ItemBits.GetField( DMGPERCENT_BITLENGTH, "Damage Val") ;

			// set the value for repair durability
			else if (iStatID == 252)
				iMod *= 11 ;
				

			char tempDesc[100] ;
			sprintf( tempDesc, gMagicStats[ iStatID].m_Desc, iMod) ;
			m_ModDesc[m_iNumModDesc] = tempDesc ;
			m_iNumModDesc++ ;

			// mods with multiple params (like cold and pois damage)
			if ((iStatID==48) || (iStatID==50) || (iStatID==52) || (iStatID==54) || 
				(iStatID==55) || (iStatID==57) || (iStatID==58))
				iStatID++ ;
			else
				bDone = TRUE ;
		}
	}
}




BOOL CDiabloItems::GetDurability()
{
	// durability and sockets
	m_iDurabilityMax = m_ItemBits.GetField(8, "m_iDurMax") ;
	// if the Max is 0, then the item is indestructable
	if ( m_iDurabilityMax == 0) {
		m_iDurabilityCur = 0 ;
		return TRUE ;
	}
	m_iDurabilityCur = m_ItemBits.GetField(8, "m_iDurCur") ;
	return FALSE ;
}





void CDiabloItems::LoadItemAttributes( const unsigned char *msg)
{
	int	iMessageLen = msg[ 2] ;
	for (int count=0; count<iMessageLen; count++)
		m_sPacketMsg[count] = msg[count] ;

	CommonByteParsing() ;

	// prepare to parse the bitfields which comprise the remaining bits;  need to exclude
	//	  5 extra bytes if it is a 9D message, still no idea why they did that
	m_ItemBits.m_Data = ((unsigned char*)msg+(( m_bMessageID == 0x9C)?8:13));

	// parse the on/off flags which are the first 32 bits
	ParseFlags() ;

	// parse the XY coordinates of where ever this item exists
	ParsePos() ;
//	dprintf(DEBUG_ALL, "m_sCode = %s", m_sCode);

	// get the specifics of the base item
	if (m_isEar) {
		m_bQuality = 1 ;
		m_sCode = "ear" ;
	} else
		GetBaseItem() ;
	
	if (m_isEar) {
		// do nothing
	} else if (m_isMiscItem) {
		GetMiscAttributes() ;
		GetBaseAttributes() ;
	} else {
		m_iUsedSockets = m_ItemBits.GetField(3, "usedSockets") ;		// 156-158	// 173-175
		DWORD level = m_ItemBits.GetField(7, "level");					// 159-165	// 176-182
		m_bQuality = m_ItemBits.GetField(3, "m_bQuality");				// 166-169	// 183-186
		
		GetBaseAttributes() ;
		
		// get the magic for rings, amulets, jewels, and charms
		if (((m_sCode == "rin") || (m_sCode == "amu") || (m_sCode == "jew") ||
			(m_sCode.Left(2) == "cm"))) {
			GetImage() ;
		} else {
			int isClassSpecific = m_ItemBits.GetField(3, "isClassSpec") ;
			if (isClassSpecific == 4) {
				m_ItemBits.GetField(11, "CLASS SPEC??") ;
			} else if (isClassSpecific) {
			}
		}

		// if the item is identified, get the magic info
		if (m_isIdentified) {
			switch (m_bQuality) {
			case QUAL_CRAFTED:	GetRareAttributes(); break ;
			case QUAL_INFERIOR:	GetInferiorAttributes(); break ;
			case QUAL_NORMAL:	break ;
			case QUAL_SUPERIOR:	GetSuperiorAttributes(); break ;
			case QUAL_MAGIC:	GetMagicAttributes(); break ;
			case QUAL_SET:		GetSetAttributes(); break ;
			case QUAL_RARE:		GetRareAttributes(); break ;
			case QUAL_UNIQUE:	GetUniqueAttributes(); break ;
			default:
				break ;
			}
		}

//cn-note sure how to make sense of this though :(
		// get the rune word
		if ( m_isRuneWord) {
			int	iRunePrefix = m_ItemBits.GetField(8, "iRunePrefix") ;
			int	iRuneSuffix = m_ItemBits.GetField(8, "iRuneSuffix") ;
		}


		// if the item is personalized, then get the name
		if ( m_isPersonalized) {
			char	charTmp = 1 ;
			CString	sPersonalized ;
			while (charTmp != 0) {
				charTmp = (char)m_ItemBits.GetField( 7, "p name") ;
				if ( charTmp > 0)
					sPersonalized += charTmp ;
			}
			sPersonalized += "'s " ;
			if ( m_sSpecialName != "")
				m_sSpecialName = sPersonalized + m_sSpecialName ;
			else
				m_sItemName = sPersonalized + m_sItemName ;
		}


		// for armor, get the defense
		if ( m_pArmor != NULL)
			m_iDefense = m_ItemBits.GetField(10, "Armor") - 10 ;


		// if the item is stackable, get the quantity
		if ( m_isStackable) {
			// for some reason, tombs have additional bits??
			if ((m_sBaseName == "Town Portal Book") || (m_sBaseName == "Identify Book")) {
				// i have seen this be 1 on a ident book
				if (m_ItemBits.GetField(5)) {} //PrintDebug("U9-2 ! 0", FALSE) ;					// 173-175	// 190-192

//cn		// weapons have an extra 16 bits here, throwing damage????
			} else if ( m_pWeapon != NULL) {
				DWORD	iThrowMin = m_ItemBits.GetField(8, "Throw 1??") ;
				DWORD	iThrowMax = m_ItemBits.GetField(8, "Throw 2??") ;
			}
			
			m_iQuantity = m_ItemBits.GetField(9, "quantity") ;
			
			
			// grab the durability
		} else if (m_hasDurability)
			m_isIndestructable = GetDurability() ; 
	}	
}


void CDiabloItems::AddInfoLine( CString sText, int iColor)
{/*
	COutputLine* newLine = new COutputLine ;
	newLine->m_iColor = iColor ;
	newLine->m_sLine = sText ;
	m_InfoLines.AddTail( newLine) ;*/
}


// we know we have an item, let's get the attributes
CDiabloItems* parseItem( const unsigned char *pkt_data)
{
	// its an item, so make a new one and load the attributes
	CDiabloItems* newItem = new CDiabloItems ;
	newItem->LoadItemAttributes( pkt_data) ;

//cn
	newItem->m_iOwnerType = ITEMOWNER_PLAYER ;

	return newItem ;
}
