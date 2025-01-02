#include "stdafx.h"
#include "TaskSheetInfoListDlg.h"

#include "baseConst.h"
#include "DataCenterItemLoader.h"
#include "DataBaseManager.h"
#include "TaskSheetInfoDlg.h"
#include "SheetList.h"
#include "TaskItem.h"
#include "ClTaskResource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int g_iHorizontalDistanceBetweenSubDialog = 1; // �Ӵ�����ˮƽ����ļ��


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskSheetInfoListDlg, CLResizableDialog)

TaskSheetInfoListDlg::TaskSheetInfoListDlg() : CLResizableDialog(TaskSheetInfoListDlg::IDD)
{
	// create the scroll helper and attach it to this dialog.
	m_scrollHelper = new CScrollHelper;
	m_scrollHelper->AttachWnd(this);
}

TaskSheetInfoListDlg::~TaskSheetInfoListDlg()
{
	delete m_scrollHelper;
}

void TaskSheetInfoListDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(TaskSheetInfoListDlg, CLResizableDialog)
    ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
    ON_WM_SIZE()
    ON_WM_HSCROLL()
    ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL TaskSheetInfoListDlg::OnInitDialog()
{
	__super::OnInitDialog();

    m_nTimer = SetTimer(10, 100, 0);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void TaskSheetInfoListDlg::OnDestroy()
{
    KillTimer(m_nTimer);
	CLResizableDialog::OnDestroy();
}

BOOL TaskSheetInfoListDlg::OnMouseWheel(UINT nFlags, short iDelta, CPoint pt)
{
	BOOL wasScrolled = m_scrollHelper->OnMouseWheel(nFlags, iDelta, pt);
	return wasScrolled;
}

void TaskSheetInfoListDlg::OnTimer(UINT nIDEvent) 
{
    if (m_pTaskItem)
    {
		int iVisibleSubDialogCount = this->GetVisibleSubWindowCount(); // �ɼ����ӶԻ�������
        if (m_pTaskItem->GetSheetList()->size() == iVisibleSubDialogCount) // ��ֹ����Ӱ�ĺ����Ͻ��붨ʱ��
        {
            POINT point;
            GetCursorPos(&point);
            for (unsigned int i = 0; i < m_pTaskItem->GetSheetList()->size(); i++)
            {
                TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[i];
                CRect ctrlRect;
                pTaskSheetInfoDlg->GetWindowRect(&ctrlRect);
                if (!ctrlRect.PtInRect(point))
                {
                    ::PostMessage(pTaskSheetInfoDlg->m_hWnd, WM_MOUSELEAVE, 0, 0);
                }
            }
        }
    }

    CLResizableDialog::OnTimer(nIDEvent);
}

unsigned int TaskSheetInfoListDlg::GetVisibleSubWindowCount() const
{
	unsigned int iVisibleSubWindowCount = 0;

	for (unsigned int i = 0; i < m_taskSheetInfoDlgList.size(); i++)
	{
		TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[i];
		if (pTaskSheetInfoDlg->IsWindowVisible())
		{
			iVisibleSubWindowCount++;
		}
	}

	return iVisibleSubWindowCount;
}

HBRUSH TaskSheetInfoListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskSheetInfoListDlg::OnSize(UINT nType, int cx, int cy)
{
    CLResizableDialog::OnSize(nType, cx, cy);

    if (!m_pTaskItem) // ��ʱ�ո�����cutleader����Щ���洴���ˣ����ǻ�û����������û���ͽ�����
    {
        return;
    }

	if (m_taskSheetInfoDlgList.size() < m_pTaskItem->GetSheetList()->size())
	{
		return;
	}

	// �ӶԻ���ĳ���
    CRect rectClient;
    GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = windowRect.Height();
	int iSubDialogHeight = rectClient.Height();

    // �����ӶԻ���
	int iScrollBarPosition = m_scrollHelper->GetScrollPos().cx; // ע�⣬������Ҫ���Ϲ�������λ��
    for (unsigned int i = 0; i < m_pTaskItem->GetSheetList()->size(); i++)
    {
        TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[i];
        pTaskSheetInfoDlg->SetWindowPos(NULL, rectClient.left + (iSubDialogWidth + g_iHorizontalDistanceBetweenSubDialog) * i - iScrollBarPosition, rectClient.top,
			iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
        pTaskSheetInfoDlg->ShowWindow(SW_SHOW);
    }

	m_scrollHelper->OnSize(nType, cx, cy);
}

void TaskSheetInfoListDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	m_scrollHelper->OnHScroll(nSBCode, nPos, pScrollBar);
}

void TaskSheetInfoListDlg::OnPaint()
{
    CLResizableDialog::OnPaint();
}

void TaskSheetInfoListDlg::Redisplay_TaskSheetInfoListDlg(TaskItemPtr pTaskItem)
{
	// �ӶԻ���ĳ���
	CRect rectClient;
	GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = windowRect.Height();
	int iSubDialogHeight = rectClient.Height();

	// Ҫ����������Ӵ��ڶ���ʾ��������Ҫ�ĶԻ������������
	int iDlgWidth_ifCanDisplayAllSubDlgs = (iSubDialogWidth + g_iHorizontalDistanceBetweenSubDialog) * pTaskItem->GetSheetList()->size();

	// �ǲ��������˰�ĵ������С�
	bool bAddedNewSheets = false;
	if (m_pTaskItem && m_pTaskItem->GetID() == pTaskItem->GetID() && pTaskItem->GetSheetList()->size() > this->GetVisibleSubWindowCount()) {
		bAddedNewSheets = true;
	}

	// Ҫ���л��������ǾͰѹ���������������
	if (!m_pTaskItem || m_pTaskItem->GetID() != pTaskItem->GetID()) {
		m_scrollHelper->ScrollToOrigin(true, false);
	}

	// ��ʾ���������������Ҫ�����������֮ǰҪ���й��������Ǿ������ַ�ʽ��֮ǰ�Ĺ������������󷽡�
	if (iDlgWidth_ifCanDisplayAllSubDlgs < rectClient.Width()) {
		m_scrollHelper->ScrollToOrigin(true, false);
	}

    m_pTaskItem = pTaskItem;

    // �����ض�����ӶԻ�����ʱ�ӶԻ���������Ȱ������Ҫ�࣬����ո�ɾ���˼��ְ�ġ�
    if (m_taskSheetInfoDlgList.size() > m_pTaskItem->GetSheetList()->size()) {
        int iCount = m_taskSheetInfoDlgList.size() - m_pTaskItem->GetSheetList()->size();
        for (int i = 0; i < iCount; i++) {
            TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[m_pTaskItem->GetSheetList()->size() + i];
            pTaskSheetInfoDlg->ShowWindow(SW_HIDE);
			pTaskSheetInfoDlg->ResetData(); // ��Ҫ���˵������������
        }
    }

    // �����Ѿ������õ��ӶԻ��������ʾ��
    for (unsigned int i = 0; i < m_taskSheetInfoDlgList.size(); i++) {
        if (i < m_pTaskItem->GetSheetList()->size()) { // ��ʱ�ӶԻ���������п��ܱ����Ҫ��ģ�����ո�ɾ���˼�����ġ�
            TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[i];
            pTaskSheetInfoDlg->Init(m_pTaskItem->GetSheetList()->operator [](i), m_pTaskItem);
            pTaskSheetInfoDlg->DisplayDataOnDialog();
			if (iDlgWidth_ifCanDisplayAllSubDlgs < rectClient.Width()) { // ��ʱ����Ҫ������
				CRect subWndRect;
				pTaskSheetInfoDlg->GetWindowRect(&subWndRect);
				this->ScreenToClient(&subWndRect);
				pTaskSheetInfoDlg->SetWindowPos(NULL, subWndRect.left, subWndRect.top, iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
			}
            pTaskSheetInfoDlg->ShowWindow(SW_SHOW);
        }
    }

    // Ҫ���Ѿ������õ��ӶԻ��������������Ǿ��ٴ����ӶԻ���
    if (m_pTaskItem->GetSheetList()->size() > m_taskSheetInfoDlgList.size()) {
        int iCurrentSubDlgCount = m_taskSheetInfoDlgList.size();
        int iCurrentSubDlgPosition = iCurrentSubDlgCount * (iSubDialogWidth + g_iHorizontalDistanceBetweenSubDialog);
		iCurrentSubDlgPosition -= m_scrollHelper->GetScrollPos().cx; // ע�⣬������Ҫ���Ϲ�������λ��
        int iCount = m_pTaskItem->GetSheetList()->size() - m_taskSheetInfoDlgList.size();
        for (int i = 0; i < iCount; i++) {
            TaskSheetInfoDlgPtr pTaskSheetInfoDlg(new TaskSheetInfoDlg);
            pTaskSheetInfoDlg->Init(m_pTaskItem->GetSheetList()->operator [](iCurrentSubDlgCount + i), m_pTaskItem);
            pTaskSheetInfoDlg->Create(IDD_clTaskUI_SheetInfo, this);
            pTaskSheetInfoDlg->SetWindowPos(NULL, iCurrentSubDlgPosition + rectClient.left + (iSubDialogWidth + g_iHorizontalDistanceBetweenSubDialog) * i, rectClient.top,
				iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
            pTaskSheetInfoDlg->ShowWindow(SW_SHOW);
            m_taskSheetInfoDlgList.push_back(pTaskSheetInfoDlg);
        }
    }

	// ���߹��������֣�������ʾ�Ķ����ĳߴ硣
	m_scrollHelper->SetDisplaySize(iDlgWidth_ifCanDisplayAllSubDlgs, 10);
	if (bAddedNewSheets) { // ��������˰�ĵ���Ĺ���������Ҫ�ѹ������������Ҳࡣ
		m_scrollHelper->ScrollToRight();
	}
}

END_CUTLEADER_NAMESPACE()
