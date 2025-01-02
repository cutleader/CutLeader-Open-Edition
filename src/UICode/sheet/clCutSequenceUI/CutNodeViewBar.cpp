#include "stdafx.h"
#include "CutNodeViewBar.h"

#include "clUtilityResource.h"
#include "GlViewPort.h"
#include "clUILibResource.h"

#include "CutNodeList.h"
#include "ISequenceSimulateShape.h"
#include "SequenceSimulateShapeList.h"
#include "clCutSequenceResource.h"
#include "CutNodeManager.h"
#include "CutSequenceDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

CutNodeViewBar* CutNodeViewBar::m_pCutNodeViewBar = NULL;

BEGIN_MESSAGE_MAP(CutNodeViewBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCTRL_CUTNODE, OnItemchangedList)

END_MESSAGE_MAP()

CutNodeViewBar::CutNodeViewBar()
{
}

CutNodeViewBar::~CutNodeViewBar()
{
}

// get the singleton object
CutNodeViewBar* CutNodeViewBar::GetInstance(CWnd* pParent)
{
	if (m_pCutNodeViewBar == NULL)
	{
		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_VIEW_CUTNODE);

		m_pCutNodeViewBar = new CutNodeViewBar();
		m_pCutNodeViewBar->m_pMainWnd = pParent;
		if (!m_pCutNodeViewBar->Create(str, (CWnd*)pParent, CRect(0, 0, RIGHT_DOCK_PANE_WIDTH, 200),
									   TRUE, IDC_SEQ_VIEW_CUTNODE, 
									   WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
			return NULL;
	}

	return m_pCutNodeViewBar;
}

/////////////////////////////////////////////////////////////////////////////

int CutNodeViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	//
	CreateCutNodeGrid();

	// layout the windows in this pane.
	AdjustLayout();

	return 0;
}

void CutNodeViewBar::OnDestroy()
{
	ReleaseGridItemData();
	XBarBase::OnDestroy();
}

void CutNodeViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// layout the windows in this pane.
	AdjustLayout();
}

void CutNodeViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}

void CutNodeViewBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient, rectCombo;
	GetClientRect(rectClient);

	rectCombo.SetRectEmpty();
	int cyCmb = rectCombo.Size().cy;

	m_cutNodeListGrid.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), rectClient.Height() - cyCmb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CutNodeViewBar::InitBar(CutNodeListPtr pCutNodeList, GlViewPortPtr pViewPort)
{
	m_pCutNodeList = pCutNodeList;
	m_pViewPort = pViewPort;

	//
	InitCutNodeGrid();
}

void CutNodeViewBar::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem >= 0)
	{
		LONGLONG* pCutNodeID = (LONGLONG*)m_cutNodeListGrid.GetItemData(pNMListView->iItem);
		ICutNodePtr pCutNode = m_pCutNodeList->GetCutNodeByID(*pCutNodeID);

		// 绘制切割节点。
		SequenceSimulateShapeListPtr pSeqSimShapeList = CutNodeManager::CutNode2SimShape(pCutNode.get());
		if (pSeqSimShapeList->size() > 0)
		{
			CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
			DisplayParamPtr pColorInfo = pCLOptionInfo->GetDisplayParam();

			COLORREF dwOldColor;
			m_pViewPort->GetDrawColor(dwOldColor);
			ROP_MODE ropMode;
			m_pViewPort->GetROP(ropMode);

			// 绘制模拟图形。
			CutSequenceDrawer cutSequenceDrawer(m_pViewPort);
			m_pViewPort->SetROP(ROP_COPY);
			m_pViewPort->SetDrawColor(*pColorInfo->GetSelectObjColor());
			cutSequenceDrawer.DrawSequenceSimulateShapes(pSeqSimShapeList.get());

			// restore the drawer
			m_pViewPort->SetROP(ropMode);
			m_pViewPort->SetDrawColor(dwOldColor);

			m_pViewPort->SwapBuffers();
		}
	}

	*pResult = 0;
}

BOOL CutNodeViewBar::CreateCutNodeGrid()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// create the listctrl.
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
	if (!m_cutNodeListGrid.Create(dwStyle, rectDummy, this, IDC_LISTCTRL_CUTNODE))
		return FALSE;

	m_cutNodeListGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	m_cutNodeListGrid.InsertColumn(0, str, LVCFMT_LEFT, 40);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTNODE);
	m_cutNodeListGrid.InsertColumn(1, str, LVCFMT_LEFT, 180);

	return TRUE;
}

void CutNodeViewBar::InitCutNodeGrid()
{
	ReleaseGridItemData();
	m_cutNodeListGrid.DeleteAllItems();

	for (unsigned int i = 0; i < m_pCutNodeList->size(); i++)
	{
		ICutNodePtr pCutNode = m_pCutNodeList->at(i);

		// insert an item.
		CString str;
		str.Format(_T("%d"), i+1);
		m_cutNodeListGrid.InsertItem(i, str);

		//
		if (pCutNode->GetCutNodeType() == CutNode_CutLine)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LINE_CUTNODE);
		else if (pCutNode->GetCutNodeType() == CutNode_CutArc)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ARC_CUTNODE);
		else if (pCutNode->GetCutNodeType() == CutNode_TraceLine)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TRACE_CUTNODE);
		else if (pCutNode->GetCutNodeType() == CutNode_DoPierce)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DOPIERCE_CUTNODE);
		else if (pCutNode->GetCutNodeType() == CutNode_UnPierce)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_UNPIERCE_CUTNODE);
        else if (pCutNode->GetCutNodeType() == CutNode_SelectTool)
            str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SELECTTOOL_CUTNODE);
		else if (pCutNode->GetCutNodeType() == CutNode_LoopInfo)
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LOOPINFO_CUTNODE);
        else if (pCutNode->GetCutNodeType() == CutNode_CoolingPoint)
        {
            str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CoolingPoint_CutNode);
        }
		m_cutNodeListGrid.SetItemText(i, 1, str);

		m_cutNodeListGrid.SetItemData(i, (DWORD_PTR)(new LONGLONG(pCutNode->GetID())));
	}
}

void CutNodeViewBar::ReleaseGridItemData()
{
	for (int i = 0; i < m_cutNodeListGrid.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_cutNodeListGrid.GetItemData(i);

		// delete the data stored in the grid item.
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()
