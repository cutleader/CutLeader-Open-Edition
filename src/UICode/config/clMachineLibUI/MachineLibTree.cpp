#include "StdAfx.h"
#include "MachineLibTree.h"

#include "baseConst.h"
#include "taoresource.h"

#include "clMachineLibResource.h"
#include "MachineLibNodeList.h"
#include "MachineLibNode.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC( MachineLibTree, SkinXTreeCtrl )

MachineLibTree::MachineLibTree(void)
				: SkinXTreeCtrl( IDC_CURSOR_TREE_COPYDROP, IDC_CURSOR_TREE_MOVEDROP, IDC_CURSOR_TREE_NODROP )
{
	// disable drag/drop.
	EnableDragDrop(FALSE);

	m_pImageList = NULL;
}

MachineLibTree::~MachineLibTree(void)
{
	delete m_pImageList;
}

void MachineLibTree::Init(MachineLibNodePtr pMacLibRootNode)
{
	/************************************************************************/
	// init the icons for the tree node.

	m_pImageList = new CImageList();
	m_pImageList->Create(IDB_MAC_MGR_TREE, 16, 10, RGB(192, 192, 192));
	SetImageList(m_pImageList, TVSIL_NORMAL);
	SkinXTreeCtrl::Init();
	/************************************************************************/

	/************************************************************************/
	// append the root node.

	CString strRootName;
	strRootName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MAC_LIST);

	// create tree node.
	TVINSERTSTRUCT rootItem;
	rootItem.hParent = NULL;
	rootItem.item.iImage = 0;
	rootItem.item.iSelectedImage = 0;
	rootItem.item.pszText = strRootName.AllocSysString();
	rootItem.item.cchTextMax = MAXLENGTH_STR;
	rootItem.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	rootItem.hInsertAfter = TVI_LAST;

	// set the node data.
	IMachineLibNode* pTmpNode = pMacLibRootNode.get();
	MachineLibNode* pRootNode = new MachineLibNode(pTmpNode->GetNodeName(), pTmpNode->GetMacLibNodeType(), pTmpNode->GetDataID());
	rootItem.item.lParam = (LPARAM)pRootNode;

	HTREEITEM hRootNode = InsertItem(&rootItem);
	/************************************************************************/

	// append each mac item node.
	MachineLibNodeListPtr pSubNodeList = pMacLibRootNode->GetSubNodeList();
	for (unsigned int i = 0; i < pSubNodeList->size(); i++)
	{
		MachineLibNodePtr pMacLibItemNode = pSubNodeList->at(i);
		AppendExpLibItemNode(hRootNode, pMacLibItemNode);
	}

	// expand the root node.
	Expand(hRootNode, TVE_EXPAND);
}

HTREEITEM MachineLibTree::AppendExpLibItemNode(HTREEITEM hRootItem, MachineLibNodePtr pMacLibItemNode)
{
	HTREEITEM hMacLibItemNode;

	/************************************************************************/
	// append the second layer.

	// create tree node.
	TVINSERTSTRUCT macLibItemNode;
	macLibItemNode.hParent = hRootItem;
	macLibItemNode.item.iImage = 1;
	macLibItemNode.item.iSelectedImage = 1;
	macLibItemNode.item.pszText = pMacLibItemNode->GetNodeName().AllocSysString();
	macLibItemNode.item.cchTextMax = MAXLENGTH_STR;
	macLibItemNode.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	macLibItemNode.hInsertAfter = TVI_LAST;

	// set the node data.
	IMachineLibNode* pTmpNode = pMacLibItemNode.get();
	MachineLibNode* pMacLibNode = new MachineLibNode(pTmpNode->GetNodeName(), pTmpNode->GetMacLibNodeType(), pTmpNode->GetDataID());
	macLibItemNode.item.lParam = (LPARAM)pMacLibNode;

	hMacLibItemNode = InsertItem(&macLibItemNode);
	/************************************************************************/

	/************************************************************************/
	// append the third layer.

	MachineLibNodeListPtr pSubNodeList = pMacLibItemNode->GetSubNodeList();
	for (unsigned int i = 0; i < pSubNodeList->size(); i++)
	{
		MachineLibNodePtr pNode = pSubNodeList->at(i);

		TVINSERTSTRUCT itemNode;
		itemNode.hParent = hMacLibItemNode;
		itemNode.item.pszText = pNode->GetNodeName().AllocSysString();
		itemNode.item.cchTextMax = MAXLENGTH_STR;
		itemNode.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		itemNode.hInsertAfter = TVI_LAST;

		// set the node data.
		IMachineLibNode* pTmpNode = pNode.get();
		MachineLibNode* pExpLibNode = new MachineLibNode(pTmpNode->GetNodeName(), pTmpNode->GetMacLibNodeType(), pTmpNode->GetDataID());
		itemNode.item.lParam = (LPARAM)pExpLibNode;

		// something special.
		if (pNode->GetMacLibNodeType() == MACLIB_NODE_MAC_PARAM)
		{
			itemNode.item.iImage = 2;
			itemNode.item.iSelectedImage = 2;
		}
		else if (pNode->GetMacLibNodeType() == MACLIB_NODE_MAC_COND)
		{
			itemNode.item.iImage = 3;
			itemNode.item.iSelectedImage = 3;
		}
		else if (pNode->GetMacLibNodeType() == MACLIB_NODE_MAC_NCCONFIG)
		{
			itemNode.item.iImage = 4;
			itemNode.item.iSelectedImage = 4;
		}

		// append this node.
		InsertItem(&itemNode);
	}
	/************************************************************************/

	return hMacLibItemNode;
}

END_CUTLEADER_NAMESPACE()
