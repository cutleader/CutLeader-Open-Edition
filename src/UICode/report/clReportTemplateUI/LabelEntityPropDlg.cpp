#include "stdafx.h"
#include "LabelEntityPropDlg.h"

#include "baseConst.h"
#include "baseConst.h"
#include "clReportResource.h"
#include "ReportTplPage.h"
#include "LabelTplEntity.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(LabelEntityPropDlg, CDialogEx)

LabelEntityPropDlg::LabelEntityPropDlg()
					: CLDialog(LabelEntityPropDlg::IDD)
{
}

LabelEntityPropDlg::~LabelEntityPropDlg(void)
{
}

void LabelEntityPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_ctrlWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_ctrlHeight);

	DDX_Check(pDX, IDC_CHECK_BORDER, m_bBorder);
	DDX_Control(pDX, IDC_EDIT_BORDER_THICKNESS, m_iBorderThickness);
	DDX_Control(pDX, IDC_COMBO_BORDER_STYLE, m_ctrlBorderStyle);
	DDX_Control(pDX, IDC_STATIC_BORDER_COLOR, m_ctrlBorderColor);

	DDX_Control(pDX, IDC_EDIT_ROW_NUM, m_ctrlRowNum);
	DDX_Control(pDX, IDC_EDIT_COLUMN_NUM, m_ctrlColumnNum);
	DDX_Control(pDX, IDC_EDIT_ROW_GAP, m_ctrlRowGap);
	DDX_Control(pDX, IDC_EDIT_COLUMN_GAP, m_ctrlColumnGap);

	DDX_Control(pDX, IDC_EDIT_WIDTH2, m_dPaperWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT3, m_dPaperHeight);
	DDX_Control(pDX, IDC_EDIT_LEFT_MARGIN2, m_dLeftMargin);
	DDX_Control(pDX, IDC_EDIT_RIGHT_MARGIN2, m_dRightMargin);
	DDX_Control(pDX, IDC_EDIT_TOP_MARGIN2, m_dTopMargin);
	DDX_Control(pDX, IDC_EDIT_BOTTOM_MARGIN2, m_dBottomMargin);

	DDX_Control(pDX, IDC_STATIC_PREVIEW_LABEL, m_ctrlLabelPreview);
	
	DDX_Control(pDX, IDC_BTN_BORDER_COLOR, m_btnBorderColor);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL LabelEntityPropDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	// init some style.
	CString text;
	text = MultiLanguageMgr::GetInstance()->TranslateString( IDS_LINE_STYLE_1 );
	m_ctrlBorderStyle.SetItemData( m_ctrlBorderStyle.AddString( text ), PS_SOLID );
	text = MultiLanguageMgr::GetInstance()->TranslateString( IDS_LINE_STYLE_2 );
	m_ctrlBorderStyle.SetItemData( m_ctrlBorderStyle.AddString( text ), PS_DASH );
	text = MultiLanguageMgr::GetInstance()->TranslateString( IDS_LINE_STYLE_3 );
	m_ctrlBorderStyle.SetItemData( m_ctrlBorderStyle.AddString( text ), PS_DOT );

	// 基本信息。
	m_strName = m_pLabelTplEntity->GetName();
	CRect rect = m_pLabelTplEntity->GetBorderRect();
	m_ctrlWidth.Init(rect.Width(), 0, FALSE);
	m_ctrlHeight.Init(rect.Height(), 0, FALSE);

	// about border.
	m_bBorder = m_pLabelTplEntity->IsShowBorder();
	m_iBorderThickness.Init(m_pLabelTplEntity->GetBorderThickness(), 0, FALSE);
	m_ctrlBorderColor.SetColor(m_pLabelTplEntity->GetBorderColor());
	for (int t = 0; t < m_ctrlBorderStyle.GetCount(); t++)
		if (m_ctrlBorderStyle.GetItemData(t) == static_cast<DWORD>(m_pLabelTplEntity->GetBorderStyle()))
			m_ctrlBorderStyle.SetCurSel(t);

	// 布局。
	m_ctrlRowNum.Init(m_pLabelTplEntity->GetRowNum(), 0, FALSE);
	m_ctrlColumnNum.Init(m_pLabelTplEntity->GetColumnNum(), 0, FALSE);
	m_ctrlRowGap.Init(m_pLabelTplEntity->GetRowGap(), 0, FALSE);
	m_ctrlColumnGap.Init(m_pLabelTplEntity->GetColumnGap(), 0, FALSE);

	// 页面信息。
	m_dPaperWidth.Init(m_pReportTplPage->GetPaperWidth(), 0, FALSE);
	m_dPaperHeight.Init(m_pReportTplPage->GetPaperHeight(), 0, FALSE);
	m_dLeftMargin.Init(m_pReportTplPage->GetLeftMargin(), 0, FALSE);
	m_dRightMargin.Init(m_pReportTplPage->GetRightMargin(), 0, FALSE);
	m_dTopMargin.Init(m_pReportTplPage->GetTopMargin(), 0, FALSE);
	m_dBottomMargin.Init(m_pReportTplPage->GetBottomMargin(), 0, FALSE);

	UpdateData(FALSE);
	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_BASIC_INFO )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_CHECK_BORDER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_BORDER_STYLE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_LAYOUT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_SIZE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_PREVIEW )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

BEGIN_MESSAGE_MAP(LabelEntityPropDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, OnKillLabelWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, OnKillLabelHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_ROW_NUM, OnKillRowNum)
	ON_EN_KILLFOCUS(IDC_EDIT_ROW_GAP, OnKillRowGap)
	ON_EN_KILLFOCUS(IDC_EDIT_COLUMN_NUM, OnKillColumnNum)
	ON_EN_KILLFOCUS(IDC_EDIT_COLUMN_GAP, OnKillColumnGap)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH2, OnKillPageWidth)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT3, OnKillPageHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_TOP_MARGIN2, OnKillTopMargin)
	ON_EN_KILLFOCUS(IDC_EDIT_BOTTOM_MARGIN2, OnKillBottomMargin)
	ON_EN_KILLFOCUS(IDC_EDIT_LEFT_MARGIN2, OnKillLeftMargin)
	ON_EN_KILLFOCUS(IDC_EDIT_RIGHT_MARGIN2, OnKillRightMargin)

	ON_BN_CLICKED(IDC_BTN_BORDER_COLOR, OnBorderColor)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH LabelEntityPropDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void LabelEntityPropDlg::OnKillLabelWidth()
{
	CRect rect = m_pLabelTplEntity->GetBorderRect();
	if (rect.Width() != (int)m_ctrlWidth.Get_Number())
	{
		rect.right = rect.left + (int)m_ctrlWidth.Get_Number();
		m_pLabelTplEntity->SetBorderRect(rect);
		m_pLabelTplEntity->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillLabelHeight()
{
	CRect rect = m_pLabelTplEntity->GetBorderRect();
	if (rect.Height() != (int)m_ctrlHeight.Get_Number())
	{
		rect.bottom = rect.top + (int)m_ctrlHeight.Get_Number();
		m_pLabelTplEntity->SetBorderRect(rect);
		m_pLabelTplEntity->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillRowNum()
{
	if (m_pLabelTplEntity->GetRowNum() != (int)m_ctrlRowNum.Get_Number())
	{
		m_pLabelTplEntity->SetRowNum((int)m_ctrlRowNum.Get_Number());
		m_pLabelTplEntity->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillRowGap()
{
	if (m_pLabelTplEntity->GetRowGap() != (int)m_ctrlRowGap.Get_Number())
	{
		m_pLabelTplEntity->SetRowGap((int)m_ctrlRowGap.Get_Number());
		m_pLabelTplEntity->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillColumnNum()
{
	if (m_pLabelTplEntity->GetColumnNum() != (int)m_ctrlColumnNum.Get_Number())
	{
		m_pLabelTplEntity->SetColumnNum((int)m_ctrlColumnNum.Get_Number());
		m_pLabelTplEntity->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillColumnGap()
{
	if (m_pLabelTplEntity->GetColumnGap() != (int)m_ctrlColumnGap.Get_Number())
	{
		m_pLabelTplEntity->SetColumnGap((int)m_ctrlColumnGap.Get_Number());
		m_pLabelTplEntity->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillPageWidth()
{
	if (m_pReportTplPage->GetPaperWidth() != (int)m_dPaperWidth.Get_Number())
	{
		m_pReportTplPage->SetPaperWidth((int)m_dPaperWidth.Get_Number());
		m_pReportTplPage->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillPageHeight()
{
	if (m_pReportTplPage->GetPaperHeight() != (int)m_dPaperHeight.Get_Number())
	{
		m_pReportTplPage->SetPaperHeight((int)m_dPaperHeight.Get_Number());
		m_pReportTplPage->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillTopMargin()
{
	if (m_pReportTplPage->GetTopMargin() != (int)m_dTopMargin.Get_Number())
	{
		m_pReportTplPage->SetTopMargin((int)m_dTopMargin.Get_Number());
		m_pReportTplPage->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillBottomMargin()
{
	if (m_pReportTplPage->GetBottomMargin() != (int)m_dBottomMargin.Get_Number())
	{
		m_pReportTplPage->SetBottomMargin((int)m_dBottomMargin.Get_Number());
		m_pReportTplPage->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillLeftMargin()
{
	if (m_pReportTplPage->GetLeftMargin() != (int)m_dLeftMargin.Get_Number())
	{
		m_pReportTplPage->SetLeftMargin((int)m_dLeftMargin.Get_Number());
		m_pReportTplPage->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnKillRightMargin()
{
	if (m_pReportTplPage->GetRightMargin() != (int)m_dRightMargin.Get_Number())
	{
		m_pReportTplPage->SetRightMargin((int)m_dRightMargin.Get_Number());
		m_pReportTplPage->SetModified(TRUE);
		Invalidate();
	}
}

void LabelEntityPropDlg::OnBorderColor()
{
	CColorDialog dlg(m_pLabelTplEntity->GetBorderColor());
	if( dlg.DoModal() == IDOK )
	{
		m_ctrlBorderColor.SetColor( dlg.GetColor() );
	}
}

void LabelEntityPropDlg::OnOK() 
{
	UpdateData(TRUE);

	// 基本信息。
	if (m_pLabelTplEntity->GetName() != m_strName)
	{
		m_pLabelTplEntity->SetName(m_strName);
		m_pLabelTplEntity->SetModified(TRUE);
	}

	// for border.
	m_pLabelTplEntity->IsShowBorder(m_bBorder);
	m_pLabelTplEntity->SetBorderThickness((int)m_iBorderThickness.Get_Number());
	LINE_STYLE_TYPE borderLineStyle = (LINE_STYLE_TYPE)m_ctrlBorderStyle.GetItemData( m_ctrlBorderStyle.GetCurSel() );
	m_pLabelTplEntity->SetBorderStyle(borderLineStyle);
	m_pLabelTplEntity->SetBorderColor(m_ctrlBorderColor.GetColor());

	CDialogEx::OnOK();
}

void LabelEntityPropDlg::OnCancel() 
{
	CDialogEx::OnCancel();
}

void LabelEntityPropDlg::Init(ReportTplPagePtr pReportTplPage, LabelTplEntityPtr pLabelTplEntity)
{
	m_pReportTplPage = pReportTplPage;
	m_pLabelTplEntity = pLabelTplEntity;
	m_ctrlLabelPreview.Init(pReportTplPage, pLabelTplEntity);
}

END_CUTLEADER_NAMESPACE()
