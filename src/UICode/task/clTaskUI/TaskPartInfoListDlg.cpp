#include "stdafx.h"
#include "TaskPartInfoListDlg.h"

#include "baseConst.h"
#include "DataCenterItemLoader.h"
#include "DataBaseManager.h"
#include "TaskPartInfoDlg.h"
#include "TaskPartList.h"
#include "TaskItem.h"
#include "ClTaskResource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int g_iVerticalDistanceBetweenSubDialog = 1; // �Ӵ����Ǵ�ֱ����ļ��
const double g_dRatio_1 = 0.37; // ������ĸ߶Ⱥ͸����ڿ�ȵı�ֵ

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskPartInfoListDlg, CLResizableDialog)

TaskPartInfoListDlg::TaskPartInfoListDlg() : CLResizableDialog(TaskPartInfoListDlg::IDD)
{
    m_msgFont.CreatePointFont(120, _T("Arial"));

	// create the scroll helper and attach it to this dialog.
	m_scrollHelper = new CScrollHelper;
	m_scrollHelper->AttachWnd(this);
}

TaskPartInfoListDlg::~TaskPartInfoListDlg()
{
    m_msgFont.DeleteObject();
	delete m_scrollHelper;
}

void TaskPartInfoListDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(TaskPartInfoListDlg, CLResizableDialog)
    ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL TaskPartInfoListDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // ���ֿؼ�
    AddAnchor(IDC_STATIC_NoTaskPartToDisplay, TOP_LEFT, BOTTOM_RIGHT);

    GetDlgItem(IDC_STATIC_NoTaskPartToDisplay)->SetWindowText(MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_NoPartToShow));
    GetDlgItem(IDC_STATIC_NoTaskPartToDisplay)->SetFont(&m_msgFont);

    m_nTimer = SetTimer(9, 100, 0);

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void TaskPartInfoListDlg::OnDestroy()
{
    KillTimer(m_nTimer);
	CLResizableDialog::OnDestroy();
}

BOOL TaskPartInfoListDlg::OnMouseWheel(UINT nFlags, short iDelta, CPoint pt)
{
	BOOL wasScrolled = m_scrollHelper->OnMouseWheel(nFlags, iDelta, pt);
	return wasScrolled;
}

void TaskPartInfoListDlg::OnTimer(UINT nIDEvent) 
{
    if (m_pTaskItem)
    {
		int iVisibleSubDialogCount = this->GetVisibleSubWindowCount(); // �ɼ����ӶԻ�������
        if (m_pTaskItem->GetTaskPartList()->size() == iVisibleSubDialogCount) // ��ֹ�������������Ͻ��붨ʱ��
        {
            POINT point;
            GetCursorPos(&point);
            for (unsigned int i = 0; i < m_pTaskItem->GetTaskPartList()->size(); i++)
            {
                TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[i];
                CRect ctrlRect;
                pTaskPartInfoDlg->GetWindowRect(&ctrlRect);
                if (!ctrlRect.PtInRect(point))
                {
                    ::PostMessage(pTaskPartInfoDlg->m_hWnd, WM_MOUSELEAVE, 0, 0);
                }
            }
        }
    }

    CLResizableDialog::OnTimer(nIDEvent);
}

HBRUSH TaskPartInfoListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void TaskPartInfoListDlg::OnSize(UINT nType, int cx, int cy)
{
    CLResizableDialog::OnSize(nType, cx, cy);

    if (!m_pTaskItem) // ��ʱ�ո�����cutleader����Щ���洴���ˣ����ǻ�û����������û���ͽ�����
    {
        return;
    }

	if (m_taskPartInfoDlgList.size() < m_pTaskItem->GetTaskPartList()->size())
	{
		return;
	}

	// �ӶԻ���ĳ���
	CRect rectClient;
	GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = rectClient.Width();
	int iSubDialogHeight = windowRect.Width() * g_dRatio_1;

	// �����ӶԻ���
	int iScrollBarPosition = m_scrollHelper->GetScrollPos().cy; // ע�⣬������Ҫ���Ϲ�������λ��
	for (unsigned int i = 0; i < m_pTaskItem->GetTaskPartList()->size(); i++)
	{
		TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[i];
		pTaskPartInfoDlg->SetWindowPos(NULL, rectClient.left, rectClient.top + (iSubDialogHeight + g_iVerticalDistanceBetweenSubDialog) * i - iScrollBarPosition,
			iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		pTaskPartInfoDlg->ShowWindow(SW_SHOW);
	}

	m_scrollHelper->OnSize(nType, cx, cy);
}

void TaskPartInfoListDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	m_scrollHelper->OnVScroll(nSBCode, nPos, pScrollBar);
}

void TaskPartInfoListDlg::OnPaint()
{
    CLResizableDialog::OnPaint();

    if (!m_pTaskItem) // ��ʱ�ո�����cutleader����Щ���洴���ˣ����ǻ�û����������û���ͽ�����
    {
        return;
    }

	if (m_pTaskItem->GetTaskPartList()->size() == 0)
	{
		GetDlgItem(IDC_STATIC_NoTaskPartToDisplay)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NoTaskPartToDisplay)->ShowWindow(SW_HIDE);
	}
}

void TaskPartInfoListDlg::ReDisplayNestedCount_ofEachPart()
{
	for (unsigned int i = 0; i < m_taskPartInfoDlgList.size(); i++) {
		TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[i];
		if (pTaskPartInfoDlg->IsWindowVisible()) {
			pTaskPartInfoDlg->ReDisplayNestedCount();
		}
	}
}

unsigned int TaskPartInfoListDlg::GetVisibleSubWindowCount() const
{
	unsigned int iVisibleSubWindowCount = 0;

	for (unsigned int i = 0; i < m_taskPartInfoDlgList.size(); i++)
	{
		TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[i];
		if (pTaskPartInfoDlg->IsWindowVisible())
		{
			iVisibleSubWindowCount++;
		}
	}

	return iVisibleSubWindowCount;
}

void TaskPartInfoListDlg::Redisplay_TaskPartInfoListDlg(TaskItemPtr pTaskItem)
{
	// �ӶԻ���ĳ���
	CRect rectClient;
	GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = rectClient.Width();
	int iSubDialogHeight = windowRect.Width() * g_dRatio_1;

	// ��ʾȫ�����ӶԻ�����Ҫ��ô��ĸ߶ȡ�
	int iDlgHeight_ifCanDisplayAllSubDlgs = (iSubDialogHeight + g_iVerticalDistanceBetweenSubDialog) * pTaskItem->GetTaskPartList()->size();

	// �ǲ�������������������С�
	bool bAddedNewParts = false;
	if (m_pTaskItem && m_pTaskItem->GetID() == pTaskItem->GetID() && pTaskItem->GetTaskPartList()->size() > this->GetVisibleSubWindowCount()) {
		bAddedNewParts = true;
	}

	// Ҫ���л��������ǾͰѹ���������������
	if (!m_pTaskItem || m_pTaskItem->GetID() != pTaskItem->GetID()) {
		m_scrollHelper->ScrollToOrigin(false, true);
	}

	// ��ʾ���������������Ҫ�����������֮ǰҪ���й��������Ǿ������ַ�ʽ��֮ǰ�Ĺ������������Ϸ���
	if (iDlgHeight_ifCanDisplayAllSubDlgs < rectClient.Height()) {
		m_scrollHelper->ScrollToOrigin(false, true);
	}

    m_pTaskItem = pTaskItem;

    // �����ض�����ӶԻ�����ʱ�ӶԻ�������������Ҫ�࣬����ո�ɾ���˼��������
    if (m_taskPartInfoDlgList.size() > m_pTaskItem->GetTaskPartList()->size()) {
        int iCount = m_taskPartInfoDlgList.size() - m_pTaskItem->GetTaskPartList()->size();
        for (unsigned int i = 0; i < iCount; i++) {
            TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[m_pTaskItem->GetTaskPartList()->size() + i];
            pTaskPartInfoDlg->ShowWindow(SW_HIDE);
			pTaskPartInfoDlg->ResetData(); // ��Ҫ���˵������������
        }
    }

    // �����Ѿ������õ��ӶԻ��������ʾ��
    for (unsigned int i = 0; i < m_taskPartInfoDlgList.size(); i++) {
        if (i < m_pTaskItem->GetTaskPartList()->size()) {
            TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[i];
            pTaskPartInfoDlg->Init(m_pTaskItem->GetTaskPartList()->operator [](i), m_pTaskItem);
            pTaskPartInfoDlg->DisplayDataOnDialog();
			if (iDlgHeight_ifCanDisplayAllSubDlgs < rectClient.Height()) { // ��ʱ����Ҫ������
				CRect subWndRect;
				pTaskPartInfoDlg->GetWindowRect(&subWndRect);
				this->ScreenToClient(&subWndRect);
				pTaskPartInfoDlg->SetWindowPos(NULL, subWndRect.left, subWndRect.top, iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
			}
            pTaskPartInfoDlg->ShowWindow(SW_SHOW);
        }
        else {
            ; // ��ʱ�ӶԻ�����������������Ҫ�࣬����ո�ɾ���˼��������
        }
    }

    // Ҫ���Ѿ������õ��ӶԻ��������������Ǿ��ٴ����ӶԻ���
    if (m_pTaskItem->GetTaskPartList()->size() > m_taskPartInfoDlgList.size()) {
        int iCurrentSubDlgCount = m_taskPartInfoDlgList.size();
        int iCurrentSubDlgPosition = iCurrentSubDlgCount * (iSubDialogHeight + g_iVerticalDistanceBetweenSubDialog);
		iCurrentSubDlgPosition -= m_scrollHelper->GetScrollPos().cy; // ע�⣬������Ҫ���Ϲ�������λ��
        int iCount = m_pTaskItem->GetTaskPartList()->size() - m_taskPartInfoDlgList.size();
        for (unsigned int i = 0; i < iCount; i++) {
            TaskPartInfoDlgPtr pTaskPartInfoDlg(new TaskPartInfoDlg);
            pTaskPartInfoDlg->Init(m_pTaskItem->GetTaskPartList()->operator [](iCurrentSubDlgCount + i), m_pTaskItem);
            pTaskPartInfoDlg->Create(IDD_clTaskUI_TaskPartInfo, this);
            pTaskPartInfoDlg->SetWindowPos(NULL, rectClient.left, iCurrentSubDlgPosition + rectClient.top + (iSubDialogHeight + g_iVerticalDistanceBetweenSubDialog) * i,
                iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
            pTaskPartInfoDlg->ShowWindow(SW_SHOW);
            m_taskPartInfoDlgList.push_back(pTaskPartInfoDlg);
        }
    }

	// ���߹��������֣�������ʾ�Ķ����ĳߴ硣
	m_scrollHelper->SetDisplaySize(10, iDlgHeight_ifCanDisplayAllSubDlgs);
	if (bAddedNewParts) { // ���������������������������Ҫ�ѹ��������������档
		m_scrollHelper->ScrollToBottom();
	}
}

END_CUTLEADER_NAMESPACE()
