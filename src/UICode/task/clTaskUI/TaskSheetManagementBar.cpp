#include "StdAfx.h"
#include "TaskSheetManagementBar.h"

#include "baseConst.h"
#include "MultiLanguageMgr.h"
#include "LogMgr.h"

#include "ClTaskResource.h"
#include "TaskPartInfoDlg.h"
#include "TaskSheetInfoDlg.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

TaskSheetManagementBar* TaskSheetManagementBar::m_pInteractBar = NULL;

TaskSheetManagementBar::TaskSheetManagementBar(void)
{
}

TaskSheetManagementBar::~TaskSheetManagementBar(void)
{
}

// get the singleton object
TaskSheetManagementBar* TaskSheetManagementBar::GetInstance(CWnd* pParent)
{
	if (m_pInteractBar == NULL)
	{
		m_pInteractBar = new TaskSheetManagementBar();
		m_pInteractBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TaskSheetManagement);
		if (!m_pInteractBar->Create(str, pParent, CRect(0, 0, 190, BOTTOM_DOCK_PANE_HEIGHT), TRUE, IDC_TaskSheetManagementBar,
								    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_BOTTOM|CBRS_FLOAT_MULTI))
		{
			return NULL;
		}
		m_pInteractBar->ShowBar(FALSE);
	}

	return m_pInteractBar;
}

BEGIN_MESSAGE_MAP(TaskSheetManagementBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int TaskSheetManagementBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

    // create the sub dialog.
    m_taskSheetManagementDlg.Create(IDD_clTaskUI_TaskSheetManagement, this);

	return 0;
}

void TaskSheetManagementBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

    CRect rectClient;
    GetClientRect(rectClient);

    // 布局“任务板材管理对话框”。
    {
        m_taskSheetManagementDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        m_taskSheetManagementDlg.ShowWindow(SW_SHOW);
    }
}

CWnd* TaskSheetManagementBar::GetTaskSheetInfoDlg(int iDialogIndex)
{
	const TaskSheetInfoListDlg* pTaskSheetInfoListDlg = m_taskSheetManagementDlg.GetTaskSheetInfoListDlg();
	vector<TaskSheetInfoDlgPtr> taskSheetInfoDlgs = pTaskSheetInfoListDlg->GetSheetSubDlgs();
	if (taskSheetInfoDlgs.size() < iDialogIndex + 1)
	{
		LogMgr::DebugWarn(_T("54886"));
		return 0;
	}

	return taskSheetInfoDlgs[iDialogIndex].get();
}

END_CUTLEADER_NAMESPACE()
