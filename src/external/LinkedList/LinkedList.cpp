#define WIN32_LEAN_AND_MEAN
//
#include "LinkedList.h"
#include "..\..\pktk\DiabloItems.h"
#include "..\..\util.h"

LinkedList::LinkedList()
{
	pFirst = 0;
	pLast = 0;
	cItemCount = 0;
}

LinkedList::~LinkedList()
{
	LinkedItem* pItem;
	while ((pItem = GetFirstItem()) != 0)
		RemoveItem(pItem);
}

//============================================================================
//	AddItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::AddItem(LPVOID lpData)
{
	LinkedItem* pItem = new LinkedItem;

	if (!pItem)
		return 0;

	pItem->lpData = lpData;
	pItem->pNext = 0;

	if (!pFirst)
		pFirst = pItem;

	if (pLast)
		pLast->pNext = pItem;

	pItem->pPrev = pLast;
	pLast = pItem;

	cItemCount++;
	
	//_LinkedListBusy = FALSE;

	return pItem;
}

//============================================================================
//	RemoveItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::RemoveItem(LinkedItem* pItem)
{
	LinkedItem* pNext;

	if (!pFirst || !pLast || !pItem)
		return 0;

	if (pItem->pPrev)
		pItem->pPrev->pNext = pItem->pNext;

	if (pItem->pNext)
		pItem->pNext->pPrev = pItem->pPrev;

	if (pItem == pFirst)
		pFirst = pItem->pNext;

	if (pItem == pLast)
		pLast = pItem->pPrev;

	pNext = pItem->pNext;

	delete pItem;

	cItemCount--;

	return pNext;
}

//============================================================================
//	GetFirstItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetFirstItem()
{
	return pFirst;
}

//============================================================================
//	GetLastItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetLastItem()
{
	return pLast;
}

//============================================================================
//	GetPrevItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetPrevItem(LinkedItem* pItem)
{
	return (pItem ? pItem->pPrev : 0);
}

//============================================================================
//	GetNextItem
//----------------------------------------------------------------------------
//============================================================================
LinkedItem* LinkedList::GetNextItem(LinkedItem* pItem)
{
	return (pItem ? pItem->pNext : 0);
}

//============================================================================
//	GetItemCount
//----------------------------------------------------------------------------
//============================================================================
UINT LinkedList::GetItemCount()
{
	return cItemCount;
}

// sorting adapted from http://www.digicide.net/code/display.php?prog_id=7
// sorts a linked list using a recursive selection sort
VOID LinkedList::SwapItems(LPVOID &a, LPVOID &b) {
	LPVOID temp;
	
	temp = a;
	a = b;
	b = temp; }

// this is only good for CDiabloItems * lists
LinkedItem *LinkedList::FindMax(LinkedItem *head) { 
	// recursively finds the node containing the smallest data field 
	// in the nonempty linked list pointed to by head 
	if (head->pNext == NULL) {
		return head; }
	else {
		CDiabloItems *thisitem = (CDiabloItems *)head->lpData;
		CDiabloItems *maxitem = (CDiabloItems *)FindMax(head->pNext)->lpData;
		int thisWidth, thisHeight, maxWidth, maxHeight;
		GetItemDimensions(thisWidth, thisHeight, thisitem);
		GetItemDimensions(maxWidth, maxHeight, maxitem);
		if (thisWidth * thisHeight > maxWidth * maxHeight) {
			return head; } }
	return FindMax(head->pNext); } 

VOID LinkedList::SortList(LinkedItem *head) { 
	// sorts the linked list pointed to by head into increasing order 
	// using a recursive selection sort 
	if (head != NULL) { 
		SwapItems(head->lpData, FindMax(head)->lpData); 
		SortList(head->pNext); } } 
