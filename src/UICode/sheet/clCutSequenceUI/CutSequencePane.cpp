#include "stdafx.h"
#include "CutSequencePane.h"

#include "XMenu.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "CommandManager.h"

#include "ICutSequence.h"
#include "CutSequenceList.h"
#include "CutSequenceSelectAction.h"
#include "clCutSequenceResource.h"
#include "SequenceEditor.h"
#include "CutSequenceDeleteCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

CutSequencePane* CutSequencePane::m_pSeqUnitPane = NULL;

BEGIN_MESSAGE_MAP(CutSequencePane, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()

	ON_CBN_SELCHANGE(IDC_CUTSEQ_PANE_COMBO, OnSelchangeCombo)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW_SEQUNIT, OnSelchangedTree)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL_SEQUNIT, OnItemchangedList)
END_MESSAGE_MAP()

CutSequencePane::CutSequencePane()
{
}

CutSequencePane::~CutSequencePane()
{
}

CutSequencePane* CutSequencePane::GetInstance(CWnd* pParent)
{
	if (m_pSeqUnitPane == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEW_SEQUNIT);

		m_pSeqUnitPane = new CutSequencePane();
		m_pSeqUnitPane->m_pMainWnd = pParent;
		if (!m_pSeqUnitPane->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200), TRUE, IDC_SEQ_VIEW_SEQUNIT, 
									WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			return NULL;
		}
	}

	return m_pSeqUnitPane;
}

int CutSequencePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the combo box.
	if (!CreateComboBox())
		return -1;

	// create the sequence unit tree.
	if (!CreateSeqUnitTree())
		return -1;

	// create the sequence unit grid.
	if (!CreateSeqUnitGrid())
		return -1;

	// layout the windows in this pane.
	AdjustLayout();

	return 0;
}

void CutSequencePane::OnSelchangeCombo()
{
	if (m_comboBox.GetCurSel() == 0)
	{
		// update the tree.
		m_seqUnitTree.DisplaySeqUnitTree(m_pCutSequences);
		m_seqUnitTree.ShowWindow(SW_SHOW);

		// hide the grid.
		m_seqUnitGrid.ShowWindow(SW_HIDE);
	}
	else
	{
		// update the grid.
		m_seqUnitGrid.DisplaySeqUnitGrid(m_pCutSequences);
		m_seqUnitGrid.ShowWindow(SW_SHOW);

		// hide the tree.
		m_seqUnitTree.ShowWindow(SW_HIDE);
	}
}

void CutSequencePane::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_bDuringInit)
		return;

	// get the selected sequence units.
	CutSequenceListPtr pSelSeqUnitList = m_seqUnitTree.GetSelCutSeqs();

	// select the sequence units.
	if (m_pSequenceEditor->GetActionObject()->GetActionType() != SEQUENCE_SELECT)
		m_pSequenceEditor->SetAction(SEQUENCE_SELECT);
	CutSequenceSelectActionPtr pSeqSelectAction = boost::dynamic_pointer_cast<CutSequenceSelectAction>(m_pSequenceEditor->GetActionObject());
	pSeqSelectAction->SelectedSeqUnit(pSelSeqUnitList);

	// redraw the view.
	CWnd* pView = GetRelativeView();
	pView->Invalidate();
}

void CutSequencePane::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bDuringInit)
		return;

	// get the selected sequence units.
	CutSequenceListPtr pSelSeqUnitList = m_seqUnitGrid.GetSelCutSeqs();

	// select the sequence units.
	m_pSequenceEditor->SetAction(SEQUENCE_SELECT);
	CutSequenceSelectActionPtr pSeqSelectAction = 
		boost::dynamic_pointer_cast<CutSequenceSelectAction>(m_pSequenceEditor->GetActionObject());
	pSeqSelectAction->SelectedSeqUnit(pSelSeqUnitList);

	// redraw the view.
	CWnd* pView = GetRelativeView();
	pView->Invalidate();
}

void CutSequencePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// layout the windows in this pane.
	AdjustLayout();
}

void CutSequencePane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_seqUnitTree.SetFocus();
}

void CutSequencePane::Update(int iEventID, const CComVariant& varEventContent)
{
	if (IsVisible())
	{
		m_bDuringInit = TRUE;

		m_seqUnitTree.DisplaySeqUnitTree(m_pCutSequences);
		m_seqUnitGrid.DisplaySeqUnitGrid(m_pCutSequences);

		m_bDuringInit = FALSE;
	}
}

void CutSequencePane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);
	m_comboBox.GetWindowRect(&rectCombo);

	/************************************************************************/
	// adjust the position of the windows.

	// the combo box.
	m_comboBox.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, 
							SWP_NOACTIVATE | SWP_NOZORDER);

	// the sequence unit tree.
	m_seqUnitTree.SetWindowPos(NULL, rectClient.left, rectClient.top+rectCombo.Size().cy, rectClient.Width(), 
							   rectClient.Height()-rectCombo.Size().cy, SWP_NOACTIVATE | SWP_NOZORDER);

	// the sequence unit grid.
	m_seqUnitGrid.SetWindowPos(NULL, rectClient.left, rectClient.top+rectCombo.Size().cy, rectClient.Width(), 
							   rectClient.Height()-rectCombo.Size().cy, SWP_NOACTIVATE | SWP_NOZORDER);
	/************************************************************************/
}

BOOL CutSequencePane::CreateComboBox()
{
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwViewStyle = WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_BORDER|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	if (!m_comboBox.Create(dwViewStyle, rectDummy, this, IDC_CUTSEQ_PANE_COMBO))
	{
		return FALSE;
	}

	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEW_BY_TYPE);
	m_comboBox.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEW_BY_ORDER);
	m_comboBox.AddString(str);

	m_comboBox.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	m_comboBox.SetCurSel(0);

	return TRUE;
}

BOOL CutSequencePane::CreateSeqUnitGrid()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
	if (!m_seqUnitGrid.Create(dwStyle, rectDummy, this, IDC_LISTCTRL_SEQUNIT))
		return FALSE;

	return TRUE;
}

BOOL CutSequencePane::CreateSeqUnitTree()
{
	CRect rectDummy (0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD|WS_VISIBLE|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_LINESATROOT|TVS_HASBUTTONS|WS_BORDER;
	if (!m_seqUnitTree.Create(dwTreeStyle, rectDummy, this, IDC_TREEVIEW_SEQUNIT))
		return FALSE;

	m_seqUnitTree.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	return TRUE;
}

void CutSequencePane::DisplaySeqUnit(CutSequenceListPtr pCutSequences, SequenceEditorPtr pSequenceEditor)
{
	m_pCutSequences = pCutSequences;
	m_pSequenceEditor = pSequenceEditor;

	m_bDuringInit = TRUE;

	// init the tree and the grid.
	m_seqUnitTree.DisplaySeqUnitTree(m_pCutSequences);
	m_seqUnitGrid.DisplaySeqUnitGrid(m_pCutSequences);

	m_bDuringInit = FALSE;

	// CutSequencePane will observe the sequence unit list.
	pCutSequences->AddObserver(this);

	if (!IsVisible())
		ShowPane(TRUE, FALSE, TRUE);
}

END_CUTLEADER_NAMESPACE()
