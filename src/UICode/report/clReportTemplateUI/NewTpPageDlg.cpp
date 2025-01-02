#include "StdAfx.h"
#include "NewTpPageDlg.h"

#include "baseConst.h"
#include "UnitHelper.h"

#include "reportConst.h"
#include "clReportResource.h"
#include "ReportTplPage.h"
#include "TemplateParam.h"
#include "TplDisplayParam.h"
#include "TplDesignConfig.h"
#include "TplEntityList.h"
#include "EntityGroupList.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(NewTpPageDlg, CDialogEx)

NewTpPageDlg::NewTpPageDlg(void) : CLDialog(NewTpPageDlg::IDD)
{
	m_strTplPageName = _T("New Template Page");
}

NewTpPageDlg::~NewTpPageDlg(void)
{
}

void NewTpPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// basic info.
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTplPageName);
	DDX_Control(pDX, IDC_COMBO_TPL_PAGE_TYPE, m_ctrlTplPageType);

	// paper size.
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_dPaperWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_dPaperHeight);

	// paper margin
	DDX_Control(pDX, IDC_EDIT_LEFT_MARGIN, m_dLeftMargin);
	DDX_Control(pDX, IDC_EDIT_RIGHT_MARGIN, m_dRightMargin);
	DDX_Control(pDX, IDC_EDIT_TOP_MARGIN, m_dTopMargin);
	DDX_Control(pDX, IDC_EDIT_BOTTOM_MARGIN, m_dBottomMargin);
	
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(NewTpPageDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

BOOL NewTpPageDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	/************************************************************************/
	// init the data.

	// get the config info.
	TemplateParamPtr pTemplateParam = TplDesignConfig::GetInstance()->GetTemplateParam();
	TplDisplayParamPtr pDisplayParam = TplDesignConfig::GetInstance()->GetTplDisplayParam();

	// paper size and paper margin.
	if (pDisplayParam->GetTplUnit() == TPL_UNIT_PIXEL)
	{
		m_dPaperWidth.Init(pTemplateParam->GetPaperWidth(), 0, FALSE);
		m_dPaperHeight.Init(pTemplateParam->GetPaperHeight(), 0, FALSE);

		m_dLeftMargin.Init(pTemplateParam->GetLeftMargin(), 0, FALSE);
		m_dRightMargin.Init(pTemplateParam->GetRightMargin(), 0, FALSE);
		m_dTopMargin.Init(pTemplateParam->GetTopMargin(), 0, FALSE);
		m_dBottomMargin.Init(pTemplateParam->GetBottomMargin(), 0, FALSE);
	}
	else if (pDisplayParam->GetTplUnit() == TPL_UNIT_INCH)
	{
		m_dPaperWidth.Init(UnitHelper::PixelsToInches(pTemplateParam->GetPaperWidth()), TPL_DIGITAL_NUM, FALSE);
		m_dPaperHeight.Init(UnitHelper::PixelsToInches(pTemplateParam->GetPaperHeight()), TPL_DIGITAL_NUM, FALSE);

		m_dLeftMargin.Init(UnitHelper::PixelsToInches(pTemplateParam->GetLeftMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dRightMargin.Init(UnitHelper::PixelsToInches(pTemplateParam->GetRightMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dTopMargin.Init(UnitHelper::PixelsToInches(pTemplateParam->GetTopMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dBottomMargin.Init(UnitHelper::PixelsToInches(pTemplateParam->GetBottomMargin()), TPL_DIGITAL_NUM, FALSE);
	}
	else if (pDisplayParam->GetTplUnit() == TPL_UNIT_CM)
	{
		m_dPaperWidth.Init(UnitHelper::PixelsToCentimeters(pTemplateParam->GetPaperWidth()), TPL_DIGITAL_NUM, FALSE);
		m_dPaperHeight.Init(UnitHelper::PixelsToCentimeters(pTemplateParam->GetPaperHeight()), TPL_DIGITAL_NUM, FALSE);

		m_dLeftMargin.Init(UnitHelper::PixelsToCentimeters(pTemplateParam->GetLeftMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dRightMargin.Init(UnitHelper::PixelsToCentimeters(pTemplateParam->GetRightMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dTopMargin.Init(UnitHelper::PixelsToCentimeters(pTemplateParam->GetTopMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dBottomMargin.Init(UnitHelper::PixelsToCentimeters(pTemplateParam->GetBottomMargin()), TPL_DIGITAL_NUM, FALSE);
	}

	// init the combo box.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_SHEETINFO);
	m_ctrlTplPageType.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_TASK);
	m_ctrlTplPageType.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_PART_LABEL);
	m_ctrlTplPageType.AddString(str);
	m_ctrlTplPageType.SetCurSel(0);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_SIZE2 )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_TPL_PAGE_TYPE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_SIZE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_MARGIN )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH NewTpPageDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NewTpPageDlg::OnOK() 
{
	UpdateData(TRUE);

	/************************************************************************/
	// paper size and paper margin.

	TplDisplayParamPtr pDisplayParam = TplDesignConfig::GetInstance()->GetTplDisplayParam();

	// get the data from the UI.
	int iPaperWidth, iPaperHeight, iLeftMargin, iRightMargin, iTopMargin, iBottomMargin;
	if (pDisplayParam->GetTplUnit() == TPL_UNIT_PIXEL)
	{
		iPaperWidth = (int)m_dPaperWidth.Get_Number();
		iPaperHeight = (int)m_dPaperHeight.Get_Number();

		iLeftMargin = (int)m_dLeftMargin.Get_Number();
		iRightMargin = (int)m_dRightMargin.Get_Number();
		iTopMargin = (int)m_dTopMargin.Get_Number();
		iBottomMargin = (int)m_dBottomMargin.Get_Number();
	}
	else if (pDisplayParam->GetTplUnit() == TPL_UNIT_INCH)
	{
		iPaperWidth = UnitHelper::InchesToPixels(m_dPaperWidth.Get_Number());
		iPaperHeight = UnitHelper::InchesToPixels(m_dPaperHeight.Get_Number());

		iLeftMargin = UnitHelper::InchesToPixels(m_dLeftMargin.Get_Number());
		iRightMargin = UnitHelper::InchesToPixels(m_dRightMargin.Get_Number());
		iTopMargin = UnitHelper::InchesToPixels(m_dTopMargin.Get_Number());
		iBottomMargin = UnitHelper::InchesToPixels(m_dBottomMargin.Get_Number());
	}
	else if (pDisplayParam->GetTplUnit() == TPL_UNIT_CM)
	{
		iPaperWidth = UnitHelper::CentimeterToPixels(m_dPaperWidth.Get_Number());
		iPaperHeight = UnitHelper::CentimeterToPixels(m_dPaperHeight.Get_Number());

		iLeftMargin = UnitHelper::CentimeterToPixels(m_dLeftMargin.Get_Number());
		iRightMargin = UnitHelper::CentimeterToPixels(m_dRightMargin.Get_Number());
		iTopMargin = UnitHelper::CentimeterToPixels(m_dTopMargin.Get_Number());
		iBottomMargin = UnitHelper::CentimeterToPixels(m_dBottomMargin.Get_Number());
	}
	/************************************************************************/

	/************************************************************************/
	// build the report template page.

	m_pReportTplPage.reset(new ReportTplPage);

	// basic info.
	m_pReportTplPage->SetName(m_strTplPageName);
	if (m_ctrlTplPageType.GetCurSel() == 0)
		m_pReportTplPage->SetTplPageType(ONE_SHEET_PAGE);
	else if (m_ctrlTplPageType.GetCurSel() == 1)
		m_pReportTplPage->SetTplPageType(ONE_TASK_PAGE);
	else if (m_ctrlTplPageType.GetCurSel() == 2)
		m_pReportTplPage->SetTplPageType(PART_LABEL_PAGE);

	// the size of the paper.
	m_pReportTplPage->SetPaperWidth(iPaperWidth);
	m_pReportTplPage->SetPaperHeight(iPaperHeight);

	// the margin of the paper.
	m_pReportTplPage->SetLeftMargin(iLeftMargin);
	m_pReportTplPage->SetRightMargin(iRightMargin);
	m_pReportTplPage->SetTopMargin(iTopMargin);
	m_pReportTplPage->SetBottomMargin(iBottomMargin);

	// the entities in the template.
	TplEntityListPtr pTplEntityList(new TplEntityList);
	m_pReportTplPage->SetTplEntityList(pTplEntityList);

	// the entity groups in the template.
	EntityGroupListPtr pEntityGroupList(new EntityGroupList);
	m_pReportTplPage->SetEntityGroupList(pEntityGroupList);
	/************************************************************************/

	CDialogEx::OnOK();
}

void NewTpPageDlg::OnCancel() 
{
	CDialogEx::OnCancel();
}

END_CUTLEADER_NAMESPACE()
