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

    // ���ֿؼ�
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
	if (m_pSheet->GetIsCanvas()) // ��ͼ���
	{
		GetDlgItem(IDC_STATIC_SheetIndex)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SheetCount)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SelectSheet)->ShowWindow(SW_HIDE);
		// ע�⣬����û������������ǩ�����ǰ�������Ϊ�գ���������������һ���������������Ԥ��ͼ����ʧ��ԭ���ǰ�����Ʊ�ǩ��XLabel���ͣ�
		// Ӧ����XLabel�ؼ���BUG������Ϊһ��workaround����Ϊ��ͼС�Ի�����Զ�ǵ�һ����������ô��û��ϵ��
		GetDlgItem(IDC_STATIC_Count)->SetWindowText(_T(""));
	}
	else // �������
	{
		// ��ʾ�������
		CString str;
		str.Format(_T("%d"), m_pTaskItem->GetSheetIndex(m_pSheet->GetID()));
		GetDlgItem(IDC_STATIC_SheetIndex)->SetWindowText(str);

		// ��ʾ���������
		m_countEditBox.Init(m_pSheet->GetSheetCount(), 0, FALSE);
	}

    // ��ʾ�������
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
    // ���Ի��򻭸�������
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

    // Ҫ�Ǳ༭�򲻿ɱ༭��ʹ֮�ɱ༭��
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
	if (iSheetCount == 0 || iSheetCount == INVALID_VALUE) // �����ǰ�༭������0���ֵ����Ҫ�ָ��༭���е�ֵΪ��һ�ε�����ֵ��
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

	// �����ǰ�༭�����ǿ�ֵ��0�������κδ������Ǽ����û�����������룬��Ϊ��ʱû�����壬��һ�û����������ˣ��ڱ༭��ʧȥ����ʱ��Ҫ�ָ��༭���е�ֵΪ��һ�ε�����ֵ��
	if (iNewSheetCount == 0 || iNewSheetCount == INVALID_VALUE)
	{
		return true;
	}

	// �޸��������
	if (iNewSheetCount != m_pSheet->GetSheetCount())
	{
		m_pSheet->SetSheetCount(iNewSheetCount);
		m_pSheet->SetModified(TRUE);

		// ����Ϣ��������ͼ��������������������������
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);
	}

	return true;
}

LRESULT TaskSheetInfoDlg::OnMouseLeave(WPARAM, LPARAM)
{
    // ��ǰ����Ի�����ǡ��Ǽ���״̬����û�б�Ҫ�ٴ�����Ϊ���Ǽ���״̬����
    if (!m_bThisDialogIsActive)
    {
        return 0;
    }

	// ����öԻ���Ľ����ڱ༭���ϣ��ñ༭��ʧȥ���뽹�㡣
	if (!m_pSheet->GetIsCanvas())
		GetDlgItem(IDC_CHECK_SelectSheet)->SetFocus();

	m_bThisDialogIsActive = false;

	// ���Ի��򻭸�������
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
	// ����������Ԥ��ͼƬʱ����Ҫ�����Ӧ�İ�ġ�
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

    // ��үү�Ի�����Ϣ��
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
		LogMgr::DebugWarn(_T("298143"));//��ʱ��Ӧ�õ������������
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
		LogMgr::DebugWarn(_T("774428"));//��ʱ��Ӧ�õ������������
		return;
	}

    ((CButton*)GetDlgItem(IDC_CHECK_SelectSheet))->SetCheck(1);
}

void TaskSheetInfoDlg::UnSelectThisSheet()
{
	if (m_pSheet->GetIsCanvas())
	{
		LogMgr::DebugWarn(_T("999312"));//��ʱ��Ӧ�õ������������
		return;
	}

    ((CButton*)GetDlgItem(IDC_CHECK_SelectSheet))->SetCheck(0);
}

END_CUTLEADER_NAMESPACE()
