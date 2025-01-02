#include "StdAfx.h"
#include "TextDefaultFontDlg.h"

#include "PathHelper.h"
#include "Text2ContourHelper.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TextDefaultFontDlg, CDialogEx)

TextDefaultFontDlg::TextDefaultFontDlg()
: CLDialog(TextDefaultFontDlg::IDD)
{
}

TextDefaultFontDlg::~TextDefaultFontDlg(void)
{
}

void TextDefaultFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_Font, m_ctrlFontComboBox);
	DDX_Control( pDX, IDOK, m_btnOk );
	DDX_Control( pDX, IDCANCEL, m_btnCancel );
}

BOOL TextDefaultFontDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (!m_ctrlFontComboBox.SelectFont(m_strFontName))
		m_ctrlFontComboBox.SetWindowTextW(m_strFontName); // 字体下拉框中没有这个字体名称，那么就把这个字体名称显示在字体下拉框中。

	SetBackgroundColor(SkinManager::GetBackgroundColor() );
	return TRUE;
}

BEGIN_MESSAGE_MAP(TextDefaultFontDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnSelchangeFontName)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH TextDefaultFontDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void TextDefaultFontDlg::OnSelchangeFontName()
{
	m_strFontName = m_ctrlFontComboBox.GetSelFont()->m_strName;
	CString strFontFilePath = Text2ContourHelper::GetFontFilePathByFontName(m_strFontName);
	m_strFontFileName = PathHelper::GetFileNameExt(strFontFilePath);
}

void TextDefaultFontDlg::OnBnClickedOk()
{
	OnOK();
}

void TextDefaultFontDlg::Init(const CString& strFontName, const CString& strFontFileName)
{
	m_strFontName = strFontName;
	m_strFontFileName = strFontFileName;
}

END_CUTLEADER_NAMESPACE()
