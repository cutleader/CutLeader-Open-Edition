#include "StdAfx.h"
#include "PartPlacementTree.h"

#include "IntegerList.h"
#include "taoresource.h"
#include "DataBaseManager.h"
#include "DataCenterItemLoader.h"

#include "PartLoader.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "clPartLayoutResource.h"
#include "PartPlacementTreeNodeData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(PartPlacementTree, XTreeCtrl)

BEGIN_MESSAGE_MAP(PartPlacementTree, XTreeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()

END_MESSAGE_MAP()

PartPlacementTree::PartPlacementTree(void)
	: XTreeCtrl(IDC_CURSOR_TREE_COPYDROP, IDC_CURSOR_TREE_MOVEDROP, IDC_CURSOR_TREE_NODROP)
{
	// disable drag/drop.
	EnableDragDrop(FALSE);

	// enable multiple selection.
	SetMultiSelect(TRUE);

	// load the image for the tree.
	m_pImageList = new CImageList();
	m_pImageList->Create(IDB_PARTPMT_TREE, 16, 10, RGB(192, 192, 192));
}

PartPlacementTree::~PartPlacementTree(void)
{
	// release the image.
	if (m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

int PartPlacementTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (XTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void PartPlacementTree::OnDestroy()
{
	// release the data which stored in the tree node.
	HTREEITEM hFirstRootItem = GetRootItem();
	if (hFirstRootItem)
		ReleaseAllNodeData(hFirstRootItem);

	XTreeCtrl::OnDestroy();
}

void PartPlacementTree::ReleaseAllNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	int iData = GetItemData(hParentItem);
	PartPlacementTreeNodeData* pPmtTreeNodeData = (PartPlacementTreeNodeData*)iData;
	delete pPmtTreeNodeData;

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

void PartPlacementTree::DisplayPartPmtTree(PartPlacementListPtr pPartPlacements)
{
	m_pPartPlacementList = pPartPlacements;
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
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT_LIST);
	HTREEITEM hRootNode = InsertItem(str);
	SetItemImage(hRootNode, 0, 0);

	// set item data.
	PartPlacementTreeNodeData* pNodeData = new PartPlacementTreeNodeData;
	pNodeData->SetPmtTreeNodeType(PMT_TREE_NODE_ROOT);
	pNodeData->SetNodeData(INVALID_ID);
	SetItemData(hRootNode, (int)pNodeData);
	/************************************************************************/

	/************************************************************************/
	// append second and third layer.

	DBConnectPtr pMainDBCon = DataBaseManager::GetInstance()->GetMainDBConnect();

	// get all kinds parts.
	IntegerListPtr pPartIDList = pPartPlacements->GetPartIDList();

	// insert each part node.
	for (unsigned int i = 0; i < pPartIDList->size(); i++)
	{
		// get the prop of the part.
		LONGLONG iPartID = pPartIDList->at(i);
		DataProperty partProp;
		DataCenterItemLoader::GetDataProp(pMainDBCon, iPartID, partProp);

		// get the part name.
		DataPropItem propItem = partProp.GetItem(0);
		const CComVariant& v = propItem.GetValue();
		CString strPartName = (LPCTSTR)v.bstrVal;

		// insert the "part ***" node.
		HTREEITEM hPartNode = InsertItem(strPartName.AllocSysString(), hRootNode);
		SetItemImage(hPartNode, 1, 1);

		// set item data.
		PartPlacementTreeNodeData* pNodeData = new PartPlacementTreeNodeData;
		pNodeData->SetPmtTreeNodeType(PMT_TREE_NODE_PART);
		pNodeData->SetNodeData(INVALID_ID);
		SetItemData(hPartNode, (int)pNodeData);

		// insert each part pmt node.
		PartPlacementListPtr pPartPlacementList1 = pPartPlacements->GetPartPlacementByPart(iPartID);
		for (unsigned int j = 0; j < pPartPlacementList1->size(); j++)
		{
			PartPlacementPtr pPartPlacement = pPartPlacementList1->at(j);

			// insert the "Part Placement n" node.
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PARTPMT);
			str.AppendFormat(_T(" %d"), j+1);
			HTREEITEM hPartPmtNode = InsertItem(str, hPartNode);
			SetItemImage(hPartPmtNode, 2, 2);

			// set item data.
			PartPlacementTreeNodeData* pPmtNodeData = new PartPlacementTreeNodeData;
			pPmtNodeData->SetPmtTreeNodeType(PMT_TREE_NODE_PMT_ITEM);
			pPmtNodeData->SetNodeData(pPartPlacement->GetID());
			SetItemData(hPartPmtNode, (int)pPmtNodeData);
		}
	}
	/************************************************************************/

	// expand the root node.
	Expand(hRootNode, TVE_EXPAND);
}

void PartPlacementTree::ReleaseNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	int iData = GetItemData(hParentItem);
	PartPlacementTreeNodeData* pPmtTreeNodeData = (PartPlacementTreeNodeData*)iData;
	delete pPmtTreeNodeData;

	HTREEITEM hChild = GetChildItem(hParentItem);
	while (hChild != NULL)
	{
		ReleaseNodeData(hChild);
		hChild = GetNextSiblingItem(hChild);
	}
}

void PartPlacementTree::DeleteSelItems()
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

PartPlacementListPtr PartPlacementTree::GetSelPartPlacementList()
{
	PartPlacementListPtr pSelectedPartPlacements(new PartPlacementList);

	// get the selected tree nodes.
	vector<HTREEITEM> selectTreeNodes = GetSelectedTreeItems();

	// go through each tree node, get part pmt under PMT_TREE_NODE_ROOT and
	// PMT_TREE_NODE_PART node.
	for (unsigned int i = 0; i < selectTreeNodes.size(); i++)
	{
		HTREEITEM treeItem = selectTreeNodes.at(i);

		// get the item data.
		int iData = GetItemData(treeItem);
		PartPlacementTreeNodeData* pPmtTreeNodeData = (PartPlacementTreeNodeData*)iData;

		if (pPmtTreeNodeData->GetPmtTreeNodeType() == PMT_TREE_NODE_PMT_ITEM)
		{
			LONGLONG iPartPmtID = pPmtTreeNodeData->GetNodeData();
			PartPlacementPtr pPartPlacement = m_pPartPlacementList->GetPartPlacementByID(iPartPmtID);
			if (pPartPlacement)
				pSelectedPartPlacements->push_back(pPartPlacement);
		}
		else
		{
			PartPlacementListPtr pTmpPartPlacementList(new PartPlacementList);
			GetSubPmtNodes(treeItem, pTmpPartPlacementList);
			pSelectedPartPlacements->insert(pSelectedPartPlacements->end(), 
									pTmpPartPlacementList->begin(), pTmpPartPlacementList->end());
		}
	}

	return pSelectedPartPlacements;
}

void PartPlacementTree::GetSubPmtNodes(HTREEITEM hParentItem, PartPlacementListPtr& pPartPlacements)
{
	HTREEITEM hChildNode = GetChildItem(hParentItem);
	while (hChildNode != NULL)
	{
		GetSubPmtNodes(hChildNode, pPartPlacements);

		hChildNode = GetNextSiblingItem(hChildNode);
	}

	//
	int iData = GetItemData(hParentItem);
	PartPlacementTreeNodeData* pPmtTreeNodeData = (PartPlacementTreeNodeData*)iData;
	if (pPmtTreeNodeData->GetPmtTreeNodeType() == PMT_TREE_NODE_PMT_ITEM)
	{
		LONGLONG iPartPmtID = pPmtTreeNodeData->GetNodeData();
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->GetPartPlacementByID(iPartPmtID);
		if (pPartPlacement)
			pPartPlacements->push_back(pPartPlacement);
	}
}

END_CUTLEADER_NAMESPACE()
