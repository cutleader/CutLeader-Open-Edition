#include "StdAfx.h"
#include "DataCenterTree.h"

#include "baseConst.h"
#include "taoresource.h"
#include "IntegerList.h"

#include "DataCenterItemList.h"
#include "DataCenterItemBase.h"
#include "FolderNode.h"
#include "DataItem.h"
#include "DataCenterNode.h"
#include "SkinManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(DataCenterTree, XTreeCtrl)

BEGIN_MESSAGE_MAP(DataCenterTree, XTreeCtrl)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

DataCenterTree::DataCenterTree(void)
		: XTreeCtrl(IDC_CURSOR_TREE_COPYDROP, IDC_CURSOR_TREE_MOVEDROP, IDC_CURSOR_TREE_NODROP)
{
	m_pImageList = new CImageList();
}

DataCenterTree::~DataCenterTree(void)
{
	delete m_pImageList;
}

BOOL DataCenterTree::CanBeFolderNode(HTREEITEM hItem)
{
	UINT dataVal = GetItemData(hItem);
	DataCenterNode* pData = (DataCenterNode*)dataVal;
	if (pData->GetNodeType() == DataCenterNodeType_Folder)
		return TRUE;

	return FALSE;
}

void DataCenterTree::OnDestroy()
{
	// release the data which stored in the tree node.
	HTREEITEM hFirstRootItem = GetRootItem();
	if (hFirstRootItem)
		ReleaseAllNodeData(hFirstRootItem);

	XTreeCtrl::OnDestroy();
}

void DataCenterTree::AppendSubNode(DataCenterItemListPtr pDataCenterItems, HTREEITEM hParentNode, vector<HTREEITEM>& newTreeItemList)
{
	for (unsigned int i = 0; i < pDataCenterItems->size(); i++)
	{
		IDataCenterItemPtr pDataCenterItem = pDataCenterItems->at(i);

		if (!m_bShowHiddenDataItem && pDataCenterItem->GetName() == HiddenDataItemName) // 不显示隐藏的数据项。
			continue;


		/************************************************************************/
		// append "pDataCenterItem" to "hParentNode".

		// create tree node.
		TVINSERTSTRUCT hItem;
		hItem.hParent = hParentNode;
		hItem.item.pszText = (LPWSTR)(LPCTSTR)pDataCenterItem->GetName();
		hItem.item.cchTextMax = MAXLENGTH_STR;
		hItem.hInsertAfter = TVI_LAST;

		// the data which will be saved to the tree node.
		DataCenterNode* pDataCenterNode = new DataCenterNode();
		pDataCenterNode->SetDataCenterItemID(pDataCenterItem->GetID());

		if (pDataCenterItem->IsFolder())
		{
			if (pDataCenterItem->GetID() == PartsLibraryFolderID)
			{
				hItem.item.iImage = 1;
				hItem.item.iSelectedImage = 1;
			}
			else
			{
				hItem.item.iImage = 3;
				hItem.item.iSelectedImage = 3;
			}
			hItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;

			// save data in tree node.
			pDataCenterNode->SetNodeType(DataCenterNodeType_Folder);
		}
		else
		{
			DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);
			if (pDataItem->GetDataType() == DATANODE_PART)
			{
				hItem.item.iImage = 2;
				hItem.item.iSelectedImage = 2;

				pDataCenterNode->SetNodeType(DataCenterNodeType_Part);
			}
			else if (pDataItem->GetDataType() == DATANODE_Task)
			{
				hItem.item.iImage = 4;
				hItem.item.iSelectedImage = 4;

				pDataCenterNode->SetNodeType(DataCenterNodeType_Task);
			}

			hItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		}

		HTREEITEM hSubNode = InsertItem(&hItem);
		newTreeItemList.push_back(hSubNode);
		SetItemData(hSubNode, (int)pDataCenterNode);
		/************************************************************************/


		// check whether we will init "hSubNode"
		if (pDataCenterItem->IsFolder())
		{
			FolderNodePtr pFolderNode = boost::dynamic_pointer_cast<FolderNode>(pDataCenterItem);
			AppendSubNode(pFolderNode->GetSubItemList(), hSubNode, newTreeItemList);
		}
	}
}

BOOL DataCenterTree::DeleteItem(HTREEITEM hItem)
{
	ReleaseNodeData(hItem);
	return XTreeCtrl::DeleteItem(hItem);
}

BOOL DataCenterTree::LocateNode(HTREEITEM hItem, LONGLONG iDataCenterItemID)
{
	// get the item data.
	UINT dataVal = GetItemData(hItem);
	DataCenterNode* pData = (DataCenterNode*)dataVal;

	// check whether this item is the item we want.
	if (pData->GetDataCenterItemID() == iDataCenterItemID)
	{
		// select the node.
		SelectAll(FALSE);
		SetItemState(hItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		SetFocus();

		return TRUE;
	}

	if (pData->GetNodeType() == DataCenterNodeType_Folder)
	{
		// expand this "folder" node.
		Expand(hItem, TVE_EXPAND);

		// go through each "child" node.
		BOOL bFound = FALSE;

		HTREEITEM hChildItem = GetChildItem(hItem);
		while (hChildItem)
		{
			if (LocateNode(hChildItem, iDataCenterItemID))
			{
				bFound = TRUE;
				return TRUE;
			}
			else
			{
				hChildItem = GetNextSiblingItem(hChildItem);
			}
		}

		// IMPORTANT! if do not found it in this "folder" node, collapse it.
		if (!bFound)
		{
			Expand(hItem, TVE_COLLAPSE);
		}
	}

	return FALSE;
}

void DataCenterTree::NavigateToNode(LONGLONG iNaviNodeID)
{
	HTREEITEM hRootItem = GetRootItem();

	if (iNaviNodeID == INVALID_ID)
	{
		// select the root node.
		SelectAll(FALSE);
		SetItemState(hRootItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		SetFocus();

		return;
	}

	// go through the children.
	LocateNode(hRootItem, iNaviNodeID);
}

void DataCenterTree::Init()
{
	// enable multiple selection.
	SetMultiSelect(TRUE);

	SetBkColor(SkinManager::GetBackgroundColor());
	SetTextColor(SkinManager::GetTextColor());
	SetLineColor(SkinManager::GetTextColor());
	SetInsertMarkColor(SkinManager::GetTextColor());

	// init the image list.
	m_pImageList->Create(IDB_PRODUCT_EXPL_TREE, 16, 10, RGB(192, 192, 192));
	SetImageList(m_pImageList, TVSIL_NORMAL);
}

void DataCenterTree::InitTree(FolderNodePtr pWorkspace, bool bShowHiddenDataItem)
{
	m_pWorkspace = pWorkspace;
	m_bShowHiddenDataItem = bShowHiddenDataItem;


	/************************************************************************/
	// append the root node.

	// create tree node.
	TVINSERTSTRUCT workspaceItem;
	workspaceItem.hParent = NULL;
	workspaceItem.item.iImage = 0;
	workspaceItem.item.iSelectedImage = 0;
	workspaceItem.item.pszText = (LPWSTR)(LPCTSTR)pWorkspace->GetName();
	workspaceItem.item.cchTextMax = MAXLENGTH_STR;
	workspaceItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	workspaceItem.hInsertAfter = TVI_LAST;
	HTREEITEM hRootNode = InsertItem(&workspaceItem);

	// save data in tree node.
	DataCenterNode* pDataCenterNode = new DataCenterNode();
	pDataCenterNode->SetNodeType(DataCenterNodeType_Folder);
	pDataCenterNode->SetDataCenterItemID(pWorkspace->GetID());
	SetItemData(hRootNode, (int)pDataCenterNode);
	/************************************************************************/

	// append all sub-nodes under "pWorkspace".
	vector<HTREEITEM> newTreeItemList;
	AppendSubNode(pWorkspace->GetSubItemList(), hRootNode, newTreeItemList);

	// expand the root node.
	Expand(hRootNode, TVE_EXPAND);
}

IntegerListPtr DataCenterTree::GetSelectedTreeNodes()
{
	IntegerListPtr pTreeItemList(new IntegerList);

	// first, get all selected tree node.
	vector<HTREEITEM> tmpTreeItemList;
	tmpTreeItemList = GetSelectedList();

	/************************************************************************/
	// check each tree node.

	for (unsigned int i = 0; i < tmpTreeItemList.size(); i++)
	{
		BOOL bNeedThis = TRUE;
		HTREEITEM hTmpItem = tmpTreeItemList.at(i);  

		// check the parent/grand-parent/.... nodes.
		HTREEITEM hParentItem = GetParentItem(hTmpItem);
		while (hParentItem)
		{
			if (IncludeItem(tmpTreeItemList, hParentItem))
			{
				bNeedThis = FALSE;
				break;
			}

			// get next "parent" node.
			hParentItem = GetParentItem(hParentItem);
		}

		if (bNeedThis)
			pTreeItemList->AddInt(GetItemData(hTmpItem));
	}
	/************************************************************************/

	return pTreeItemList;
}

void DataCenterTree::ReleaseAllNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	int iData = GetItemData(hParentItem);
	DataCenterNode* pDataCenterNode = (DataCenterNode*)iData;
	delete pDataCenterNode;

	// go through all brother nodes of "hParentItem".
	HTREEITEM hBrotherNode = GetNextSiblingItem(hParentItem);
	if (hBrotherNode != NULL)
		ReleaseAllNodeData(hBrotherNode);

	// go through all child nodes of "hParentItem".
	HTREEITEM hChildNode = GetChildItem(hParentItem);
	if (hChildNode != NULL)
		ReleaseAllNodeData(hChildNode);
}

void DataCenterTree::ReleaseNodeData(HTREEITEM hParentItem)
{
	// release the data stored in this node.
	int iData = GetItemData(hParentItem);
	DataCenterNode* pDataCenterNode = (DataCenterNode*)iData;
	delete pDataCenterNode;

	HTREEITEM hChild = GetChildItem(hParentItem);
	while (hChild != NULL)
	{
		ReleaseNodeData(hChild);
		hChild = GetNextSiblingItem(hChild);
	}
}

END_CUTLEADER_NAMESPACE()
