#include "stdafx.h"
#include "StaticEntityPropDlg.h"

#include "baseConst.h"
#include "UnitHelper.h"

#include "FontParam.h"
#include "StaticTplEntity.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(StaticEntityPropDlg, CDialogEx)

StaticEntityPropDlg::StaticEntityPropDlg()
					: CLDialog(StaticEntityPropDlg::IDD)
{
}

StaticEntityPropDlg::~StaticEntityPropDlg(void)
{
}

void StaticEntityPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_LABEL_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_LABEL_TEXT, m_strContent);
	DDX_Radio(pDX, IDC_RADIO_LEFT, m_iJustification);

	DDX_Control(pDX, IDC_BTN_FONT, m_btnFont);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL StaticEntityPropDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	/************************************************************************/
	// init the dialog content.

	m_strName = m_pStaticTplEntity->GetName();
	m_strContent = m_pStaticTplEntity->GetTitle();

	// text justification type.
	if (m_pStaticTplEntity->GetTextJustType() == TEXT_JUST_LEFT)
		m_iJustification = 0;
	else if (m_pStaticTplEntity->GetTextJustType() == TEXT_JUST_CENTER)
		m_iJustification = 1;
	else if (m_pStaticTplEntity->GetTextJustType() == TEXT_JUST_RIGHT)
		m_iJustification = 2;

	// the font info.
	FontParamPtr pFontParam = m_pStaticTplEntity->GetFontParam();
	m_logicFont = pFontParam->GetLogicFont();
	m_fontColor = pFontParam->GetFontColor();
	/************************************************************************/

	UpdateData(FALSE);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_JUSTIFICATION )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_LEFT )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_CENTER )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_RADIO_RIGHT )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

BEGIN_MESSAGE_MAP(StaticEntityPropDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BTN_FONT, OnFont)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH StaticEntityPropDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void StaticEntityPropDlg::OnFont() 
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

void StaticEntityPropDlg::OnOK() 
{
	UpdateData(TRUE);

	// check name.
	if (m_strName != m_pStaticTplEntity->GetName())
	{
		m_pStaticTplEntity->SetName(m_strName);
		m_pStaticTplEntity->SetModified(TRUE);
	}

	// check title.
	if (m_strContent != m_pStaticTplEntity->GetTitle())
	{
		m_pStaticTplEntity->SetTitle(m_strContent);
		m_pStaticTplEntity->SetModified(TRUE);
	}

	// check text justification style.
	if (m_iJustification != m_pStaticTplEntity->GetTextJustType())
	{
		m_pStaticTplEntity->SetTextJustType((TEXT_JUST_TYPE)m_iJustification);
		m_pStaticTplEntity->SetModified(TRUE);
	}

	// check font param.
	FontParamPtr pFontParam = m_pStaticTplEntity->GetFontParam();
	if (!pFontParam->FitLogicFont(m_logicFont, m_fontColor))
	{
		pFontParam->UpdateFromLogicFont(m_logicFont, m_fontColor);
		m_pStaticTplEntity->SetModified(TRUE);
	}

	CDialogEx::OnOK();
}

void StaticEntityPropDlg::OnCancel() 
{
	CDialogEx::OnCancel();
}

END_CUTLEADER_NAMESPACE()
