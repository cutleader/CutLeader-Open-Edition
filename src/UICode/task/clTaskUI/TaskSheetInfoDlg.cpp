#include "stdafx.h"
#include "TaskSheetInfoDlg.h"

#include "baseConst.h"
#include "ColorSelector.h"
#include "ImgData.h"
#include "ClWindowFinder.h"
#include "ClUILibResource.h"
#include "LogMgr.h"

#include "Sheet.h"
#include "ClTaskResource.h"
#include "TaskItem.h"
#include "ClPartLayoutResource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskSheetInfoDlg, CLResizableDialog)

TaskSheetInfoDlg::TaskSheetInfoDlg() : CLResizableDialog(TaskSheetInfoDlg::IDD)
{
	m_bThisDialogIsActive = false;
}

TaskSheetInfoDlg::~TaskSheetInfoDlg()
{
}

void TaskSheetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATIC_SheetPreview, m_previewWnd);
	DDX_Control(pDX, IDC_EDIT_SheetName, m_ctrlSheetNameLabel);
    DDX_Control(pDX, IDC_EDIT_SheetCount, m_countEditBox);
}

BEGIN_MESSAGE_MAP(TaskSheetInfoDlg, CLResizableDialog)
    ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
    ON_WM_SETCURSOR()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_WM_LBUTTONDOWN()

    ON_BN_CLICKED(IDC_CHECK_SelectSheet, OnSelectSheet)
	ON_MESSAGE(WM_OnCharEventFinished, OnCharEventOfEditBoxFinished)
	ON_MESSAGE(WM_OnKillFocusEventFinished, OnKillFocusEventOfEditBoxFinished)
END_MESSAGE_MAP()

BOOL TaskSheetInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // 布局控件
    {
        AddAnchor(IDC_STATIC_SheetIndex, TOP_LEFT);

        AddAnchor(IDC_STATIC_SheetPreview, TOP_LEFT);
        AddAnchor(IDC_CHECK_SelectSheet, MIDDLE_LEFT);

        AddAnchor(IDC_EDIT_SheetName, BOTTOM_LEFT);

		AddAnchor(IDC_STATIC_Count, BOTTOM_LEFT);
        AddAnchor(IDC_EDIT_SheetCount, BOTTOM_LEFT);
    }

	m_ctrlSheetNameLabel.SetFontBold(TRUE);
	m_ctrlSheetNameLabel.SetBkColor(SkinManager::GetBackgroundColor());
	m_ctrlSheetNameLabel.SetTextColor(SkinManager::GetTextColor());

    DisplayDataOnDialog();

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	SetWindowTheme(GetDlgItem(IDC_CHECK_SelectSheet)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

void TaskSheetInfoDlg::DisplayDataOnDialog()
{
	if (m_pSheet->GetIsCanvas()) // 底图板材
	{
		GetDlgItem(IDC_STATIC_SheetIndex)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SheetCount)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SelectSheet)->ShowWindow(SW_HIDE);
		// 注意，这里没有隐藏数量标签，而是把内容设为空，看起来和隐藏了一样。如果隐藏它，预览图会消失，原因是板材名称标签是XLabel类型，
		// 应该是XLabel控件有BUG。这作为一个workaround，因为底图小对话框永远是第一个，所以这么做没关系。
		GetDlgItem(IDC_STATIC_Count)->SetWindowText(_T(""));
	}
	else // 正常板材
	{
		// 显示板材索引
		CString str;
		str.Format(_T("%d"), m_pTaskItem->GetSheetIndex(m_pSheet->GetID()));
		GetDlgItem(IDC_STATIC_SheetIndex)->SetWindowText(str);

		// 显示板材数量。
		m_countEditBox.Init(m_pSheet->GetSheetCount(), 0, FALSE);
	}

    // 显示板材名称
    GetDlgItem(IDC_EDIT_SheetName)->SetWindowText(m_pSheet->GetName());

	m_bThisDialogIsActive = false;

    ImgData::DrawImgData(m_pSheet->GetPreviewData().get(), &m_previewWnd, SkinManager::GetBackgroundColor());
}

void TaskSheetInfoDlg::OnPaint()
{
    CLResizableDialog::OnPaint();
}

void TaskSheetInfoDlg::OnDestroy()
{
	CLResizableDialog::OnDestroy();
}

HBRUSH TaskSheetInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	ImgData::DrawImgData(m_pSheet->GetPreviewData().get(), &m_previewWnd, SkinManager::GetBackgroundColor());
    return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

BOOL TaskSheetInfoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // 给对话框画个高亮框。
    {
        RECT rect;
        GetWindowRect(&rect);
        ScreenToClient(&rect);
        rect.left = rect.left + 1;
        rect.top = rect.top + 1;
        rect.bottom = rect.bottom - 1;
        rect.right = rect.right - 1;

        CDC* pDC = GetDC();
        CBrush redBrush(ColorSelector::GetColor(COLOR_BLUE));
        pDC->FrameRect(&rect, &redBrush);
        ReleaseDC(pDC);
    }

    // 要是编辑框不可编辑，使之可编辑。
    if (!m_pSheet->GetIsCanvas() && !GetDlgItem(IDC_EDIT_SheetCount)->IsWindowEnabled())
    {
        GetDlgItem(IDC_EDIT_SheetCount)->EnableWindow(TRUE);
    }

	m_bThisDialogIsActive = true;

    return __super::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT TaskSheetInfoDlg::OnKillFocusEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	if (m_pSheet->GetIsCanvas())
		return false;

	int iSheetCount = (int)m_countEditBox.Get_Number();
	if (iSheetCount == 0 || iSheetCount == INVALID_VALUE) // 如果当前编辑框中是0或空值，需要恢复编辑框中的值为上一次的输入值。
	{
		m_countEditBox.Init(m_pSheet->GetSheetCount(), 0, FALSE);
		CString str;
		str.Format(_T("%d"), m_pSheet->GetSheetCount());
		GetDlgItem(IDC_EDIT_SheetCount)->SetWindowText(str);
	}

	return true;
}

LRESULT TaskSheetInfoDlg::OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	if (m_pSheet->GetIsCanvas())
		return false;

	m_countEditBox.UpdateNumEdit_fromUI();
	int iNewSheetCount = (int)m_countEditBox.Get_Number();

	// 如果当前编辑框中是空值或0，不做任何处理。我们假设用户还会继续输入，因为此时没有意义，万一用户忘记输入了，在编辑框失去焦点时需要恢复编辑框中的值为上一次的输入值。
	if (iNewSheetCount == 0 || iNewSheetCount == INVALID_VALUE)
	{
		return true;
	}

	// 修改任务对象
	if (iNewSheetCount != m_pSheet->GetSheetCount())
	{
		m_pSheet->SetSheetCount(iNewSheetCount);
		m_pSheet->SetModified(TRUE);

		// 发消息给任务视图，让零件管理器更新零件个数。
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);
	}

	return true;
}

LRESULT TaskSheetInfoDlg::OnMouseLeave(WPARAM, LPARAM)
{
    // 此前这个对话框就是“非激活状态”，没有必要再次设置为“非激活状态”。
    if (!m_bThisDialogIsActive)
    {
        return 0;
    }

	// 如果该对话框的焦点在编辑框上，让编辑框失去输入焦点。
	if (!m_pSheet->GetIsCanvas())
		GetDlgItem(IDC_CHECK_SelectSheet)->SetFocus();

	m_bThisDialogIsActive = false;

	// 给对话框画个高亮框。
	{
		RECT rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.left = rect.left + 1;
		rect.top = rect.top + 1;
		rect.bottom = rect.bottom - 1;
		rect.right = rect.right - 1;

		CDC* pDC = GetDC();
		CBrush redBrush(UI_COLOR);
		pDC->FrameRect(&rect, &redBrush);
		ReleaseDC(pDC);
	}

    return NULL;
}

void TaskSheetInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 当点击到板材预览图片时，需要激活对应的板材。
	CRect sheetPreviewStaticRect;
	GetDlgItem(IDC_STATIC_SheetPreview)->GetWindowRect(&sheetPreviewStaticRect);
	ScreenToClient(sheetPreviewStaticRect);
	if (sheetPreviewStaticRect.PtInRect(point))
	{
		OnActiveSheet();
	}

    CLResizableDialog::OnLButtonDown(nFlags, point);
}

void TaskSheetInfoDlg::OnSelectSheet()
{
	if (m_pSheet->GetIsCanvas())
		return;

    // 给爷爷对话框发消息。
    GetParent()->GetParent()->SendMessage(WM_selectOrUnSelectSheetInSheetDlg, 0, 0);
}

void TaskSheetInfoDlg::OnActiveSheet()
{
    LONGLONG* pSheetID = new LONGLONG(m_pSheet->GetID());
    ClWindowFinder::GetCurrentView_2()->SendMessage(WM_activeSheetSubView, (WPARAM)pSheetID, 0);
    delete pSheetID;
}

void TaskSheetInfoDlg::Init(SheetPtr pSheet, TaskItemPtr pTaskItem)
{
    m_pSheet = pSheet;
    m_pTaskItem = pTaskItem;
}

void TaskSheetInfoDlg::ResetData()
{
	m_pSheet.reset();
	m_pTaskItem.reset();
}

bool TaskSheetInfoDlg::IsSelected() const
{
	if (m_pSheet->GetIsCanvas()) {
		LogMgr::DebugWarn(_T("298143"));//此时不应该调用这个函数。
		return false;
	}

    if (((CButton*)GetDlgItem(IDC_CHECK_SelectSheet))->GetCheck() == 1)
    {
        return true;
    }

    return false;
}

void TaskSheetInfoDlg::SelectThisSheet()
{
	if (m_pSheet->GetIsCanvas())
	{
		LogMgr::DebugWarn(_T("774428"));//此时不应该调用这个函数。
		return;
	}

    ((CButton*)GetDlgItem(IDC_CHECK_SelectSheet))->SetCheck(1);
}

void TaskSheetInfoDlg::UnSelectThisSheet()
{
	if (m_pSheet->GetIsCanvas())
	{
		LogMgr::DebugWarn(_T("999312"));//此时不应该调用这个函数。
		return;
	}

    ((CButton*)GetDlgItem(IDC_CHECK_SelectSheet))->SetCheck(0);
}

END_CUTLEADER_NAMESPACE()
