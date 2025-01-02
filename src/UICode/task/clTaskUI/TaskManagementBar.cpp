#include "StdAfx.h"
#include "TaskManagementBar.h"

#include "baseConst.h"
#include "MultiLanguageMgr.h"
#include "DebugWarner.h"

#include "ClTaskResource.h"
#include "TaskPartInfoDlg.h"
#include "TaskSheetInfoDlg.h"

using namespace std;


BEGIN_CUTLEADER_NAMESPACE()

TaskManagementBar* TaskManagementBar::m_pInteractBar = NULL;

TaskManagementBar::TaskManagementBar(void)
{
}

TaskManagementBar::~TaskManagementBar(void)
{
}

// get the singleton object
TaskManagementBar* TaskManagementBar::GetInstance(CWnd* pParent)
{
	if (m_pInteractBar == NULL)
	{
		m_pInteractBar = new TaskManagementBar();
		m_pInteractBar->m_pMainWnd = pParent;

		CString str;
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TaskManagement);
		if (!m_pInteractBar->Create(str, pParent, CRect(0, 0, 190, 190), TRUE, IDC_TaskManagementBar, 
								    WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_BOTTOM|CBRS_FLOAT_MULTI))
		{
			return NULL;
		}
	}

	return m_pInteractBar;
}

BEGIN_MESSAGE_MAP(TaskManagementBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
    ON_MESSAGE(WM_reDisplayTaskParts, OnReDisplayTaskParts)
    ON_MESSAGE(WM_reDisplayTaskSheets, OnReDisplayTaskSheets)
END_MESSAGE_MAP()

int TaskManagementBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

    // create the sub dialog.
    m_taskPartManagementDlg.Create(IDD_ClTask_TaskPartManagement, this);
    m_taskSheetManagementDlg.Create(IDD_ClTask_TaskSheetManagement, this);

	return 0;
}

void TaskManagementBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

    CRect rectClient;
    GetClientRect(rectClient);

    double dWidthFactor_1 = 7.0 / 10.0;
    double dWidthFactor_2 = 3.0 / 10.0;

    // 布局“任务零件管理对话框”。
    {
        m_taskPartManagementDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width() * dWidthFactor_1, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        m_taskPartManagementDlg.ShowWindow(SW_SHOW);
    }

    // 布局“任务板材管理对话框”。
    {
        m_taskSheetManagementDlg.SetWindowPos(NULL, rectClient.left + rectClient.Width() * dWidthFactor_1, rectClient.top,
            rectClient.Width() * dWidthFactor_2, rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        m_taskSheetManagementDlg.ShowWindow(SW_SHOW);
    }
}

LRESULT TaskManagementBar::OnReDisplayTaskParts(WPARAM wParam, LPARAM lParam)
{
    m_taskPartManagementDlg.DisplayAllTaskParts(GetTaskItem());

    return 0;
}

LRESULT TaskManagementBar::OnReDisplayTaskSheets(WPARAM wParam, LPARAM lParam)
{
    m_taskSheetManagementDlg.DisplayAllSheets(GetTaskItem());

    return 0;
}

void TaskManagementBar::UpdateTaskItem(TaskItemPtr pTaskItem)
{
    m_taskPartManagementDlg.DisplayAllTaskParts(pTaskItem);
    m_taskSheetManagementDlg.DisplayAllSheets(pTaskItem);
}

void TaskManagementBar::UpdateTaskPartsOnPanel()
{
    m_taskPartManagementDlg.UpdateTaskPartsOnPanel();
}

void TaskManagementBar::RedrawSheetThumbnail(LONGLONG iSheetID)
{
    m_taskSheetManagementDlg.RedrawSheetThumbnail(iSheetID);
}

TaskItemPtr TaskManagementBar::GetTaskItem() const
{
    return m_taskPartManagementDlg.GetTaskItem();
}

CWnd* TaskManagementBar::GetTaskPartInfoDlg(int iDialogIndex)
{
	const TaskPartInfoListDlg* pTaskPartInfoListDlg = m_taskPartManagementDlg.GetTaskPartInfoListDlg();
	vector<TaskPartInfoDlgPtr> taskPartInfoDlgs = pTaskPartInfoListDlg->GetTaskPartSubDlgs();
	if (taskPartInfoDlgs.size() < iDialogIndex + 1)
	{
		DebugWarner::Warn(_T("57950"));
		return 0;
	}

	return taskPartInfoDlgs[iDialogIndex].get();
}

CWnd* TaskManagementBar::GetTaskSheetInfoDlg(int iDialogIndex)
{
	const TaskSheetInfoListDlg* pTaskSheetInfoListDlg = m_taskSheetManagementDlg.GetTaskSheetInfoListDlg();
	vector<TaskSheetInfoDlgPtr> taskSheetInfoDlgs = pTaskSheetInfoListDlg->GetSheetSubDlgs();
	if (taskSheetInfoDlgs.size() < iDialogIndex + 1)
	{
		DebugWarner::Warn(_T("54886"));
		return 0;
	}

	return taskSheetInfoDlgs[iDialogIndex].get();
}

END_CUTLEADER_NAMESPACE()
