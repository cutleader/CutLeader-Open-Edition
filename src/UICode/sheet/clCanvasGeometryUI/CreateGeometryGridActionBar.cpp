#include "StdAfx.h"
#include "CreateGeometryGridActionBar.h"

#include "baseConst.h"
#include "clGeometryBaseResource.h"
#include "GlViewPort.h"
#include "clBaseDataResource.h"
#include "clUILibResource.h"

#include "CLOptionInfo.h"
#include "SheetEditParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(CreateGeometryGridActionBar, ActionBar)

CreateGeometryGridActionBar::CreateGeometryGridActionBar(CWnd* pParent /*=NULL*/)
	: ActionBar(CreateGeometryGridActionBar::IDD, pParent)
{
	m_pGeometryGridParam.reset(new GeometryGridParam);
}

CreateGeometryGridActionBar::~CreateGeometryGridActionBar(void)
{
}

void CreateGeometryGridActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_RowSpacing, m_ctrlRowSpacing);
	DDX_Control(pDX, IDC_EDIT_ColumnSpacing, m_ctrlColumnSpacing);
	DDX_Control(pDX, IDC_EDIT_ROW_COUNT, m_ctrlRowNum);
	DDX_Control(pDX, IDC_EDIT_COLUMN_COUNT, m_ctrlColumnNum);
	DDX_Control(pDX, IDC_COMBO_GRID_DIR, m_ctrlGridDir);

	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BOOL CreateGeometryGridActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	// 阵列间距。
	m_ctrlRowSpacing.Init(m_pGeometryGridParam->GetRowSpacing(), DIGITAL_NUM, false, .0);
	m_ctrlColumnSpacing.Init(m_pGeometryGridParam->GetColumnSpacing(), DIGITAL_NUM, false, .0);

	// 行列数。
	m_ctrlRowNum.Init(m_pGeometryGridParam->GetRowNum(), 0, false, 1.0);
	m_ctrlColumnNum.Init(m_pGeometryGridParam->GetColumnNum(), 0, false, 1.0);

	// 阵列方向。
	CString strGridDir;
	strGridDir = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RIGHT_TOP);
	m_ctrlGridDir.InsertString(0, strGridDir); // 右上。
	strGridDir = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RIGHT_BOTTOM);
	m_ctrlGridDir.InsertString(1, strGridDir); // 右下。
	strGridDir = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LEFT_TOP);
	m_ctrlGridDir.InsertString(2, strGridDir); // 左上。
	strGridDir = MultiLanguageMgr::GetInstance()->TranslateString(IDS_LEFT_BOTTOM);
	m_ctrlGridDir.InsertString(3, strGridDir); // 左下。
	m_ctrlGridDir.SetCurSel(0);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_COMBO_GRID_DIR )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_STATIC_Spacing)->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem(IDC_STATIC_Quantity)->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

BEGIN_MESSAGE_MAP(CreateGeometryGridActionBar, ActionBar)
	ON_MESSAGE(WM_OnCharEventFinished, OnCharEventOfEditBoxFinished)
	ON_CBN_SELCHANGE(IDC_COMBO_GRID_DIR, OnSelchangeGridType)
	ON_BN_CLICKED(IDC_BTN_OK, OnOK)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

LRESULT CreateGeometryGridActionBar::OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = ::GetFocus();
	int iCtrlID = ::GetDlgCtrlID(hwnd);
	if (iCtrlID == IDC_EDIT_RowSpacing)
	{
		m_ctrlRowSpacing.UpdateNumEdit_fromUI();
		double dRowSpacing = m_ctrlRowSpacing.Get_Number();
		if ((int)dRowSpacing == INVALID_VALUE) // 输入的数字者为空。
			return true;
		if (fabs(dRowSpacing - m_pGeometryGridParam->GetRowSpacing()) > GEOM_TOLERANCE)
		{
			m_pGeometryGridParam->SetRowSpacing(dRowSpacing);
			NotifyAllObserver(ID_EVENT_GridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_ColumnSpacing)
	{
		m_ctrlColumnSpacing.UpdateNumEdit_fromUI();
		double dColumnSpacing = m_ctrlColumnSpacing.Get_Number();
		if ((int)dColumnSpacing == INVALID_VALUE) // 输入的数字者为空。
			return true;
		if (fabs(dColumnSpacing - m_pGeometryGridParam->GetColumnSpacing()) > GEOM_TOLERANCE)
		{
			m_pGeometryGridParam->SetColumnSpacing(dColumnSpacing);
			NotifyAllObserver(ID_EVENT_GridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_ROW_COUNT)
	{
		m_ctrlRowNum.UpdateNumEdit_fromUI();
		int iRowNum = (int)m_ctrlRowNum.Get_Number();
		if (iRowNum == INVALID_VALUE) // 输入的数字为空。
			return true;
		if (iRowNum < 1) // 输入的数字太小。
		{
			m_ctrlRowNum.SetNumber(m_pGeometryGridParam->GetRowNum());
			iRowNum = (int)m_ctrlRowNum.Get_Number();
		}
		else if (iRowNum != m_pGeometryGridParam->GetRowNum())
		{
			m_pGeometryGridParam->SetRowNum(iRowNum);
			NotifyAllObserver(ID_EVENT_GridParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_COLUMN_COUNT)
	{
		m_ctrlColumnNum.UpdateNumEdit_fromUI();
		int iColumnNum = (int)m_ctrlColumnNum.Get_Number();
		if (iColumnNum == INVALID_VALUE) // 输入的数字为空。
			return true;
		if (iColumnNum < 1) // 输入的数字太小。
		{
			m_ctrlColumnNum.SetNumber(m_pGeometryGridParam->GetColumnNum());
			iColumnNum = (int)m_ctrlColumnNum.Get_Number();
		}
		else if (iColumnNum != m_pGeometryGridParam->GetColumnNum())
		{
			m_pGeometryGridParam->SetColumnNum(iColumnNum);
			NotifyAllObserver(ID_EVENT_GridParamChanged, CComVariant());
		}
	}

	return true;
}

void CreateGeometryGridActionBar::OnSelchangeGridType()
{
	GeometryGridDirType emGeometryGridDirType;
	if (m_ctrlGridDir.GetCurSel() == 0)
		emGeometryGridDirType = GRID_RIGHT_TOP;
	else if (m_ctrlGridDir.GetCurSel() == 1)
		emGeometryGridDirType = GRID_RIGHT_BOTTOM;
	else if (m_ctrlGridDir.GetCurSel() == 2)
		emGeometryGridDirType = GRID_LEFT_TOP;
	else if (m_ctrlGridDir.GetCurSel() == 3)
		emGeometryGridDirType = GRID_LEFT_BOTTOM;

	if (emGeometryGridDirType != m_pGeometryGridParam->GetGeometryGridDir())
	{
		m_pGeometryGridParam->SetGeometryGridDir(emGeometryGridDirType);
		NotifyAllObserver(ID_EVENT_GridParamChanged, CComVariant());
	}
}

void CreateGeometryGridActionBar::OnOK()
{
	NotifyAllObserver(ID_EVENT_GridGeometry_OK, CComVariant());
	ShowWindow(SW_HIDE);
}

void CreateGeometryGridActionBar::OnCancel()
{
	NotifyAllObserver(ID_EVENT_GridGeometry_Cancel, CComVariant());
	ShowWindow(SW_HIDE);
}

void CreateGeometryGridActionBar::Show_CreateGeometryGridActionBar()
{
	PositionActionBar();
}

END_CUTLEADER_NAMESPACE()
