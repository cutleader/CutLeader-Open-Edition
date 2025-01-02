#include "stdafx.h"
#include "TaskCommonFunctionPanel.h"

#include "ClFrameResource.h"
#include "ClUtilityResource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_CUTLEADER_NAMESPACE()

TaskCommonFunctionPanel* TaskCommonFunctionPanel::m_pTaskCommonFunctionPanel = NULL;

BEGIN_MESSAGE_MAP(TaskCommonFunctionPanel, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

TaskCommonFunctionPanel::TaskCommonFunctionPanel()
{
}

TaskCommonFunctionPanel::~TaskCommonFunctionPanel()
{
}

TaskCommonFunctionPanel* TaskCommonFunctionPanel::GetInstance(CWnd* pParent)
{
	if (m_pTaskCommonFunctionPanel == NULL)
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_ToolBar);
		m_pTaskCommonFunctionPanel = new TaskCommonFunctionPanel();
		m_pTaskCommonFunctionPanel->m_pMainWnd = pParent;
		m_pTaskCommonFunctionPanel->Create(str, (CWnd*)pParent, CRect(0, 0, 50, 200), TRUE, IDC_TaskCommonFunctionBar, 
			WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	}

	return m_pTaskCommonFunctionPanel;
}

int TaskCommonFunctionPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the sub dialog.
	m_sheetFunctionToolBar.Create(IDD_clFrame_SheetFunctionToolBar, this);
	m_canvasFunctionToolBar.Create(IDD_clFrame_CanvasFunctionToolBar, this);

	return 0;
}

void TaskCommonFunctionPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// position the sub dialog
	CRect rectClient;
	GetClientRect(rectClient);
	m_sheetFunctionToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	m_canvasFunctionToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void TaskCommonFunctionPanel::ShowCanvasFunctionToolBar()
{
	m_sheetFunctionToolBar.ShowWindow(SW_HIDE);
	m_canvasFunctionToolBar.ShowWindow(SW_SHOW);
}

void TaskCommonFunctionPanel::ShowSheetFunctionToolBar()
{
	m_canvasFunctionToolBar.ShowWindow(SW_HIDE);
	m_sheetFunctionToolBar.ShowWindow(SW_SHOW);
}

END_CUTLEADER_NAMESPACE()
