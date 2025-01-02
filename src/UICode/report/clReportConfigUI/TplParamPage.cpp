#include "StdAfx.h"
#include "TplParamPage.h"

#include "baseConst.h"
#include "UnitHelper.h"

#include "clReportResource.h"
#include "reportConst.h"
#include "FontParam.h"
#include "TemplateParam.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TplParamPage, CResizableDialog)

TplParamPage::TplParamPage(void) : CLResizableDialog(TplParamPage::IDD)
{
	m_strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TPL_PARAM);
}

TplParamPage::~TplParamPage(void)
{
}

void TplParamPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MSG, m_strMsg);

	// paper size.
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_dPaperWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_dPaperHeight);

	// paper margin
	DDX_Control(pDX, IDC_EDIT_LEFT_MARGIN, m_dLeftMargin);
	DDX_Control(pDX, IDC_EDIT_RIGHT_MARGIN, m_dRightMargin);
	DDX_Control(pDX, IDC_EDIT_TOP_MARGIN, m_dTopMargin);
	DDX_Control(pDX, IDC_EDIT_BOTTOM_MARGIN, m_dBottomMargin);

	DDX_Control(pDX, IDC_COMBO_TEXT_JUST_TYPE, m_ctrlTextJustType);
	
	DDX_Control(pDX, IDC_BTN_FONT, m_btnFont);
}

BEGIN_MESSAGE_MAP(TplParamPage, CResizableDialog)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_FONT, OnFont)
END_MESSAGE_MAP()

BOOL TplParamPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_INFO, TOP_LEFT);
	AddAnchor(IDC_EDIT_MSG, TOP_LEFT, TOP_RIGHT);

	// paper size.
	AddAnchor(IDC_GROUP_PAPER_SIZE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_TEXT_WIDTH, TOP_LEFT);
	AddAnchor(IDC_EDIT_WIDTH, TOP_LEFT);
	AddAnchor(IDC_TEXT_HEIGHT, TOP_LEFT);
	AddAnchor(IDC_EDIT_HEIGHT, TOP_LEFT);

	// paper margin
	AddAnchor(IDC_GROUP_PAPER_MARGIN, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_TEXT_LEFT_MARGIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_LEFT_MARGIN, TOP_LEFT);
	AddAnchor(IDC_TEXT_RIGHT_MARGIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_RIGHT_MARGIN, TOP_LEFT);
	AddAnchor(IDC_TEXT_TOP_MARGIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_TOP_MARGIN, TOP_LEFT);
	AddAnchor(IDC_TEXT_BOTTOM_MARGIN, TOP_LEFT);
	AddAnchor(IDC_EDIT_BOTTOM_MARGIN, TOP_LEFT);

	// text style
	AddAnchor(IDC_GROUP_TEXT_STYLE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_TEXT_JUST_TYPE, TOP_LEFT);
	AddAnchor(IDC_COMBO_TEXT_JUST_TYPE, TOP_LEFT);
	AddAnchor(IDC_BTN_FONT, TOP_LEFT);
	/************************************************************************/

	/************************************************************************/
	// init the data.

	// init the combo box.
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_JUST_LEFT);
	m_ctrlTextJustType.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_JUST_CENTER);
	m_ctrlTextJustType.AddString(str);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_JUST_RIGHT);
	m_ctrlTextJustType.AddString(str);
	/************************************************************************/

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_SIZE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_MARGIN )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_TEXT_STYLE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_TEXT_JUST_TYPE )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH TplParamPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void TplParamPage::OnFont()
{
	LOGFONT	logicFont = m_logicFont;
	logicFont.lfHeight = UnitHelper::PointsToPixels(logicFont.lfHeight);

	CFontDialog	dlg(&logicFont);
	dlg.m_cf.rgbColors = m_fontColor;
	dlg.m_cf.Flags |= CF_EFFECTS;
	if( dlg.DoModal() == IDOK )
	{
		dlg.GetCurrentFont(&m_logicFont);
		m_logicFont.lfHeight = dlg.GetSize();
		m_fontColor = dlg.GetColor();
	}
}

void TplParamPage::DisplayTemplateParam(TemplateParamPtr pTemplateParam, TPL_UNIT_TYPE unitType)
{
	m_pTemplateParam = pTemplateParam;
	m_unitType = unitType;

	// the font info.
	FontParamPtr pFontParam = m_pTemplateParam->GetFontParam();
	m_logicFont = pFontParam->GetLogicFont();
	m_fontColor = pFontParam->GetFontColor();

	// paper size and paper margin.
	if (m_unitType == TPL_UNIT_PIXEL)
	{
		m_dPaperWidth.Init(m_pTemplateParam->GetPaperWidth(), 0, FALSE);
		m_dPaperHeight.Init(m_pTemplateParam->GetPaperHeight(), 0, FALSE);

		m_dLeftMargin.Init(m_pTemplateParam->GetLeftMargin(), 0, FALSE);
		m_dRightMargin.Init(m_pTemplateParam->GetRightMargin(), 0, FALSE);
		m_dTopMargin.Init(m_pTemplateParam->GetTopMargin(), 0, FALSE);
		m_dBottomMargin.Init(m_pTemplateParam->GetBottomMargin(), 0, FALSE);
	}
	else if (m_unitType == TPL_UNIT_INCH)
	{
		m_dPaperWidth.Init(UnitHelper::PixelsToInches(m_pTemplateParam->GetPaperWidth()), TPL_DIGITAL_NUM, FALSE);
		m_dPaperHeight.Init(UnitHelper::PixelsToInches(m_pTemplateParam->GetPaperHeight()), TPL_DIGITAL_NUM, FALSE);

		m_dLeftMargin.Init(UnitHelper::PixelsToInches(m_pTemplateParam->GetLeftMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dRightMargin.Init(UnitHelper::PixelsToInches(m_pTemplateParam->GetRightMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dTopMargin.Init(UnitHelper::PixelsToInches(m_pTemplateParam->GetTopMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dBottomMargin.Init(UnitHelper::PixelsToInches(m_pTemplateParam->GetBottomMargin()), TPL_DIGITAL_NUM, FALSE);
	}
	else if (m_unitType == TPL_UNIT_CM)
	{
		m_dPaperWidth.Init(UnitHelper::PixelsToCentimeters(m_pTemplateParam->GetPaperWidth()), TPL_DIGITAL_NUM, FALSE);
		m_dPaperHeight.Init(UnitHelper::PixelsToCentimeters(m_pTemplateParam->GetPaperHeight()), TPL_DIGITAL_NUM, FALSE);

		m_dLeftMargin.Init(UnitHelper::PixelsToCentimeters(m_pTemplateParam->GetLeftMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dRightMargin.Init(UnitHelper::PixelsToCentimeters(m_pTemplateParam->GetRightMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dTopMargin.Init(UnitHelper::PixelsToCentimeters(m_pTemplateParam->GetTopMargin()), TPL_DIGITAL_NUM, FALSE);
		m_dBottomMargin.Init(UnitHelper::PixelsToCentimeters(m_pTemplateParam->GetBottomMargin()), TPL_DIGITAL_NUM, FALSE);
	}

	// justification type.
	if (m_pTemplateParam->GetTextJustType() == TEXT_JUST_LEFT)
		m_ctrlTextJustType.SetCurSel(0);
	else if (m_pTemplateParam->GetTextJustType() == TEXT_JUST_CENTER)
		m_ctrlTextJustType.SetCurSel(1);
	else if (m_pTemplateParam->GetTextJustType() == TEXT_JUST_RIGHT)
		m_ctrlTextJustType.SetCurSel(2);

	UpdateData(FALSE);
}

TemplateParamPtr TplParamPage::GetTemplateParam()
{
	UpdateData(TRUE);

	/************************************************************************/
	// paper size and paper margin.

	// get the data from the UI.
	int iPaperWidth, iPaperHeight, iLeftMargin, iRightMargin, iTopMargin, iBottomMargin;
	if (m_unitType == TPL_UNIT_PIXEL)
	{
		iPaperWidth = (int)m_dPaperWidth.Get_Number();
		iPaperHeight = (int)m_dPaperHeight.Get_Number();

		iLeftMargin = (int)m_dLeftMargin.Get_Number();
		iRightMargin = (int)m_dRightMargin.Get_Number();
		iTopMargin = (int)m_dTopMargin.Get_Number();
		iBottomMargin = (int)m_dBottomMargin.Get_Number();
	}
	else if (m_unitType == TPL_UNIT_INCH)
	{
		iPaperWidth = UnitHelper::InchesToPixels(m_dPaperWidth.Get_Number());
		iPaperHeight = UnitHelper::InchesToPixels(m_dPaperHeight.Get_Number());

		iLeftMargin = UnitHelper::InchesToPixels(m_dLeftMargin.Get_Number());
		iRightMargin = UnitHelper::InchesToPixels(m_dRightMargin.Get_Number());
		iTopMargin = UnitHelper::InchesToPixels(m_dTopMargin.Get_Number());
		iBottomMargin = UnitHelper::InchesToPixels(m_dBottomMargin.Get_Number());
	}
	else if (m_unitType == TPL_UNIT_CM)
	{
		iPaperWidth = UnitHelper::CentimeterToPixels(m_dPaperWidth.Get_Number());
		iPaperHeight = UnitHelper::CentimeterToPixels(m_dPaperHeight.Get_Number());

		iLeftMargin = UnitHelper::CentimeterToPixels(m_dLeftMargin.Get_Number());
		iRightMargin = UnitHelper::CentimeterToPixels(m_dRightMargin.Get_Number());
		iTopMargin = UnitHelper::CentimeterToPixels(m_dTopMargin.Get_Number());
		iBottomMargin = UnitHelper::CentimeterToPixels(m_dBottomMargin.Get_Number());
	}

	// update "m_pTemplateParam".
	if (iPaperWidth != m_pTemplateParam->GetPaperWidth())
	{
		m_pTemplateParam->SetPaperWidth(iPaperWidth);
		m_pTemplateParam->SetModified(TRUE);
	}
	if (iPaperHeight != m_pTemplateParam->GetPaperHeight())
	{
		m_pTemplateParam->SetPaperHeight(iPaperHeight);
		m_pTemplateParam->SetModified(TRUE);
	}
	if (iLeftMargin != m_pTemplateParam->GetLeftMargin())
	{
		m_pTemplateParam->SetLeftMargin(iLeftMargin);
		m_pTemplateParam->SetModified(TRUE);
	}
	if (iRightMargin != m_pTemplateParam->GetRightMargin())
	{
		m_pTemplateParam->SetRightMargin(iRightMargin);
		m_pTemplateParam->SetModified(TRUE);
	}
	if (iTopMargin != m_pTemplateParam->GetTopMargin())
	{
		m_pTemplateParam->SetTopMargin(iTopMargin);
		m_pTemplateParam->SetModified(TRUE);
	}
	if (iBottomMargin != m_pTemplateParam->GetBottomMargin())
	{
		m_pTemplateParam->SetBottomMargin(iBottomMargin);
		m_pTemplateParam->SetModified(TRUE);
	}
	/************************************************************************/

	// the text justification style.
	if (m_ctrlTextJustType.GetCurSel() != (int)m_pTemplateParam->GetTextJustType())
	{
		m_pTemplateParam->SetTextJustType((TEXT_JUST_TYPE)m_ctrlTextJustType.GetCurSel());
		m_pTemplateParam->SetModified(TRUE);
	}

	// 字体设置。
	FontParamPtr pFontParam = m_pTemplateParam->GetFontParam();
	if (!pFontParam->FitLogicFont(m_logicFont, m_fontColor))
	{
		pFontParam->UpdateFromLogicFont(m_logicFont, m_fontColor);
		m_pTemplateParam->SetModified(TRUE);
	}

	return m_pTemplateParam;
}

END_CUTLEADER_NAMESPACE()
