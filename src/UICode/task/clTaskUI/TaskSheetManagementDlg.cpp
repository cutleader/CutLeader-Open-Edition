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

    // ���ֿؼ�
    AddAnchor(IDC_STATIC_SheetsArea, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_BTN_addSheet2Task, TOP_RIGHT);
    AddAnchor(IDC_BTN_deleteSheetFromTask, TOP_RIGHT);

    // �����ӶԻ���
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

    // �����ӶԻ���
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
        // �½��������
        SheetPtr pSheet = sheetInfoDlg.GetSheet();
        SheetProcessor::UpdateSheetCache(pSheet);
        m_pTaskItem->GetSheetList()->push_back(pSheet);

        // ����ǰ������ͼ����Ϣ�������һ�Ű�ġ�
        ClWindowFinder::GetCurrentView_2()->SendMessage(WM_openLastSheetInTask, 0, 0);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);

        // �����������
        m_taskSheetInfoListDlg.Redisplay_TaskSheetInfoListDlg(m_pTaskItem);
    }
}

void TaskSheetManagementDlg::OnDeleteSheetFromTask()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// �õ�����ѡ�еİ�ġ�
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

	// ִ��ɾ��
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_MakeSureToDeleteTaskSheet);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		// ���ɾ�����
		for (unsigned int i = 0; i < pSelectedSheets->size(); i++)
		{
			// Ҫɾ���İ���ڰ�ļ����е�������Ҳ�Ƕ�Ӧ��������ͼ��ǩҳ��������
			int iTabIndex = m_pTaskItem->GetSheetIndex(pSelectedSheets->operator [](i)->GetID());

			// ɾ�������еİ�����ݡ�
			m_pTaskItem->DeleteSheetFromTask(pSelectedSheets->operator [](i)->GetID());
			m_pTaskItem->SetModified(TRUE);

			// ����ǰ������ͼ����Ϣ��ɾ��ָ���ı�ǩҳ��
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_deleteSheetSubView, iTabIndex, 0);
		}

		// ������ʾ�����������ӶԻ���
		m_taskSheetInfoListDlg.Redisplay_TaskSheetInfoListDlg(m_pTaskItem);

		// ���ѡ��Ķ���
		std::vector<TaskSheetInfoDlgPtr> taskSheetSubDlgs = m_taskSheetInfoListDlg.GetSheetSubDlgs();
		for (unsigned int i = 1; i < taskSheetSubDlgs.size(); i++)
		{
			TaskSheetInfoDlgPtr pTaskSheetSubDlg = taskSheetSubDlgs.operator [](i);
			pTaskSheetSubDlg->UnSelectThisSheet();
		}

		// ����Ϣ��������ͼ��������������������������
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);

		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_UpdateLoopToolPanel, 0, 0);
	}
}

void TaskSheetManagementDlg::Redisplay_TaskSheetManagementDlg(TaskItemPtr pTaskItem)
{
    m_pTaskItem = pTaskItem;
    m_taskSheetInfoListDlg.Redisplay_TaskSheetInfoListDlg(pTaskItem);
    //this->Invalidate(TRUE);//�о�����Ҫ�������
}

void TaskSheetManagementDlg::RedrawSheetThumbnail(LONGLONG iSheetID)
{
    if (!m_pTaskItem) // ���������������
    {
        LogMgr::DebugWarn(_T("8759"));
    }

    // ��������ʾ�İ���ӶԻ������ҵ���Ӧ�ģ���������������ʾ��
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
	if (!m_pTaskItem) // ���������������
	{
		LogMgr::DebugWarn(_T("210397"));
	}

	return m_pTaskItem;
}

END_CUTLEADER_NAMESPACE()
