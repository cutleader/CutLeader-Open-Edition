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


const int g_iHorizontalDistanceBetweenSubDialog = 1; // 子窗口们水平方向的间距


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
		int iVisibleSubDialogCount = this->GetVisibleSubWindowCount(); // 可见的子对话框数量
        if (m_pTaskItem->GetSheetList()->size() == iVisibleSubDialogCount) // 防止在添加板材后马上进入定时器
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

    if (!m_pTaskItem) // 此时刚刚启动cutleader，这些界面创建了，但是还没打开任务，数据没有送进来。
    {
        return;
    }

	if (m_taskSheetInfoDlgList.size() < m_pTaskItem->GetSheetList()->size())
	{
		return;
	}

	// 子对话框的长宽。
    CRect rectClient;
    GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = windowRect.Height();
	int iSubDialogHeight = rectClient.Height();

    // 布局子对话框。
	int iScrollBarPosition = m_scrollHelper->GetScrollPos().cx; // 注意，这里需要算上滚动条的位置
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
	// 子对话框的长宽。
	CRect rectClient;
	GetClientRect(rectClient);
	CRect windowRect;
	this->GetWindowRect(&windowRect);
	int iSubDialogWidth = windowRect.Height();
	int iSubDialogHeight = rectClient.Height();

	// 要是想把所有子窗口都显示出来，需要的对话框宽度是这个。
	int iDlgWidth_ifCanDisplayAllSubDlgs = (iSubDialogWidth + g_iHorizontalDistanceBetweenSubDialog) * pTaskItem->GetSheetList()->size();

	// 是不是新增了板材到任务中。
	bool bAddedNewSheets = false;
	if (m_pTaskItem && m_pTaskItem->GetID() == pTaskItem->GetID() && pTaskItem->GetSheetList()->size() > this->GetVisibleSubWindowCount()) {
		bAddedNewSheets = true;
	}

	// 要是切换了任务，那就把滚动条滚到最左面
	if (!m_pTaskItem || m_pTaskItem->GetID() != pTaskItem->GetID()) {
		m_scrollHelper->ScrollToOrigin(true, false);
	}

	// 显示这个任务的零件不需要滚动条，如果之前要是有滚动条，那就用这种方式让之前的滚动条滚到最左方。
	if (iDlgWidth_ifCanDisplayAllSubDlgs < rectClient.Width()) {
		m_scrollHelper->ScrollToOrigin(true, false);
	}

    m_pTaskItem = pTaskItem;

    // 先隐藏多余的子对话框。有时子对话框的数量比板材种类要多，比如刚刚删除了几种板材。
    if (m_taskSheetInfoDlgList.size() > m_pTaskItem->GetSheetList()->size()) {
        int iCount = m_taskSheetInfoDlgList.size() - m_pTaskItem->GetSheetList()->size();
        for (int i = 0; i < iCount; i++) {
            TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[m_pTaskItem->GetSheetList()->size() + i];
            pTaskSheetInfoDlg->ShowWindow(SW_HIDE);
			pTaskSheetInfoDlg->ResetData(); // 不要忘了调用这个函数。
        }
    }

    // 先用已经创建好的子对话框进行显示。
    for (unsigned int i = 0; i < m_taskSheetInfoDlgList.size(); i++) {
        if (i < m_pTaskItem->GetSheetList()->size()) { // 此时子对话框的数量有可能比零件要多的，比如刚刚删除了几个板材。
            TaskSheetInfoDlgPtr pTaskSheetInfoDlg = m_taskSheetInfoDlgList[i];
            pTaskSheetInfoDlg->Init(m_pTaskItem->GetSheetList()->operator [](i), m_pTaskItem);
            pTaskSheetInfoDlg->DisplayDataOnDialog();
			if (iDlgWidth_ifCanDisplayAllSubDlgs < rectClient.Width()) { // 此时不需要滚动条
				CRect subWndRect;
				pTaskSheetInfoDlg->GetWindowRect(&subWndRect);
				this->ScreenToClient(&subWndRect);
				pTaskSheetInfoDlg->SetWindowPos(NULL, subWndRect.left, subWndRect.top, iSubDialogWidth, iSubDialogHeight, SWP_NOACTIVATE | SWP_NOZORDER);
			}
            pTaskSheetInfoDlg->ShowWindow(SW_SHOW);
        }
    }

    // 要是已经创建好的子对话框数量不够，那就再创建子对话框。
    if (m_pTaskItem->GetSheetList()->size() > m_taskSheetInfoDlgList.size()) {
        int iCurrentSubDlgCount = m_taskSheetInfoDlgList.size();
        int iCurrentSubDlgPosition = iCurrentSubDlgCount * (iSubDialogWidth + g_iHorizontalDistanceBetweenSubDialog);
		iCurrentSubDlgPosition -= m_scrollHelper->GetScrollPos().cx; // 注意，这里需要算上滚动条的位置
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

	// 告诉滚动条助手，我想显示的东西的尺寸。
	m_scrollHelper->SetDisplaySize(iDlgWidth_ifCanDisplayAllSubDlgs, 10);
	if (bAddedNewSheets) { // 如果新增了板材到板材管理器，需要把滚动条拉到最右侧。
		m_scrollHelper->ScrollToRight();
	}
}

END_CUTLEADER_NAMESPACE()
