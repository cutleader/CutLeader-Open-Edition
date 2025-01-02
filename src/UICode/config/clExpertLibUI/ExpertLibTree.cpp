#include "StdAfx.h"
#include "ExpertLibTree.h"

#include "baseConst.h"
#include "taoresource.h"

#include "ExpertLibNodeList.h"
#include "ExpertLibNode.h"
#include "clExpertLibResource.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(ExpertLibTree, XTreeCtrl)

ExpertLibTree::ExpertLibTree(void)
	: XTreeCtrl(IDC_CURSOR_TREE_COPYDROP, IDC_CURSOR_TREE_MOVEDROP, IDC_CURSOR_TREE_NODROP)
{
	// disable drag/drop.
	EnableDragDrop(FALSE);

	m_pImageList = NULL;
}

ExpertLibTree::~ExpertLibTree(void)
{
	delete m_pImageList;
}

void ExpertLibTree::Init(ExpLibNodePtr pExpLibRootNode, LONGLONG iInitialParamConfigID)
{
	// 树控件用到的图标。
	m_pImageList = new CImageList();
	m_pImageList->Create(IDB_EXPERTLIB_TREE, 16, 10, RGB(192, 192, 192));
	SetImageList(m_pImageList, TVSIL_NORMAL);

	// 初始化树节点。
	ExpertLibNodeListPtr pExpLibItemNodeList = pExpLibRootNode->GetSubNodeList();
	for (unsigned int i = 0; i < pExpLibItemNodeList->size(); i++)
	{
		ExpLibNodePtr pExpLibItemNode = pExpLibItemNodeList->at(i);
		AppendExpLibItemNode(pExpLibItemNode, iInitialParamConfigID);
	}
}

HTREEITEM ExpertLibTree::AppendExpLibItemNode(ExpLibNodePtr pExpLibItemNode, LONGLONG iInitialParamConfigID)
{
	HTREEITEM hExpLibItemNode;

	/************************************************************************/
	// 挂上“专家库项”节点。

	// create tree node.
	TVINSERTSTRUCT expLibItemNode;
	expLibItemNode.hParent = NULL;
	expLibItemNode.item.iImage = 0;
	expLibItemNode.item.iSelectedImage = 0;
	expLibItemNode.item.pszText = pExpLibItemNode->GetNodeName().AllocSysString();
	expLibItemNode.item.cchTextMax = MAXLENGTH_STR;
	expLibItemNode.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	expLibItemNode.hInsertAfter = TVI_LAST;

	// set the node data.
	IExpertLibNode* pTmpNode = pExpLibItemNode.get();
	ExpertLibNode* pExpLibNode = new ExpertLibNode(pTmpNode->GetNodeName(), pTmpNode->GetNodeType(), pTmpNode->GetDataID());
	expLibItemNode.item.lParam = (LPARAM)pExpLibNode;

	hExpLibItemNode = InsertItem(&expLibItemNode);
	/************************************************************************/


	// 挂上“参数配置”节点。
	ExpertLibNodeListPtr pParamConfigNodeList = pExpLibItemNode->GetSubNodeList();
	for (unsigned int i = 0; i < pParamConfigNodeList->size(); i++)
	{
		ExpLibNodePtr pParamConfigNode = pParamConfigNodeList->at(i);

		// append the "part schemes" node.
		TVINSERTSTRUCT paramConfigNode;
		paramConfigNode.hParent = hExpLibItemNode;
		paramConfigNode.item.iImage = 1;
		paramConfigNode.item.iSelectedImage = 1;
		paramConfigNode.item.pszText = pParamConfigNode->GetNodeName().AllocSysString();
		paramConfigNode.item.cchTextMax = MAXLENGTH_STR;
		paramConfigNode.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		paramConfigNode.hInsertAfter = TVI_LAST;

		// set the node data.
		pTmpNode = pParamConfigNode.get();
		pExpLibNode = new ExpertLibNode(pTmpNode->GetNodeName(), pTmpNode->GetNodeType(), pTmpNode->GetDataID());
		paramConfigNode.item.lParam = (LPARAM)pExpLibNode;

		HTREEITEM hParamConfigNode = InsertItem(&paramConfigNode);

		if (pTmpNode->GetDataID() == iInitialParamConfigID)
		{
			SelectItem(hParamConfigNode);
		}
	}

	// expand the "expert library item"
	Expand(hExpLibItemNode, TVE_EXPAND);

	return hExpLibItemNode;
}

END_CUTLEADER_NAMESPACE()
