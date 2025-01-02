#include "StdAfx.h"
#include "PartPlacementViewBar.h"

#include "XMenu.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "CommandManager.h"
#include "clGeometryFeatureResource.h"

#include "clPartLayoutResource.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementSelectAction.h"
#include "PartLayoutEditor.h"
#include "PartPlacementDeleteCommand.h"
#include "PartPlacementPropertyBar.h"
#include "PartPlacementTreeNodeData.h"

#include "ParamConfig.h"

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementViewBar* PartPlacementViewBar::m_pPartPmtViewBar = NULL;

PartPlacementViewBar::PartPlacementViewBar(void)
{
}

PartPlacementViewBar::~PartPlacementViewBar(void)
{
}

PartPlacementViewBar* PartPlacementViewBar::GetInstance(CWnd* pParent)
{
	if (m_pPartPmtViewBar == NULL)
	{
		m_pPartPmtViewBar = new PartPlacementViewBar();
		m_pPartPmtViewBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VIEWBAR_PARTPMT);
		if (!m_pPartPmtViewBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_VIEWBAR_PARTPMT, 
									   WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
			return NULL;
	}

	return m_pPartPmtViewBar;
}

BEGIN_MESSAGE_MAP(PartPlacementViewBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW_PARTPMT, OnSelchangedTree)
	ON_NOTIFY(NM_DBLCLK, IDC_TREEVIEW_PARTPMT, OnDblclkTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREEVIEW_PARTPMT, OnRClickTree)

	ON_COMMAND(IDC_PROPBAR_EDITPRTPMT, OnEditPartPmt)
END_MESSAGE_MAP()

int PartPlacementViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// create the part placement tree.
	if (!CreatePartPmtTree())
		return -1;

	AdjustLayout();

	return 0;
}

void PartPlacementViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void PartPlacementViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_partPmtTree.SetFocus();
}

void PartPlacementViewBar::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// get the selected part pmts.
	PartPlacementListPtr pSelectedPartPlacements = m_partPmtTree.GetSelPartPlacementList();

	// select the part pmt.
	m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTPMT);
	PartPlacementSelectActionPtr pPartPmtSelectAction = 
		boost::dynamic_pointer_cast<PartPlacementSelectAction>(m_pPartLayoutEditor->GetActionObject());
	pPartPmtSelectAction->SelectPartPlacements(pSelectedPartPlacements);

	// redraw the view
	CWnd* pView = GetRelativeView();
	pView->Invalidate();
}

void PartPlacementViewBar::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// get the data that is stored in the node.
	HTREEITEM hItem = m_partPmtTree.GetSelectedItem();
	if (!hItem)
		return;

	// get the item data.
	int iData = m_partPmtTree.GetItemData(hItem);
	PartPlacementTreeNodeData* pPmtTreeNodeData = (PartPlacementTreeNodeData*)iData;

	// enter the PartPlacementSelectAction for pattern node.
	if (pPmtTreeNodeData->GetPmtTreeNodeType() == PMT_TREE_NODE_PMT_ITEM)
	{
		// get the select part pmt.
		LONGLONG iPartPmtID = pPmtTreeNodeData->GetNodeData();
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->GetPartPlacementByID(iPartPmtID);

		/************************************************************************/		
		// select the part pmt and display the prop bar.

		// display the prop bar.
		PartPlacementListPtr pSelectedPartPlacements(new PartPlacementList);
		pSelectedPartPlacements->push_back(pPartPlacement);

		// select the part pmt.
		m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTPMT);
		PartPlacementSelectActionPtr pPartPmtSelectAction = 
			boost::dynamic_pointer_cast<PartPlacementSelectAction>(m_pPartLayoutEditor->GetActionObject());
		pPartPmtSelectAction->SelectPartPlacements(pSelectedPartPlacements);
		/************************************************************************/

		// redraw the view
		CWnd* pView = GetRelativeView();
		pView->Invalidate();
	}

	*pResult = 0;
}

void PartPlacementViewBar::OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString str;

	/************************************************************************/
	// check whether click on the item.

	// convert the coordinate from screen to client.
	POINT pos;
	if (!GetCursorPos(&pos))
		return;
	m_partPmtTree.ScreenToClient(&pos);

	// check the hit item.
	UINT uFlags;
	HTREEITEM hItem = m_partPmtTree.HitTest(pos, &uFlags);
	if ((hItem == NULL) || !(uFlags & TVHT_ONITEM))
		return;
	else
	{
		if (!m_partPmtTree.IsSelected(hItem))
		{
			m_partPmtTree.SelectAll(FALSE);
			m_partPmtTree.SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		}
	}
	/************************************************************************/

	/************************************************************************/
	// display the popup menu.

	// figure out the position where the menu will pop up.
	CRect rect;
	this->GetClientRect(&rect);
	ClientToScreen(&rect);
	m_partPmtTree.ClientToScreen(&pos);

	// display the popup.
	int iSelNodeCount = m_partPmtTree.GetSelectedCount();
	if (iSelNodeCount > 0)
	{
		// define the popup menu.
		XMenu xMenu;
		xMenu.CreatePopupMenu();

		// "property" sub menu.
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PROPERTY);
		xMenu.AppendMenu(MF_STRING, IDC_PROPBAR_EDITPRTPMT, str.AllocSysString());

		// display the menu.
		xMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, pos.x, pos.y, this);
	}
	/************************************************************************/
}

void PartPlacementViewBar::OnEditPartPmt()
{
	// get the selected part pmts.
	PartPlacementListPtr pSelectedPartPlacements = m_partPmtTree.GetSelPartPlacementList();

	// we should display and update PartPlacementPropertyBar.
	PartPlacementPropertyBar* pPartPmtPropBar = PartPlacementPropertyBar::GetInstance();
	pPartPmtPropBar->DisplayPartPmt(pSelectedPartPlacements, m_pPartInstanceList, m_pPartLayoutEditor->GetDrawer());
}

void PartPlacementViewBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	rectCombo.SetRectEmpty();
	int cyCmb = rectCombo.Size().cy;
	m_partPmtTree.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), rectClient.Height() - cyCmb, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL PartPlacementViewBar::CreatePartPmtTree()
{
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_HASBUTTONS | WS_BORDER;
	if (!m_partPmtTree.Create(dwTreeStyle, rectDummy, this, IDC_TREEVIEW_PARTPMT))
		return FALSE;

	m_partPmtTree.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	return TRUE;
}

void PartPlacementViewBar::DisplayPartPmt(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartLayoutEditorPtr pPartLayoutEditor)
{
	m_pPartPlacementList = pPartPlacements;
	m_pPartInstanceList = pPartInstances;
	m_pPartLayoutEditor = pPartLayoutEditor;

	m_partPmtTree.DisplayPartPmtTree(m_pPartPlacementList);

	// PartPlacementViewBar will observe the part pmt list.
	m_pPartPlacementList->AddObserver(this);

	if (!IsVisible())
		ShowPane(TRUE, FALSE, TRUE);
}

void PartPlacementViewBar::Update(int iEventID, const CComVariant& varEventContent)
{
	if (IsVisible())
	{
		// update the tree.
		m_partPmtTree.DisplayPartPmtTree(m_pPartPlacementList);
	}
}

END_CUTLEADER_NAMESPACE()
