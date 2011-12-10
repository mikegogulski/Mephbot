//
#pragma once

#include <windows.h>

typedef struct LinkedItem_t
{
	LinkedItem_t*	pPrev;
	LinkedItem_t*	pNext;
	LPVOID			lpData;
} LinkedItem;

class LinkedList
{
private:
	LinkedItem*	pFirst;
	LinkedItem*	pLast;
	UINT		cItemCount;
public:
	LinkedList();
	virtual ~LinkedList();
	LinkedItem* AddItem(LPVOID lpData);
	LinkedItem* RemoveItem(LinkedItem* pItem);
	LinkedItem* GetFirstItem();
	LinkedItem* GetLastItem();
	LinkedItem* GetPrevItem(LinkedItem* pItem);
	LinkedItem* GetNextItem(LinkedItem* pItem);
	VOID PurgeList();
	UINT LinkedList::GetItemCount();
	VOID SwapItems(LPVOID &a, LPVOID &b);
	LinkedItem *FindMax(LinkedItem *head);  
	VOID SortList(LinkedItem *head);
};

#define PURGELIST(list, type)	\
	if (list->GetItemCount()) { \
		LinkedItem *q; \
		for (q = list->GetFirstItem(); list->GetItemCount() && q; q = list->RemoveItem(q)) { \
			if (q && q->lpData) { \
				delete type(q->lpData); } } }
