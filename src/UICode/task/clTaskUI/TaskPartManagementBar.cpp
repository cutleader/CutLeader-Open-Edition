#include "StdAfx.h"
#include "TaskPartManagementBar.h"

#include "baseConst.h"
#include "MultiLanguageMgr.h"
#include "LogMgr.h"

#include "ClTaskResource.h"
#include "TaskPartInfoDlg.h"
#include "TaskSheetInfoDlg.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

TaskPartManagementBar* TaskPartManagementBar::m_pTaskPartManagementBar = NULL;

TaskPartManagementBar::TaskPartManagementBar(void)
{
}

TaskPartManagementBar::~TaskPartManagementBar(void)
{
}

// get the singleton object
TaskPartManagementBar* TaskPartManagementBar::GetInstance(CWnd* pParent)
{
	if (m_pTaskPartManagementBar == NULL)
	{
		m_pTaskPartManagementBar = new TaskPartManagementBar();
		m_pTaskPartManagementBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TaskPartManagement);
		if (!m_pTaskPartManagementBar->Create(str, pParent, CRect(0, 0, 220, 190), TRUE, IDC_TaskPartManagementBar, 
								    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI))
		{
			return NULL;
		}
		m_pTaskPartManagementBar->ShowBar(FALSE, FALSE);
	}

	return m_pTaskPartManagementBar;
}

BEGIN_MESSAGE_MAP(TaskPartManagementBar, XBarBase)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int TaskPartManagementBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

    // create the sub dialog.
    m_taskPartManagementDlg.Create(IDD_clTaskUI_TaskPartManagement, this);

	return 0;
}

void TaskPartManagementBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

    CRect rectClient;
    GetClientRect(rectClient);

    // 布局“任务零件管理对话框”。
    {
        m_taskPartManagementDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        m_taskPartManagementDlg.ShowWindow(SW_SHOW);
    }
}

CWnd* TaskPartManagementBar::GetTaskPartInfoDlg(int iDialogIndex)
{
	const TaskPartInfoListDlg* pTaskPartInfoListDlg = m_taskPartManagementDlg.GetTaskPartInfoListDlg();
	vector<TaskPartInfoDlgPtr> taskPartInfoDlgs = pTaskPartInfoListDlg->GetTaskPartSubDlgs();
	if (taskPartInfoDlgs.size() < iDialogIndex + 1)
	{
		LogMgr::DebugWarn(_T("57950"));
		return 0;
	}

	return taskPartInfoDlgs[iDialogIndex].get();
}

void TaskPartManagementBar::ShowBar(BOOL bShow, BOOL bDelay)
{
	// 隐藏该面板时，需要清空那些豆腐块窗口，否则再次打开任务会存在刷新的问题。
	if (!bShow) {
		m_taskPartManagementDlg.GetTaskPartInfoListDlg()->DestroyAllSubWindows();
	}

	__super::ShowBar(bShow, bDelay);
}

END_CUTLEADER_NAMESPACE()
