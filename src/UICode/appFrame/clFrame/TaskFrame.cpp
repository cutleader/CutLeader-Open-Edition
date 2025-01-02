#include "stdafx.h"
#include "TaskFrame.h"

#include "ClWindowFinder.h"

#include "TaskView.h"
#include "SheetSubView.h"
#include "PartCommonFunctionPanel.h"
#include "TaskCommonFunctionPanel.h"
#include "UIManager.h"
#include "ClFrameResource.h"
#include "TaskPartManagementBar.h"
#include "TaskSheetManagementBar.h"
#include "TaskDocument.h"
#include "DataItemTracker.h"
#include "TaskItem.h"
#include "ChangeLoopToolPane.h"
#include "ClTaskResource.h"
#include "Sheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskFrame, CChildFrame)

BEGIN_MESSAGE_MAP(TaskFrame, CChildFrame)
    ON_WM_MDIACTIVATE()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


TaskFrame::TaskFrame()
{
	
}

TaskFrame::~TaskFrame()
{
}

void TaskFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
    CChildFrame::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

    if (bActivate)
    {
		// 显示CNC控制面板。
		ChangeLoopToolPanel::GetInstance()->ShowBar(TRUE, TRUE);

        // 显示零件和板材管理面板。
        TaskPartManagementBar::GetInstance()->ShowBar(TRUE, TRUE);
		TaskSheetManagementBar::GetInstance()->ShowBar(TRUE, TRUE);

        // 如果是从其他文档切换到这个任务文档的，那么需要进行一些更新。
        CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pMainFrame->GetActiveFrame();
        CDocument* pDocument = pChildFrame->GetActiveDocument();
        if (pDocument) // 是从其他文档切换到这个任务文档的。
        {
            TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pDocument);
            TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskDoc->GetTaskItem());
			TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskDoc->GetTaskItem());

			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);

			// 当从其他文档切换到任务文档时，需要根据当前任务打开的是底图还是板材，再对界面进行更新。此处之前是通过给任务视图发送
			// WM_UpdateTaskPartManagementPanel消息来实现的，道软对此做了重构，目前CCP中的消息机制一般用于下层模块想和上层模块通信，
			// 这种情况只能通过发消息来实现，用消息进行通信的优点是简单，缺点是提高了模块之间的耦合度，让代码变得难以理解。
			const SheetSubView* pSheetSubView = (const SheetSubView*)ClWindowFinder::GetCurrentView_1();
			if (pSheetSubView->GetSheet()->GetIsCanvas()) {
				TaskCommonFunctionPanel::GetInstance()->ShowCanvasFunctionToolBar();
			}
			else {
				TaskCommonFunctionPanel::GetInstance()->ShowSheetFunctionToolBar();
			}
        }

        // 显示常用功能面板。
        PartCommonFunctionPanel::GetInstance()->ShowBar(FALSE, TRUE);
		TaskCommonFunctionPanel::GetInstance()->ShowBar(TRUE, TRUE);

        // 隐藏所有右侧的面板。
        UIManager::GetInstance()->GetBarManager()->HideAllRightBar();

        // 激活板材管理器面板，因为它和其它面板dock在一起。
        TaskSheetManagementBar::GetInstance()->ShowPane(TRUE, FALSE, TRUE);

        // 显示状态栏。
        UIManager::GetInstance()->GetStatusBar()->Show();

		// 让主框架重新布局。
		((CMDIFrameWnd*)AfxGetApp()->GetMainWnd())->RecalcLayout();
    }
}

void TaskFrame::OnClose()
{
    // check whether the current document has been modified.
    CDocument* pTmpDoc = GetActiveDocument();
    TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pTmpDoc);
    pTaskDoc->SetModifyStatus();

    CChildFrame::OnClose();
}

void TaskFrame::OnDestroy()
{
    // remove the task from DataItemTracker.
    CDocument* pTmpDoc = GetActiveDocument();
    TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pTmpDoc);
    TaskItemPtr pTaskItem = pTaskDoc->GetTaskItem();
    DataItemTracker::GetInstance()->RemoveDataItem(pTaskItem->GetID());

    // tell CMainFrame that i has been destroyed.
    HWND hwnd = GetParent()->GetParent()->GetSafeHwnd();
    ::SendMessage(hwnd, WM_CHILDFRM_CLOSE, 0, 0);

    CChildFrame::OnDestroy();
}

END_CUTLEADER_NAMESPACE();
