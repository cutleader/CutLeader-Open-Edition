#include "stdafx.h"
#include "TaskPartInfoDlg.h"

#include "baseConst.h"
#include "DataCenterItemLoader.h"
#include "DataBaseManager.h"
#include "ColorSelector.h"
#include "ImgData.h"
#include "ClWindowFinder.h"
#include "ClUILibResource.h"
#include "XMenu.h"
#include "clBaseDataResource.h"

#include "TaskPart.h"
#include "Part.h"
#include "TaskItem.h"
#include "ClTaskResource.h"
#include "SheetList.h"
#include "clTaskUIResource.h"
#include "clPartLayoutUIResource.h"
#include "Sheet.h"
#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartInplaceEditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TaskPartInfoDlg, CLResizableDialog)

TaskPartInfoDlg::TaskPartInfoDlg() : CLResizableDialog(TaskPartInfoDlg::IDD)
{
}

TaskPartInfoDlg::~TaskPartInfoDlg()
{
}

void TaskPartInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATIC_PartPreview, m_previewWnd);
	DDX_Control(pDX, IDC_EDIT_PartName, m_ctrlPartNameLabel);
    DDX_Control(pDX, IDC_STATIC_PlacedPartCount, m_nestedCountLabel);
    DDX_Control(pDX, IDC_EDIT_InitialPartCount, m_initialCountEditBox);
    DDX_Control(pDX, IDC_BTN_PlaceToSheet, m_btnPlacePart);
}

BEGIN_MESSAGE_MAP(TaskPartInfoDlg, CLResizableDialog)
    ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()

    ON_BN_CLICKED(IDC_CHECK_SelectTaskPart, OnSelectTaskPart)
    ON_BN_CLICKED(IDC_BTN_PlaceToSheet, OnPlaceToSheet)
	ON_BN_CLICKED(IDC_InplaceEditPart_InTaskPartList, OnInplaceEditPart)

	ON_MESSAGE(WM_OnCharEventFinished, OnCharEventOfEditBoxFinished)
	ON_MESSAGE(WM_OnKillFocusEventFinished, OnKillFocusEventOfEditBoxFinished)
END_MESSAGE_MAP()

BOOL TaskPartInfoDlg::OnInitDialog()
{
	__super::OnInitDialog();

    // ���ֿؼ�
    {
        AddAnchor(IDC_STATIC_PartPreview, TOP_LEFT);

        AddAnchor(IDC_CHECK_SelectTaskPart, MIDDLE_LEFT);

        AddAnchor(IDC_STATIC_PartIndex, TOP_LEFT);

        AddAnchor(IDC_EDIT_PartName, TOP_LEFT);

        AddAnchor(IDC_STATIC_PlacedPartCount, TOP_LEFT);
        AddAnchor(IDC_STATIC_Slash, TOP_LEFT);
        AddAnchor(IDC_EDIT_InitialPartCount, TOP_LEFT);

        AddAnchor(IDC_BTN_PlaceToSheet, TOP_LEFT);
    }

	m_ctrlPartNameLabel.SetFontBold(TRUE);
	m_ctrlPartNameLabel.SetBkColor(SkinManager::GetBackgroundColor());
	m_nestedCountLabel.SetBkColor(SkinManager::GetBackgroundColor());

    DisplayDataOnDialog();

	SetBackgroundColor(SkinManager::GetBackgroundColor());
	return TRUE;
}

void TaskPartInfoDlg::OnPaint()
{
	__super::OnPaint();
}

void TaskPartInfoDlg::OnDestroy()
{
	CLResizableDialog::OnDestroy();
}

BOOL TaskPartInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	if ((HBRUSH)m_brush == NULL)
		m_brush.CreateSolidBrush(SkinManager::GetBackgroundColor());
	pDC->FillRect(&r, &m_brush);

	ImgData::DrawImgData(m_pTaskPart->GetPart()->GetPreviewData().get(), &m_previewWnd, SkinManager::GetBackgroundColor());

	return TRUE;
}

HBRUSH TaskPartInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

BOOL TaskPartInfoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // ���Ի��򻭸����ʾ������
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

    // ��ʾ��ť
    GetDlgItem(IDC_BTN_PlaceToSheet)->ShowWindow(SW_SHOW);

    return __super::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT TaskPartInfoDlg::OnMouseLeave(WPARAM, LPARAM)
{
    // ��ǰ����Ի�����ǡ��Ǽ���״̬����û�б�Ҫ�ٴ�����Ϊ���Ǽ���״̬����
    if (!GetDlgItem(IDC_BTN_PlaceToSheet)->IsWindowVisible())
    {
        return 0;
    }

	// ����öԻ���Ľ����ڱ༭���ϣ��ñ༭��ʧȥ���뽹�㡣
	GetDlgItem(IDC_STATIC_PlacedPartCount)->SetFocus();

    // ���ذ�ť
    GetDlgItem(IDC_BTN_PlaceToSheet)->ShowWindow(SW_HIDE);

	// ȡ������Ի���ĸ�����
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

void TaskPartInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CLResizableDialog::OnLButtonDown(nFlags, point);
}

void TaskPartInfoDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// �����˵���
	XMenu xMenu;
	xMenu.CreatePopupMenu();

	// ��Ӳ˵��
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_InPlaceEditPart);
	xMenu.AppendMenu(MF_STRING, IDC_InplaceEditPart_InTaskPartList, str.AllocSysString());

	// �����˵���
	SetMenu(&xMenu);
	xMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

void TaskPartInfoDlg::OnSelectTaskPart()
{
    // ��үү�Ի�����Ϣ��
    GetParent()->GetParent()->SendMessage(WM_selectOrUnSelectPartInTaskPartDlg, 0, 0);
}

void TaskPartInfoDlg::OnPlaceToSheet()
{
    LONGLONG* pPartID = new LONGLONG(m_pTaskPart->GetPart()->GetID());
    ClWindowFinder::GetCurrentView_2()->SendMessage(WM_placePartOnCurrentSheetSubView, (WPARAM)pPartID, 0);
    delete pPartID;
}

void TaskPartInfoDlg::OnInplaceEditPart()
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_WhetherInplaceEditPart);
	if (MessageBox(str, strProductName, MB_YESNO | MB_ICONQUESTION) != IDYES) {
		return;
	}

	// �õ������ж�Ӧ�������������á�
	PartPlacementListPtr pPartPlacements(new PartPlacementList);
	for (unsigned int i = 0; i < m_pTaskItem->GetSheetList()->size(); i++)
	{
		const Sheet* pSheet = m_pTaskItem->GetSheetList()->operator[](i).get();
		if (pSheet->GetIsCanvas())
			continue;
		PartPlacementListPtr pTmpPartPlacements = pSheet->GetPartPlacements()->GetPartPlacementByPart(m_pTaskPart->GetPart()->GetID());
		pPartPlacements->AppendPartPmts(pTmpPartPlacements.get());
	}

	// ��ʼ����λ�༭���ݡ�
	int iCurrentTabIndex = ClWindowFinder::GetCurrentDocTabIndex(); // ��ǰ�����ĵ���Ӧ�ı�ǩҳ��
	PartCamDataPtr pPartCamData;
	if (pPartPlacements->size() > 0)
		pPartCamData = pPartPlacements->front()->GetCamData();
	PartInplaceEditData::GetInstance()->InitInplaceEditData(iCurrentTabIndex, m_pTaskPart->GetPart_FW(), pPartCamData);

	// �����̷߳�����Ϣ�����������������λ�༭��
	AfxGetApp()->PostThreadMessage(WM_InPlaceEditPart, NULL, NULL);
}

LRESULT TaskPartInfoDlg::OnKillFocusEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	int iInitialCount = (int)m_initialCountEditBox.Get_Number();
	if (iInitialCount == INVALID_VALUE) // �����ǰ�༭�����ǿ�ֵ����Ҫ�ָ��༭���е�ֵΪ��һ�ε�����ֵ��
	{
		m_initialCountEditBox.Init(m_pTaskPart->GetPlanningCount(), 0, FALSE);
		CString str;
		str.Format(_T("%d"), m_pTaskPart->GetPlanningCount());
		GetDlgItem(IDC_EDIT_InitialPartCount)->SetWindowText(str);
	}

	return true;
}

LRESULT TaskPartInfoDlg::OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	m_initialCountEditBox.UpdateNumEdit_fromUI();
	int iNewInitialCount = (int)m_initialCountEditBox.Get_Number();

	// �����ǰ�༭�����ǿ�ֵ�������κδ������Ǽ����û�����������룬��Ϊ��ֵû�����壬��һ�û����������ˣ��ڱ༭��ʧȥ����ʱ��Ҫ�ָ��༭���е�ֵΪ��һ�ε�����ֵ��
	if (iNewInitialCount == INVALID_VALUE)
	{
		return true;
	}

	if (iNewInitialCount != m_pTaskPart->GetPlanningCount()) // �޸��������
	{
		m_pTaskPart->SetPlanningCount(iNewInitialCount);
		m_pTaskPart->SetModified(TRUE);

		// ����������ɫ
		int iNestCount = m_pTaskItem->GetSheetList()->GetPartNestedCount(m_pTaskPart->GetPart()->GetID());
		if (iNestCount > m_pTaskPart->GetPlanningCount())
		{
			m_nestedCountLabel.SetTextColor(RGB(255, 0, 0));
		}
		else
		{
			m_nestedCountLabel.SetTextColor(RGB(0, 0, 0));
		}
		m_nestedCountLabel.SetBkColor(UI_COLOR);
	}

	return true;
}

void TaskPartInfoDlg::Init(TaskPartPtr pTaskPart, TaskItemPtr pTaskItem)
{
    m_pTaskPart = pTaskPart;
    m_pTaskItem = pTaskItem;
}

void TaskPartInfoDlg::ResetData()
{
	m_pTaskPart.reset();
	m_pTaskItem.reset();
}

bool TaskPartInfoDlg::IsSelected() const
{
    if (((CButton*)GetDlgItem(IDC_CHECK_SelectTaskPart))->GetCheck() == 1)
    {
        return true;
    }

    return false;
}

void TaskPartInfoDlg::ReDisplayNestedCount()
{
	CString str, strValueInEditBox;
	int iNestedCount = m_pTaskItem->GetSheetList()->GetPartNestedCount(m_pTaskPart->GetPart()->GetID());
	str.Format(_T("%d"), iNestedCount);
	GetDlgItem(IDC_STATIC_PlacedPartCount)->GetWindowText(strValueInEditBox);
	if (str != strValueInEditBox)
	{
		GetDlgItem(IDC_STATIC_PlacedPartCount)->SetWindowText(str);
		if (iNestedCount > m_pTaskPart->GetPlanningCount())
		{
			m_nestedCountLabel.SetTextColor(RGB(255, 0, 0));
		}
		else
		{
			m_nestedCountLabel.SetTextColor(RGB(0, 0, 0));
		}
	}
}

void TaskPartInfoDlg::DisplayDataOnDialog()
{
    // ��ʾ�������
    CString str;
    str.Format(_T("%d"), m_pTaskItem->GetTaskPartIndex(m_pTaskPart->GetID()) + 1);
    GetDlgItem(IDC_STATIC_PartIndex)->SetWindowText(str);

    // ��ʾ�������
	m_ctrlPartNameLabel.SetText(m_pTaskPart->GetPart()->GetName());

    // ��ʾ���ŵ����������
    int iNestCount = m_pTaskItem->GetSheetList()->GetPartNestedCount(m_pTaskPart->GetPart()->GetID());
    str.Format(_T("%d"), iNestCount);
    GetDlgItem(IDC_STATIC_PlacedPartCount)->SetWindowText(str);
    if (iNestCount > m_pTaskPart->GetPlanningCount())
    {
        m_nestedCountLabel.SetTextColor(RGB(255,0,0));
    }
    else
    {
        m_nestedCountLabel.SetTextColor(RGB(0,0,0));
    }
    m_nestedCountLabel.SetBkColor(UI_COLOR);

    // ��ʾ�����ʼ����
    m_initialCountEditBox.Init(m_pTaskPart->GetPlanningCount(), 0, FALSE);

    // ���ذ�ť
    GetDlgItem(IDC_BTN_PlaceToSheet)->ShowWindow(SW_HIDE);

    ImgData::DrawImgData(m_pTaskPart->GetPart()->GetPreviewData().get(), &m_previewWnd, SkinManager::GetBackgroundColor());
}

void TaskPartInfoDlg::SelectThisTaskPart()
{
    ((CButton*)GetDlgItem(IDC_CHECK_SelectTaskPart))->SetCheck(1);
}

void TaskPartInfoDlg::UnSelectThisTaskPart()
{
    ((CButton*)GetDlgItem(IDC_CHECK_SelectTaskPart))->SetCheck(0);
}

END_CUTLEADER_NAMESPACE()
