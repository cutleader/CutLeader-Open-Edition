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


const int g_iVerticalDistanceBetweenSubDialog = 1; // 子窗口们垂直方向的间距
const double g_dRatio_1 = 0.37; // 豆腐块的高度和父窗口宽度的比值

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

    // 布局控件
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
		int iVisibleSubDialogCount = this->GetVisibleSubWindowCount(); // 可见的子对话框数量
        if (m_pTaskItem->GetTaskPartList()->size() == iVisibleSubDialogCount) // 防止在添加零件后马上进入定时器
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

    if (!m_pTaskItem) // 此时刚刚启动cutleader，这些界面创建了，但是还没打开任务，数据没有送进来。
    {
        return;
    }

	if (m_taskPartInfoDlgList.size() < m_pTaskItem->GetTaskPartList()->size())
	{
		return;
	}

	// 子对话框的长宽。
	CRect rectClient;
	GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = rectClient.Width();
	int iSubDialogHeight = windowRect.Width() * g_dRatio_1;

	// 布局子对话框。
	int iScrollBarPosition = m_scrollHelper->GetScrollPos().cy; // 注意，这里需要算上滚动条的位置
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

    if (!m_pTaskItem) // 此时刚刚启动cutleader，这些界面创建了，但是还没打开任务，数据没有送进来。
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
	// 子对话框的长宽。
	CRect rectClient;
	GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = rectClient.Width();
	int iSubDialogHeight = windowRect.Width() * g_dRatio_1;

	// 显示全部的子对话框，需要这么大的高度。
	int iDlgHeight_ifCanDisplayAllSubDlgs = (iSubDialogHeight + g_iVerticalDistanceBetweenSubDialog) * pTaskItem->GetTaskPartList()->size();

	// 是不是新增了零件到任务中。
	bool bAddedNewParts = false;
	if (m_pTaskItem && m_pTaskItem->GetID() == pTaskItem->GetID() && pTaskItem->GetTaskPartList()->size() > this->GetVisibleSubWindowCount()) {
		bAddedNewParts = true;
	}

	// 要是切换了任务，那就把滚动条滚到最上面
	if (!m_pTaskItem || m_pTaskItem->GetID() != pTaskItem->GetID()) {
		m_scrollHelper->ScrollToOrigin(false, true);
	}

	// 显示这个任务的零件不需要滚动条，如果之前要是有滚动条，那就用这种方式让之前的滚动条滚到最上方。
	if (iDlgHeight_ifCanDisplayAllSubDlgs < rectClient.Height()) {
		m_scrollHelper->ScrollToOrigin(false, true);
	}

    m_pTaskItem = pTaskItem;

    // 先隐藏多余的子对话框。有时子对话框的数量比零件要多，比如刚刚删除了几个零件。
    if (m_taskPartInfoDlgList.size() > m_pTaskItem->GetTaskPartList()->size()) {
        int iCount = m_taskPartInfoDlgList.size() - m_pTaskItem->GetTaskPartList()->size();
        for (unsigned int i = 0; i < iCount; i++) {
            TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[m_pTaskItem->GetTaskPartList()->size() + i];
            pTaskPartInfoDlg->ShowWindow(SW_HIDE);
			pTaskPartInfoDlg->ResetData(); // 不要忘了调用这个函数。
        }
    }

    // 先用已经创建好的子对话框进行显示。
    for (unsigned int i = 0; i < m_taskPartInfoDlgList.size(); i++) {
        if (i < m_pTaskItem->GetTaskPartList()->size()) {
            TaskPartInfoDlgPtr pTaskPartInfoDlg = m_taskPartInfoDlgList[i];
            pTaskPartInfoDlg->Init(m_pTaskItem->GetTaskPartList()->operator [](i), m_pTaskItem);
            pTaskPartInfoDlg->DisplayDataOnDialog();
			if (iDlgHeight_ifCanDisplayAllSubDlgs < rectClient.Height()) { // 此时不需要滚动条
				CRect subWndRect;
				pTaskPartInfoDlg->GetWindowRect(&subWndRect);
				this->ScreenToClient(&subWndRect);
				pTaskPartInfoDlg->SetWindowPos(NULL, subWndRect.left, subWndRect.top, iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
			}
            pTaskPartInfoDlg->ShowWindow(SW_SHOW);
        }
        else {
            ; // 此时子对话框的数量比零件种类要多，比如刚刚删除了几个零件。
        }
    }

    // 要是已经创建好的子对话框数量不够，那就再创建子对话框。
    if (m_pTaskItem->GetTaskPartList()->size() > m_taskPartInfoDlgList.size()) {
        int iCurrentSubDlgCount = m_taskPartInfoDlgList.size();
        int iCurrentSubDlgPosition = iCurrentSubDlgCount * (iSubDialogHeight + g_iVerticalDistanceBetweenSubDialog);
		iCurrentSubDlgPosition -= m_scrollHelper->GetScrollPos().cy; // 注意，这里需要算上滚动条的位置
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

	// 告诉滚动条助手，我想显示的东西的尺寸。
	m_scrollHelper->SetDisplaySize(10, iDlgHeight_ifCanDisplayAllSubDlgs);
	if (bAddedNewParts) { // 如果新增了零件到零件管理器，需要把滚动条拉到最下面。
		m_scrollHelper->ScrollToBottom();
	}
}

END_CUTLEADER_NAMESPACE()
