#include "StdAfx.h"
#include "CutSequenceTree.h"

#include "baseConst.h"
#include "clUtilityResource.h"
#include "taoresource.h"

#include "ICutSequence.h"
#include "CutSequenceList.h"
#include "clCutSequenceResource.h"
#include "SequenceTreeNodeData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CutSequenceTree, XTreeCtrl)

BEGIN_MESSAGE_MAP(CutSequenceTree, XTreeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CutSequenceTree::CutSequenceTree(void)
	: XTreeCtrl(IDC_CURSOR_TREE_COPYDROP, IDC_CURSOR_TREE_MOVEDROP, IDC_CURSOR_TREE_NODROP)
{
	// disable drag/drop.
	EnableDragDrop(FALSE);

	// enable multiple selection.
	SetMultiSelect(TRUE);

	// load the image for the tree.
	m_pImageList = new CImageList();
	m_pImageList->Create(IDB_CUTSEQ_TREE, 16, 10, RGB(192, 192, 192));
}

CutSequenceTree::~CutSequenceTree(void)
{
	// release the image.
	if (m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

int CutSequenceTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (XTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CutSequenceTree::OnDestroy()
{
	// release the data which stored in the tree node.
	HTREEITEM hFirstRootItem = GetRootItem();
	if (hFirstRootItem)
		ReleaseAllNodeData(hFirstRootItem);

	XTreeCtrl::OnDestroy();
}

void CutSequenceTree::ReleaseAllNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	int iData = GetItemData(hParentItem);
	SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)iData;
	delete pSeqTreeNodeData;

	// go through all brother nodes of "hParentItem".
	HTREEITEM hBrotherNode = GetNextSiblingItem(hParentItem);
	if (hBrotherNode != NULL)
	{
		ReleaseAllNodeData(hBrotherNode);
	}

	// go through all child nodes of "hParentItem".
	HTREEITEM hChildNode = GetChildItem(hParentItem);
	if (hChildNode != NULL)
	{
		ReleaseAllNodeData(hChildNode);
	}
}

void CutSequenceTree::ReleaseNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	int iData = GetItemData(hParentItem);
	SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)iData;
	delete pSeqTreeNodeData;

	HTREEITEM hChild = GetChildItem(hParentItem);
	while (hChild != NULL)
	{
		ReleaseNodeData(hChild);
		hChild = GetNextSiblingItem(hChild);
	}
}

void CutSequenceTree::DisplaySeqUnitTree(CutSequenceListPtr pCutSequences)
{
	m_pCutSequences = pCutSequences;
	SetImageList(m_pImageList, TVSIL_NORMAL);

	/************************************************************************/
	// reset the tree if the tree has been inited.

	// release the data which stored in the tree node.
	HTREEITEM hFirstRootItem = GetRootItem();
	if (hFirstRootItem)
		ReleaseAllNodeData(hFirstRootItem);

	// delete tree nodes at first
	DeleteAllItems();
	/************************************************************************/


	/************************************************************************/
	// insert the root node.

	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTSEQ_LIST);
	HTREEITEM hRootNode = InsertItem(str);
	SetItemImage(hRootNode, 0, 0);

	// set item data.
	SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
	pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_ROOT);
	pNodeData->SetNodeData(INVALID_ID);
	SetItemData(hRootNode, (int)pNodeData);
	/************************************************************************/


	// 显示轮廓工序。
	{
		CutSequenceListPtr pLoopSequenceList = pCutSequences->GetCutSequencesByType(CutSequence_Loop);

		HTREEITEM hLoopSeqNode = NULL;
		if (pLoopSequenceList->size() > 0)
		{
			// insert "Loop Sequence Unit" sub node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_SEQUNIT);
			hLoopSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hLoopSeqNode, 1, 1);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hLoopSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pLoopSequenceList->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pLoopSequenceList->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i + 1);
			HTREEITEM hTmpNode = InsertItem(str, hLoopSeqNode);
			SetItemImage(hTmpNode, 1, 1);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示“阵列轮廓工序”。
	{
		CutSequenceListPtr pLoopGridSeqList = pCutSequences->GetCutSequencesByType(CutSequence_GridLoop);

		HTREEITEM hLoopGridSeqNode = NULL;
		if (pLoopGridSeqList->size() > 0)
		{
			// insert "Loop Grid Sequence Unit" sub node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOP_GRID_SEQUNIT);
			hLoopGridSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hLoopGridSeqNode, 1, 1);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hLoopGridSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pLoopGridSeqList->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pLoopGridSeqList->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i + 1);
			HTREEITEM hTmpNode = InsertItem(str, hLoopGridSeqNode);
			SetItemImage(hTmpNode, 1, 1);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示共边工序。
	{
		CutSequenceListPtr pComSeqUnitList = pCutSequences->GetCutSequencesByType(CutSequence_CommonCut);
		if (pComSeqUnitList->size() == 0)
			pComSeqUnitList = pCutSequences->GetCutSequencesByType(CutSequence_CanvasCommonCut);

		HTREEITEM hComSeqUnitNode = NULL;
		if (pComSeqUnitList->size() > 0)
		{
			// insert "Common Cutting Sequence Unit" sub node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMCUT_SEQUNIT);
			hComSeqUnitNode = InsertItem(str, hRootNode);
			SetItemImage(hComSeqUnitNode, 2, 2);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hComSeqUnitNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pComSeqUnitList->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pComSeqUnitList->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i + 1);
			HTREEITEM hTmpNode = InsertItem(str, hComSeqUnitNode);
			SetItemImage(hTmpNode, 2, 2);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示“桥接工序”。
	{
		CutSequenceListPtr pBdgeSeqUnitList = pCutSequences->GetCutSequencesByType(CutSequence_Bridge);

		HTREEITEM hBdgeSeqNode = NULL;
		if (pBdgeSeqUnitList->size() > 0)
		{
			// 插入树节点。
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_BDGE_SEQUNIT);
			hBdgeSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hBdgeSeqNode, 3, 3);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hBdgeSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pBdgeSeqUnitList->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pBdgeSeqUnitList->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i + 1);
			HTREEITEM hTmpNode = InsertItem(str, hBdgeSeqNode);
			SetItemImage(hTmpNode, 3, 3);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示“连割工序”。
	{
		CutSequenceListPtr pChainSeqs = pCutSequences->GetCutSequencesByType(CutSequence_ChainCut);

		HTREEITEM hChainSeqNode = NULL;
		if (pChainSeqs->size() > 0)
		{
			// 插入树节点。
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CHAIN_SEQUNIT);
			hChainSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hChainSeqNode, 4, 4);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hChainSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pChainSeqs->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pChainSeqs->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i + 1);
			HTREEITEM hTmpNode = InsertItem(str, hChainSeqNode);
			SetItemImage(hTmpNode, 4, 4);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示“田字工序”。
	{
		CutSequenceListPtr pGridSeqs = pCutSequences->GetCutSequencesByType(CutSequence_GridCut);

		HTREEITEM hGridSeqNode = NULL;
		if (pGridSeqs->size() > 0)
		{
			// 插入树节点。
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GRID_SEQUNIT);
			hGridSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hGridSeqNode, 5, 5);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hGridSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pGridSeqs->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pGridSeqs->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i + 1);
			HTREEITEM hTmpNode = InsertItem(str, hGridSeqNode);
			SetItemImage(hTmpNode, 5, 5);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示“飞行切割工序”。
	{
		CutSequenceListPtr pFlyCutSeqs = pCutSequences->GetCutSequencesByType(CutSequence_FlyCut);

		HTREEITEM hFlyCutSeqNode = NULL;
		if (pFlyCutSeqs->size() > 0)
		{
			// 插入树节点。
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FLYCUT_SEQUNIT);
			hFlyCutSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hFlyCutSeqNode, 6, 6);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hFlyCutSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pFlyCutSeqs->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pFlyCutSeqs->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i+1);
			HTREEITEM hTmpNode = InsertItem(str, hFlyCutSeqNode);
			SetItemImage(hTmpNode, 6, 6);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// 显示“余料切割线”。
	{
		CutSequenceListPtr pRemnantLineSeqs = pCutSequences->GetCutSequencesByType(CutSequence_RemnantCutLine);

		HTREEITEM hCutSeqNode = NULL;
		if (pRemnantLineSeqs->size() > 0)
		{
			// 插入树节点。
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_REMNANT_CUTLINE);
			hCutSeqNode = InsertItem(str, hRootNode);
			SetItemImage(hCutSeqNode, 7, 7);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_TYPE);
			pNodeData->SetNodeData(INVALID_ID);
			SetItemData(hCutSeqNode, (int)pNodeData);
		}

		for (unsigned int i = 0; i < pRemnantLineSeqs->size(); i++)
		{
			ICutSequencePtr pSeqUnit = pRemnantLineSeqs->at(i);

			// insert the "Instance n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INSTANCE);
			str.AppendFormat(_T(" %d"), i+1);
			HTREEITEM hTmpNode = InsertItem(str, hCutSeqNode);
			SetItemImage(hTmpNode, 7, 7);

			// set item data.
			SequenceTreeNodeData* pNodeData = new SequenceTreeNodeData;
			pNodeData->SetSeqTreeNodeType(SEQ_TREE_NODE_SEQ_ITEM);
			pNodeData->SetNodeData(pSeqUnit->GetID());
			SetItemData(hTmpNode, (int)pNodeData);
		}
	}

	// expand the root node.
	Expand(hRootNode, TVE_EXPAND);
}

void CutSequenceTree::DeleteSelItems()
{
	// get all selected tree items.
	vector<HTREEITEM> selTreeItemList = GetSelectedTreeItems();

	for (unsigned int i = 0; i < selTreeItemList.size(); i++)
	{
		HTREEITEM hSelTreeItem = selTreeItemList.at(i);
		
		// release the data stored in "hSelTreeItem" and its children.
		ReleaseNodeData(hSelTreeItem);

		// delete the tree node.
		DeleteItem(hSelTreeItem);
	}
}

CutSequenceListPtr CutSequenceTree::GetSelCutSeqs()
{
	CutSequenceListPtr pSelSeqUnitList(new CutSequenceList);

	// get the selected tree nodes.
	vector<HTREEITEM> selectTreeNodes = GetSelectedTreeItems();

	// go through each tree node, get sequence unit under SEQ_TREE_NODE_ROOT and
	// SEQ_TREE_NODE_SEQ_TYPE node.
	for (unsigned int i = 0; i < selectTreeNodes.size(); i++)
	{
		HTREEITEM treeItem = selectTreeNodes.at(i);

		// get the item data.
		int iData = GetItemData(treeItem);
		SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)iData;

		if (pSeqTreeNodeData->GetSeqTreeNodeType() == SEQ_TREE_NODE_SEQ_ITEM)
		{
			LONGLONG iCutSequenceID = pSeqTreeNodeData->GetNodeData();
			ICutSequencePtr pSeqUnit = m_pCutSequences->GetCutSequenceByID(iCutSequenceID);
			if (pSeqUnit)
				pSelSeqUnitList->push_back(pSeqUnit);
		}
		else
		{
			CutSequenceListPtr pTmpSeqUnitList(new CutSequenceList);
			GetSubSeqNodes(treeItem, pTmpSeqUnitList);
			pSelSeqUnitList->insert(pSelSeqUnitList->end(), pTmpSeqUnitList->begin(), pTmpSeqUnitList->end());
		}
	}

	return pSelSeqUnitList;
}

void CutSequenceTree::GetSubSeqNodes(HTREEITEM hParentItem, CutSequenceListPtr& pCutSequences)
{
	HTREEITEM hChildNode = GetChildItem(hParentItem);
	while (hChildNode != NULL)
	{
		GetSubSeqNodes(hChildNode, pCutSequences);
		hChildNode = GetNextSiblingItem(hChildNode);
	}

	//
	int iData = GetItemData(hParentItem);
	SequenceTreeNodeData* pSeqTreeNodeData = (SequenceTreeNodeData*)iData;
	if (pSeqTreeNodeData->GetSeqTreeNodeType() == SEQ_TREE_NODE_SEQ_ITEM)
	{
		LONGLONG iCutSequenceID = pSeqTreeNodeData->GetNodeData();
		ICutSequencePtr pSeqUnit = m_pCutSequences->GetCutSequenceByID(iCutSequenceID);
		if (pSeqUnit)
			pCutSequences->push_back(pSeqUnit);
	}
}

END_CUTLEADER_NAMESPACE()
