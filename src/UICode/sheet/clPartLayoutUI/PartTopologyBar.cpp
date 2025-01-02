#include "StdAfx.h"
#include "PartTopologyBar.h"

#include "GlViewPort.h"

#include "PartPlacement.h"
#include "PartPlacementDrawer.h"
#include "clPartLayoutResource.h"
#include "PartInstance.h"
#include "PartInstanceList.h"
#include "PartInstanceSelectAction.h"
#include "PartLayoutEditor.h"

BEGIN_CUTLEADER_NAMESPACE()

PartTopologyBar* PartTopologyBar::m_pPartTopBar = NULL;

PartTopologyBar::PartTopologyBar(void)
{
}

PartTopologyBar::~PartTopologyBar(void)
{
}

PartTopologyBar* PartTopologyBar::GetInstance(CWnd* pParent)
{
	if (m_pPartTopBar == NULL)
	{
		m_pPartTopBar = new PartTopologyBar();
		m_pPartTopBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VIEWBAR_PARTTOP);

		if (!m_pPartTopBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_VIEWBAR_PARTTOP, 
								   WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI))
			return NULL;
	}

	return m_pPartTopBar;
}

BEGIN_MESSAGE_MAP(PartTopologyBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW_PARTTOP, OnSelchangedTree)

END_MESSAGE_MAP()

int PartTopologyBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// create the part placement tree.
	if (!CreatePartTopTree())
		return -1;

	AdjustLayout();

	return 0;
}

void PartTopologyBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void PartTopologyBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_partTopTree.SetFocus();
}

void PartTopologyBar::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// get the selected part inst list.
	PartInstanceListPtr pSelPartInstanceList = m_partTopTree.GetSelPartInstanceList();

	// select the part inst.
	m_pPartLayoutEditor->SetEditAction(EDIT_SELECTPRTINST);
	PartInstanceSelectActionPtr pPartInstSelectAction = boost::dynamic_pointer_cast<PartInstanceSelectAction>(m_pPartLayoutEditor->GetActionObject());
	pPartInstSelectAction->SelectPartInsts(pSelPartInstanceList);

	// redraw the view
	CWnd* pView = GetRelativeView();
	pView->Invalidate();
}

void PartTopologyBar::DisplayPartTop(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList, PartLayoutEditorPtr pPartLayoutEditor)
{
	m_pPartInstanceTopologyItemList = pPartInstanceTopologyItemList;
	m_pPartLayoutEditor = pPartLayoutEditor;
	m_partTopTree.DisplayPartTop(m_pPartInstanceTopologyItemList);

	if (!IsVisible())
		ShowPane(TRUE, FALSE, TRUE);
}

void PartTopologyBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	rectCombo.SetRectEmpty();
	int cyCmb = rectCombo.Size().cy;
	m_partTopTree.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, 
							   rectClient.Width(), rectClient.Height() - cyCmb, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL PartTopologyBar::CreatePartTopTree()
{
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_LINESATROOT | 
							  TVS_HASBUTTONS | WS_BORDER;

	if (!m_partTopTree.Create(dwTreeStyle, rectDummy, this, IDC_TREEVIEW_PARTTOP))
		return FALSE;

	m_partTopTree.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
