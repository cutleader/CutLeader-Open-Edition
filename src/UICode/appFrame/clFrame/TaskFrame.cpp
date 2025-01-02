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
		// ��ʾCNC������塣
		ChangeLoopToolPanel::GetInstance()->ShowBar(TRUE, TRUE);

        // ��ʾ����Ͱ�Ĺ�����塣
        TaskPartManagementBar::GetInstance()->ShowBar(TRUE, TRUE);
		TaskSheetManagementBar::GetInstance()->ShowBar(TRUE, TRUE);

        // ����Ǵ������ĵ��л�����������ĵ��ģ���ô��Ҫ����һЩ���¡�
        CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();
        CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pMainFrame->GetActiveFrame();
        CDocument* pDocument = pChildFrame->GetActiveDocument();
        if (pDocument) // �Ǵ������ĵ��л�����������ĵ��ġ�
        {
            TaskDocument* pTaskDoc = dynamic_cast<TaskDocument*>(pDocument);
            TaskPartManagementBar::GetInstance()->GetTaskPartManagementDlg()->Redisplay_TaskPartManagementDlg(pTaskDoc->GetTaskItem());
			TaskSheetManagementBar::GetInstance()->GetTaskSheetManagementDlg()->Redisplay_TaskSheetManagementDlg(pTaskDoc->GetTaskItem());

			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);

			// ���������ĵ��л��������ĵ�ʱ����Ҫ���ݵ�ǰ����򿪵��ǵ�ͼ���ǰ�ģ��ٶԽ�����и��¡��˴�֮ǰ��ͨ����������ͼ����
			// WM_UpdateTaskPartManagementPanel��Ϣ��ʵ�ֵģ�����Դ������ع���ĿǰCCP�е���Ϣ����һ�������²�ģ������ϲ�ģ��ͨ�ţ�
			// �������ֻ��ͨ������Ϣ��ʵ�֣�����Ϣ����ͨ�ŵ��ŵ��Ǽ򵥣�ȱ���������ģ��֮�����϶ȣ��ô�����������⡣
			const SheetSubView* pSheetSubView = (const SheetSubView*)ClWindowFinder::GetCurrentView_1();
			if (pSheetSubView->GetSheet()->GetIsCanvas()) {
				TaskCommonFunctionPanel::GetInstance()->ShowCanvasFunctionToolBar();
			}
			else {
				TaskCommonFunctionPanel::GetInstance()->ShowSheetFunctionToolBar();
			}
        }

        // ��ʾ���ù�����塣
        PartCommonFunctionPanel::GetInstance()->ShowBar(FALSE, TRUE);
		TaskCommonFunctionPanel::GetInstance()->ShowBar(TRUE, TRUE);

        // ���������Ҳ����塣
        UIManager::GetInstance()->GetBarManager()->HideAllRightBar();

        // �����Ĺ�������壬��Ϊ�����������dock��һ��
        TaskSheetManagementBar::GetInstance()->ShowPane(TRUE, FALSE, TRUE);

        // ��ʾ״̬����
        UIManager::GetInstance()->GetStatusBar()->Show();

		// ����������²��֡�
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
