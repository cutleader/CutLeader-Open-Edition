#include "StdAfx.h"
#include "PartTopologyTree.h"

#include "baseConst.h"
#include "taoresource.h"

#include "PartItem.h"
#include "Part.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "PartPlacement.h"
#include "PartInstanceTopologyItem.h"
#include "PartInstanceTopologyItemList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(PartTopologyTree, XTreeCtrl)

BEGIN_MESSAGE_MAP(PartTopologyTree, XTreeCtrl)
	ON_WM_CREATE()

END_MESSAGE_MAP()

PartTopologyTree::PartTopologyTree(void)
		: XTreeCtrl(IDC_CURSOR_TREE_COPYDROP, IDC_CURSOR_TREE_MOVEDROP, IDC_CURSOR_TREE_NODROP)
{
	// disable drag/drop.
	EnableDragDrop(FALSE);

	// enable multiple selection.
	SetMultiSelect(TRUE);
}

PartTopologyTree::~PartTopologyTree(void)
{
}

int PartTopologyTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (XTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void PartTopologyTree::DisplayPartTop(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList)
{
	m_pPartInstanceTopologyItemList = pPartInstanceTopologyItemList;

	// delete tree nodes at first
	DeleteAllItems();

	AppendTopNodes(NULL, m_pPartInstanceTopologyItemList);
}

PartInstanceListPtr PartTopologyTree::GetSelPartInstanceList()
{
	PartInstanceListPtr pPartInstances(new PartInstanceList);

	// get the selected tree nodes.
	vector<HTREEITEM> selectTreeNodes = GetSelectedTreeItems();
	
	// get select part inst list.
	for (unsigned int i = 0; i < selectTreeNodes.size(); i++)
	{
		// the part inst ID.
		HTREEITEM hTreeItem = selectTreeNodes.at(i);
		LONGLONG* pPartInstID = (LONGLONG*)GetItemData(hTreeItem);

		PartInstancePtr pPartInstance = m_pPartInstanceTopologyItemList->GetPartInstanceByID(*pPartInstID);
		pPartInstances->push_back(pPartInstance);
	}

	return pPartInstances;
}

void PartTopologyTree::AppendTopNodes(HTREEITEM hParentNode, PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList)
{
	for (unsigned int i = 0; i < pPartInstanceTopologyItemList->size(); i++)
	{
		PartInstanceTopologyItemPtr pPartInstanceTopologyItem = pPartInstanceTopologyItemList->at(i);

		// some info of this part top item.
		PartInstancePtr pPartInstance = pPartInstanceTopologyItem->GetPartInstance();
		CString strPartName = pPartInstance->GetPartPlacement()->GetPart()->GetName();
		CString strPartInstID;
		strPartInstID.Format(_T("%I64d"), pPartInstance->GetID());

		// composite the display name.
		CString strDisplayName = strPartName;
		strDisplayName += _T("[");
		strDisplayName += strPartInstID;
		strDisplayName += _T("]");

		// append this item.
		TVINSERTSTRUCT itemNode;
		itemNode.hParent = hParentNode;
		itemNode.item.pszText = strDisplayName.AllocSysString();
		itemNode.item.cchTextMax = MAXLENGTH_STR;
		itemNode.item.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
		itemNode.hInsertAfter = TVI_LAST;
		HTREEITEM hItemNode = InsertItem(&itemNode);
		SetItemData(hItemNode, (DWORD_PTR)(new LONGLONG(pPartInstance->GetID())));

		// append the child node of "hItemNode".
		AppendTopNodes(hItemNode, pPartInstanceTopologyItem->GetSubItemList());
	}
}

END_CUTLEADER_NAMESPACE()
