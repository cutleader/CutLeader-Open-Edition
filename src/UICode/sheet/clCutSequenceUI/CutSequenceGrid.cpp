#include "StdAfx.h"
#include "CutSequenceGrid.h"

#include "clUtilityResource.h"

#include "ICutSequence.h"
#include "CutSequenceList.h"
#include "clCutSequenceResource.h"
#include "SequenceTreeNodeData.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CutSequenceGrid, CGridListCtrlEx)

BEGIN_MESSAGE_MAP(CutSequenceGrid, CGridListCtrlEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()

END_MESSAGE_MAP()

CutSequenceGrid::CutSequenceGrid(void)
{
}

CutSequenceGrid::~CutSequenceGrid(void)
{
}

void CutSequenceGrid::OnDestroy()
{
	ReleaseGridItemData();

	CGridListCtrlEx::OnDestroy();
}

int CutSequenceGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGridListCtrlEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// init the column.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	InsertColumn(0, str, LVCFMT_LEFT, 40);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQUNIT);
	InsertColumn(1, str, LVCFMT_LEFT, 180);

	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return 0;
}

void CutSequenceGrid::DisplaySeqUnitGrid(CutSequenceListPtr pCutSequences)
{
	m_pCutSequences = pCutSequences;

	// clear all items.
	ReleaseGridItemData();
	DeleteAllItems();

	for (unsigned int i = 0; i < m_pCutSequences->size(); i++)
	{
		ICutSequencePtr pSeqUnit = m_pCutSequences->at(i);

		// insert an item.
		CString str;
		str.Format(_T("%d"), i+1);
		InsertItem(i, str);

		/************************************************************************/
		// set the second column.

		if (pSeqUnit->GetCutSequenceType() == CutSequence_Loop)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_SEQUNIT);
		else if (pSeqUnit->GetCutSequenceType() == CutSequence_GridLoop)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_GRID_SEQUNIT);
		else if (pSeqUnit->GetCutSequenceType() == CutSequence_CommonCut)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMCUT_SEQUNIT);
		else if (pSeqUnit->GetCutSequenceType() == CutSequence_CanvasCommonCut)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMCUT_SEQUNIT);

		SetItemText(i, 1, str);

		// set item data.
		SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
		pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
		pNodeData->SetNodeData(pSeqUnit->GetID());
		SetItemData(i, (int)pNodeData);
		/************************************************************************/
	}
}

CutSequenceListPtr CutSequenceGrid::GetSelCutSeqs()
{
	CutSequenceListPtr pSelSeqUnitList(new CutSequenceList);

	for (int i = 0; i < GetItemCount(); i++)
	{
		if (IsRowSelected(i))
		{
			int iItemData = GetItemData(i);

			// get the sequence unit object.
			SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)iItemData;
			LONGLONG iCutSequenceID = pSeqTreeNodeData->GetNodeData();
			ICutSequencePtr pSeqUnit = m_pCutSequences->GetCutSequenceByID(iCutSequenceID);
			if (pSeqUnit)
				pSelSeqUnitList->push_back(pSeqUnit);
		}
	}

	return pSelSeqUnitList;
}

void CutSequenceGrid::DeleteSelItems()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		if (IsRowSelected(i))
		{
			int iItemData = GetItemData(i);

			// delete the grid item.
			DeleteItem(i);
			i--;

			// delete the data stored in the grid item.
			SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)iItemData;
			delete pSeqTreeNodeData;
		}
	}
}

void CutSequenceGrid::ReleaseGridItemData()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		DWORD_PTR pItemData = GetItemData(i);

		// delete the data stored in the grid item.
		SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)pItemData;
		delete pSeqTreeNodeData;
	}
}

END_CUTLEADER_NAMESPACE()
