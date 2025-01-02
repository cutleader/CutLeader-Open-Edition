#include "StdAfx.h"
#include "CreatePartGridActionBar.h"

#include "baseConst.h"
#include "clGeometryBaseResource.h"
#include "GlViewPort.h"
#include "clBaseDataResource.h"
#include "DataBaseManager.h"
#include "clUILibResource.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"

#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartCadData.h"
#include "PartPlacement.h"
#include "PartPlacementList.h"
#include "PartPlacementManager.h"
#include "clPartLayoutResource.h"
#include "PartLayoutHelper.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CreatePartGridActionBar, ActionBar)

CreatePartGridActionBar::CreatePartGridActionBar(CWnd* pParent /*=NULL*/)
	: ActionBar(CreatePartGridActionBar::IDD, pParent)
{
	m_pPartGridParam.reset(new PartGridParam);
}

CreatePartGridActionBar::~CreatePartGridActionBar(void)
{
}

void CreatePartGridActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_RowSpacing, m_ctrlRowSpacing);
	DDX_Control(pDX, IDC_EDIT_ColumnSpacing, m_ctrlColumnSpacing);
	DDX_Control(pDX, IDC_EDIT_ROW_COUNT, m_ctrlRowNum);
	DDX_Control(pDX, IDC_EDIT_COLUMN_COUNT, m_ctrlColumnNum);
	DDX_Control(pDX, IDC_COMBO_GRID_DIR, m_ctrlGridDir);

	DDX_Check(pDX, IDC_CHECK_UsePartDistance, m_bUsePartDistance);
	DDX_Control(pDX, IDC_EDIT_PART_GAP, m_dPartDistance);
	DDX_Check(pDX, IDC_CHECK_COMCUT, m_bCommonCut);

	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BOOL CreatePartGridActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	//
	m_ctrlRowSpacing.Init_WithoutMsg(m_pPartGridParam->GetRowSpacing(), 4, false, .0);
	m_ctrlColumnSpacing.Init_WithoutMsg(m_pPartGridParam->GetColumnSpacing(), 4, false, .0);
	m_ctrlRowNum.Init_WithoutMsg(m_pPartGridParam->GetRowNum(), 0, false, 1.0);
	m_ctrlColumnNum.Init_WithoutMsg(m_pPartGridParam->GetColumnNum(), 0, false, 1.0);

	// 阵列方向。
	m_ctrlGridDir.InsertString(0, MultiLanguageMgr::GetInstance()->TranslateString(IDS_RIGHT_TOP)); // 右上。
	m_ctrlGridDir.InsertString(1, MultiLanguageMgr::GetInstance()->TranslateString(IDS_RIGHT_BOTTOM)); // 右下。
	m_ctrlGridDir.InsertString(2, MultiLanguageMgr::GetInstance()->TranslateString(IDS_LEFT_TOP)); // 左上。
	m_ctrlGridDir.InsertString(3, MultiLanguageMgr::GetInstance()->TranslateString(IDS_LEFT_BOTTOM)); // 左下。
	m_ctrlGridDir.SetCurSel(0);

	// 基于零件间距创建阵列。
	m_bUsePartDistance = m_pPartGridParam->GetIsUsePartDistance();
	m_dPartDistance.Init_WithoutMsg(m_pPartGridParam->GetPartDistance(), 4, false, .0);
	m_bCommonCut = m_pPartGridParam->GetIsCommonCut();

	if (m_bUsePartDistance) {
		GetDlgItem(IDC_STATIC_RowSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RowSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ColumnSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ColumnSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PartDistance)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PART_GAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_COMCUT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_STATIC_RowSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RowSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ColumnSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ColumnSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PartDistance)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PART_GAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_COMCUT)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	// 按钮提示。
	m_btnOK.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_GRID_OK));
	m_btnCancel.SetTooltip(MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_GRID_CANCEL));

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme(GetDlgItem(IDC_STATIC_Spacing)->GetSafeHwnd(), L"", L"");
	SetWindowTheme( GetDlgItem( IDC_COMBO_GRID_DIR )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_ROW_COLUMN_NUM )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme(GetDlgItem(IDC_STATIC_PART_SPACE)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_UsePartDistance)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK_COMCUT)->GetSafeHwnd(), L"", L"");

	return TRUE;
}

BEGIN_MESSAGE_MAP(CreatePartGridActionBar, ActionBar)
	ON_MESSAGE(WM_OnCharEventFinished, OnCharEventOfEditBoxFinished)
	ON_CBN_SELCHANGE(IDC_COMBO_GRID_DIR, OnSelchangeGridType)
	ON_BN_CLICKED(IDC_CHECK_UsePartDistance, OnCheck_UsePartDistance)
	ON_BN_CLICKED(IDC_CHECK_COMCUT, OnCheckComcut)

	ON_BN_CLICKED(IDC_BTN_OK, OnOK)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

LRESULT CreatePartGridActionBar::OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = ::GetFocus();
	int iCtrlID = ::GetDlgCtrlID(hwnd);
	if (iCtrlID == IDC_EDIT_RowSpacing)
	{
		m_ctrlRowSpacing.UpdateNumEdit_fromUI();
		double dRowSpacing = m_ctrlRowSpacing.Get_Number();
		if ((int)dRowSpacing == INVALID_VALUE) // 输入的数字者为空。
			return true;
		if (fabs(dRowSpacing - m_pPartGridParam->GetRowSpacing()) > GEOM_TOLERANCE) {
			m_pPartGridParam->SetRowSpacing(dRowSpacing);
			NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_ColumnSpacing)
	{
		m_ctrlColumnSpacing.UpdateNumEdit_fromUI();
		double dColumnSpacing = m_ctrlColumnSpacing.Get_Number();
		if ((int)dColumnSpacing == INVALID_VALUE) // 输入的数字者为空。
			return true;
		if (fabs(dColumnSpacing - m_pPartGridParam->GetColumnSpacing()) > GEOM_TOLERANCE) {
			m_pPartGridParam->SetColumnSpacing(dColumnSpacing);
			NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_PART_GAP)
	{
		m_dPartDistance.UpdateNumEdit_fromUI();
		double dPartDistance = m_dPartDistance.Get_Number();
		if ((int)dPartDistance == INVALID_VALUE) // 输入的数字者为空。
			return true;
		if (fabs(dPartDistance - m_pPartGridParam->GetPartDistance()) > GEOM_TOLERANCE) {
			m_pPartGridParam->SetPartDistance(dPartDistance);
			NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_ROW_COUNT)
	{
		m_ctrlRowNum.UpdateNumEdit_fromUI();
		int iRowNum = (int)m_ctrlRowNum.Get_Number();
		if (iRowNum == INVALID_VALUE) // 输入的数字为空。
			return true;
		if (iRowNum < 1) { // 输入的数字太小。
			m_ctrlRowNum.SetNumber(m_pPartGridParam->GetRowNum());
			iRowNum = (int)m_ctrlRowNum.Get_Number();
		}
		else if (iRowNum != m_pPartGridParam->GetRowNum()) {
			m_pPartGridParam->SetRowNum(iRowNum);
			NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_COLUMN_COUNT)
	{
		m_ctrlColumnNum.UpdateNumEdit_fromUI();
		int iColumnNum = (int)m_ctrlColumnNum.Get_Number();
		if (iColumnNum == INVALID_VALUE) // 输入的数字为空。
			return true;
		if (iColumnNum < 1) { // 输入的数字太小。
			m_ctrlColumnNum.SetNumber(m_pPartGridParam->GetColumnNum());
			iColumnNum = (int)m_ctrlColumnNum.Get_Number();
		}
		else if (iColumnNum != m_pPartGridParam->GetColumnNum()) {
			m_pPartGridParam->SetColumnNum(iColumnNum);
			NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
		}
	}

	return true;
}

void CreatePartGridActionBar::OnSelchangeGridType()
{
	PART_GRID_DIR emPartGridDirType;
	if (m_ctrlGridDir.GetCurSel() == 0)
		emPartGridDirType = PART_GRID_RIGHT_TOP;
	else if (m_ctrlGridDir.GetCurSel() == 1)
		emPartGridDirType = PART_GRID_RIGHT_BOTTOM;
	else if (m_ctrlGridDir.GetCurSel() == 2)
		emPartGridDirType = PART_GRID_LEFT_TOP;
	else if (m_ctrlGridDir.GetCurSel() == 3)
		emPartGridDirType = PART_GRID_LEFT_BOTTOM;

	if (emPartGridDirType != m_pPartGridParam->GetPartGridDir()) {
		m_pPartGridParam->SetPartGridDir(emPartGridDirType);
		NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
	}
}

void CreatePartGridActionBar::OnCheck_UsePartDistance()
{
	UpdateData(TRUE);
	if (m_bUsePartDistance) {
		GetDlgItem(IDC_STATIC_RowSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RowSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ColumnSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ColumnSpacing)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PartDistance)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PART_GAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_COMCUT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_STATIC_RowSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RowSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ColumnSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ColumnSpacing)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PartDistance)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PART_GAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_COMCUT)->EnableWindow(FALSE);
	}

	if (m_bUsePartDistance != m_pPartGridParam->GetIsUsePartDistance()) {
		m_pPartGridParam->SetIsUsePartDistance(m_bUsePartDistance);
		NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
	}
}

void CreatePartGridActionBar::OnCheckComcut()
{
	UpdateData(TRUE);
	if (m_bCommonCut) {
		GetDlgItem(IDC_STATIC_PartDistance)->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_STATIC_PartDistance)->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_PART_GAP))->EnableWindow(TRUE);
	}

	if (m_bCommonCut != m_pPartGridParam->GetIsCommonCut()) {
		m_pPartGridParam->SetIsCommonCut(m_bCommonCut);
		NotifyAllObserver(ID_EVENT_PartGridParamChanged, CComVariant());
	}
}

void CreatePartGridActionBar::OnOK()
{
	NotifyAllObserver(ID_EVENT_GridPart_OK, CComVariant());
	ShowWindow(SW_HIDE);
}

void CreatePartGridActionBar::OnCancel()
{
	NotifyAllObserver(ID_EVENT_GridPart_Cancel, CComVariant());
	ShowWindow(SW_HIDE);
}

void CreatePartGridActionBar::Show_CreatePartGridActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()
