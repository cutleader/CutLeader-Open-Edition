#include "stdafx.h"
#include "TaskSheetManagementDlg.h"

#include "baseConst.h"
#include "LogMgr.h"
#include "ClBaseDataResource.h"
#include "ClWindowFinder.h"

#include "TaskSheetInfoDlg.h"
#include "Sheet.h"
#include "SheetManager.h"
#include "SheetInfoDlg.h"
#include "SheetProcessor.h"
#include "TaskItem.h"
#include "SheetList.h"
#include "ClTaskResource.h"
#include "ClSheetResource.h"
#include "ClPartLayoutResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskSheetManagementDlg, CLResizableDialog)

TaskSheetManagementDlg::TaskSheetManagementDlg() : CLResizableDialog(TaskSheetManagementDlg::IDD)
{
}

TaskSheetManagementDlg::~TaskSheetManagementDlg()
{
}

void TaskSheetManagementDlg::DoDataExchange(CDataExchange* pDX)
{
    CLResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_addSheet2Task, m_btnAddSheet);
	DDX_Control(pDX, IDC_BTN_deleteSheetFromTask, m_btnDeleteSheet);
}

BEGIN_MESSAGE_MAP(TaskSheetManagementDlg, CLResizableDialog)
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
    ON_WM_SIZE()

    ON_BN_CLICKED(IDC_BTN_addSheet2Task, OnNewSheet2Task)
    ON_BN_CLICKED(IDC_BTN_deleteSheetFromTask, OnDeleteSheetFromTask)

    ON_MESSAGE(WM_selectOrUnSelectSheetInSheetDlg, OnSelectOrUnSelectSheetInTaskSheetDlg)
END_MESSAGE_MAP()

BOOL TaskSheetManagementDlg::OnInitDialog()
{
    __super::OnInitDialog();

    // 布局控件
    AddAnchor(IDC_STATIC_SheetsArea, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_BTN_addSheet2Task, TOP_RIGHT);
    AddAnchor(IDC_BTN_deleteSheetFromTask, TOP_RIGHT);

    // 创建子对话框。
    {
        m_taskSheetInfoListDlg.Create(IDD_clTaskUI_SheetInfoList, this);
        m_taskSheetInfoListDlg.ShowWindow(SW_HIDE);
    }

    SetBackgroundColor(SkinManager::GetBackgroundColor());
    return TRUE;
}

void TaskSheetManagementDlg::OnDestroy()
{
    CLResizableDialog::OnDestroy();
}

HBRUSH TaskSheetManagementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskSheetManagementDlg::OnSize(UINT nType, int cx, int cy)
{
    CLResizableDialog::OnSize(nType, cx, cy);

    // 布局子对话框。
    CWnd* pWndPos = GetDlgItem(IDC_STATIC_SheetsArea);
    if (pWndPos)
    {
        CRect rectClient;
        pWndPos->GetWindowRect(&rectClient);
        ScreenToClient(&rectClient);
        m_taskSheetInfoListDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        m_taskSheetInfoListDlg.ShowWindow(SW_SHOW);
    }
}

LRESULT TaskSheetManagementDlg::OnSelectOrUnSelectSheetInTaskSheetDlg(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void TaskSheetManagementDlg::OnNewSheet2Task()
{
    SheetInfoDlg sheetInfoDlg;
    CString strSheetName;
    strSheetName.Format(_T("%s-%d"), MultiLanguageMgr::GetInstance()->TranslateString(IDS_SHEET), m_pTaskItem->GetSheetList()->size());
    sheetInfoDlg.SetSheetName(strSheetName);
    if (sheetInfoDlg.DoModal() == IDOK)
    {
        // 新建板材数据
        SheetPtr pSheet = sheetInfoDlg.GetSheet();
        SheetProcessor::UpdateSheetCache(pSheet);
        m_pTaskItem->GetSheetList()->push_back(pSheet);

        // 给当前任务视图发消息，打开最后一张板材。
        ClWindowFinder::GetCurrentView_2()->SendMessage(WM_openLastSheetInTask, 0, 0);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);

        // 更新任务面板
        m_taskSheetInfoListDlg.Redisplay_TaskSheetInfoListDlg(m_pTaskItem);
    }
}

void TaskSheetManagementDlg::OnDeleteSheetFromTask()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 得到所有选中的板材。
	SheetListPtr pSelectedSheets(new SheetList);
	std::vector<TaskSheetInfoDlgPtr> taskSheetSubDlgs = m_taskSheetInfoListDlg.GetSheetSubDlgs();
	for (unsigned int i = 1; i < taskSheetSubDlgs.size(); i++)
	{
		TaskSheetInfoDlgPtr pTaskSheetSubDlg = taskSheetSubDlgs.operator [](i);
		if (pTaskSheetSubDlg->IsWindowVisible() && pTaskSheetSubDlg->IsSelected())
		{
			pSelectedSheets->push_back(pTaskSheetSubDlg->GetSheet());
		}
	}

	if (pSelectedSheets->size() == 0)
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NoSheetIsSelectedToDelete);
		MessageBox(str, strProductName, MB_ICONWARNING);
		return;
	}

	if (pSelectedSheets->size() == m_pTaskItem->GetSheetList()->size() - 1)
	{
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CannotDeleteAllTaskSheet);
		MessageBox(str, strProductName, MB_ICONWARNING);
		return;
	}

	// 执行删除
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MakeSureToDeleteTaskSheet);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		// 逐个删除板材
		for (unsigned int i = 0; i < pSelectedSheets->size(); i++)
		{
			// 要删除的板材在板材集合中的索引，也是对应的任务视图标签页的索引。
			int iTabIndex = m_pTaskItem->GetSheetIndex(pSelectedSheets->operator [](i)->GetID());

			// 删除任务中的板材数据。
			m_pTaskItem->DeleteSheetFromTask(pSelectedSheets->operator [](i)->GetID());
			m_pTaskItem->SetModified(TRUE);

			// 给当前任务视图发消息，删除指定的标签页。
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_deleteSheetSubView, iTabIndex, 0);
		}

		// 重新显示所有任务板材子对话框。
		m_taskSheetInfoListDlg.Redisplay_TaskSheetInfoListDlg(m_pTaskItem);

		// 清空选择的对象
		std::vector<TaskSheetInfoDlgPtr> taskSheetSubDlgs = m_taskSheetInfoListDlg.GetSheetSubDlgs();
		for (unsigned int i = 1; i < taskSheetSubDlgs.size(); i++)
		{
			TaskSheetInfoDlgPtr pTaskSheetSubDlg = taskSheetSubDlgs.operator [](i);
			pTaskSheetSubDlg->UnSelectThisSheet();
		}

		// 发消息给任务视图，让零件管理器更新零件个数。
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
	}
}

void TaskSheetManagementDlg::Redisplay_TaskSheetManagementDlg(TaskItemPtr pTaskItem)
{
    m_pTaskItem = pTaskItem;
    m_taskSheetInfoListDlg.Redisplay_TaskSheetInfoListDlg(pTaskItem);
    //this->Invalidate(TRUE);//感觉不需要这个调用
}

void TaskSheetManagementDlg::RedrawSheetThumbnail(LONGLONG iSheetID)
{
    if (!m_pTaskItem) // 这个函数调用早了
    {
        LogMgr::DebugWarn(_T("8759"));
    }

    // 从所有显示的板材子对话框中找到对应的，并且让它重新显示。
    std::vector<TaskSheetInfoDlgPtr> taskSheetSubDlgs = m_taskSheetInfoListDlg.GetSheetSubDlgs();
    for (unsigned int i = 0; i < taskSheetSubDlgs.size(); i++)
    {
        TaskSheetInfoDlgPtr pTaskSheetSubDlg = taskSheetSubDlgs.operator [](i);
        if (pTaskSheetSubDlg->IsWindowVisible() && pTaskSheetSubDlg->GetSheet()->GetID() == iSheetID)
        {
            pTaskSheetSubDlg->Invalidate();
        }
    }
}

TaskItemPtr TaskSheetManagementDlg::GetTaskItem() const
{
	if (!m_pTaskItem) // 这个函数调用早了
	{
		LogMgr::DebugWarn(_T("210397"));
	}

	return m_pTaskItem;
}

END_CUTLEADER_NAMESPACE()
