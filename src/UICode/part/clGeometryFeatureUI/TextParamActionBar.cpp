#include "StdAfx.h"
#include "TextParamActionBar.h"

#include "ClWindowFinder.h"
#include "LogMgr.h"
#include "clUILibResource.h"
#include "TextStruct.h"
#include "Text2ContourHelper.h"
#include "PathHelper.h"

#include "PartEditParam.h"
#include "CLOptionInfo.h"

using namespace std;


/*************************************************************************************************/
// 这里的代码可以得到系统中所有字体，这些字体只需读取一次，所以比CMFCFontComboBox读取字体效率高。
std::vector<CString> g_allFontNames;
INT CALLBACK NEnumFontNameProc(LOGFONT* plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam/**/)
{
	//同步调用的回调
	vector<CString>* pAllFontNames = (vector<CString>*)lParam;
	if (pAllFontNames != NULL)
	{
		CString strVal = plf->lfFaceName;
		if (strVal.Left(1) != L"@") {
			pAllFontNames->push_back(strVal);
		}
	}
	return TRUE; //EnumFontFamilies 返回值由此回调返回决定
}
void GetAllFontNames(HWND& hwnd)
{
	HDC hdc = ::GetDC(hwnd);
	// int nRet = ::EnumFontFamilies(hdc, (LPTSTR)NULL, (FONTENUMPROC)NEnumFontNameProc, (LPARAM) & (g_vSysFonts));
	int nRet = ::EnumFonts(hdc, (LPTSTR)NULL, (FONTENUMPROC)NEnumFontNameProc, (LPARAM)&g_allFontNames);
	::ReleaseDC(hwnd, hdc);
}
/*************************************************************************************************/


BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TextParamActionBar, ActionBar)

TextParamActionBar::TextParamActionBar(CWnd* pParent /*=NULL*/)
	: ActionBar(TextParamActionBar::IDD, pParent)
{
	CString strFontName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontName();
	CString strFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();
	m_pTextParam.reset(new TextParam(_T("TAOSoft"), strFontName, strFontFileName, 50.0, 50.0, .0));

	m_bExistActionBarBetyondMe = false;
}

TextParamActionBar::~TextParamActionBar(void)
{
}

void TextParamActionBar::DoDataExchange(CDataExchange* pDX)
{
	ActionBar::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_TextContent, m_ctrlTextContent);
	DDX_Control(pDX, IDC_COMBO_Font, m_ctrlFontComboBox);
	DDX_Control(pDX, IDC_EDIT_TextHeight, m_ctrlTextHeight);
	DDX_Control(pDX, IDC_EDIT_TextWidth, m_ctrlTextWidth);
	DDX_Control(pDX, IDC_EDIT_TextTiltAngle, m_ctrlTextTiltAngle);
}

BOOL TextParamActionBar::OnInitDialog()
{
	__super::OnInitDialog();

	// 初始化字体下拉框。
	{
		if (g_allFontNames.size() == 0) {
			GetAllFontNames(this->m_hWnd);
		}
		m_ctrlFontComboBox.SetRedraw(FALSE);
		for (unsigned int i = 0; i < g_allFontNames.size(); i++) {
			m_ctrlFontComboBox.AddString(g_allFontNames[i]);
		}
		m_ctrlFontComboBox.SetRedraw(TRUE);
		m_ctrlFontComboBox.Invalidate();
	}

	m_ctrlTextContent.SetWindowTextW(m_pTextParam->GetTextContent());
	m_ctrlFontComboBox.SetWindowTextW(m_pTextParam->GetFontName()); // 把字体名称显示在字体下拉框中。
	m_ctrlTextHeight.Init_WithoutMsg(m_pTextParam->GetTextHeight(), DIGITAL_NUM, false, .0);
	m_ctrlTextWidth.Init_WithoutMsg(m_pTextParam->GetTextWidth(), DIGITAL_NUM, false, .0);
	m_ctrlTextTiltAngle.Init_WithoutMsg(m_pTextParam->GetTextTiltAngle(), DIGITAL_NUM, true, .0, -DBL_MAX/2.0, DBL_MAX/2.0);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	SetWindowTheme( GetDlgItem( IDC_STATIC_OPTION )->GetSafeHwnd(), L"", L"" );
	return TRUE;
}

BEGIN_MESSAGE_MAP(TextParamActionBar, ActionBar)
	ON_WM_PAINT()
	ON_MESSAGE(WM_OnCharEventFinished, OnCharEventOfEditBoxFinished)
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnSelchangeFontName)
END_MESSAGE_MAP()

void TextParamActionBar::OnPaint()
{
	if (this->IsWindowVisible()) {
		CRect parentRect, winRect;
		ClWindowFinder::GetCurrentView_1()->GetWindowRect(parentRect);
		this->GetWindowRect(winRect);
		if (!m_bExistActionBarBetyondMe) {
			this->MoveWindow(parentRect.Width() - winRect.Width() - 2, 2, winRect.Width(), winRect.Height());
		}
		else {
			this->MoveWindow(parentRect.Width() - winRect.Width() - 2, m_existActionBarRect.Height(), winRect.Width(), winRect.Height());
		}
	}

	CLDialog::OnPaint();
}

LRESULT TextParamActionBar::OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = ::GetFocus();
	int iCtrlID = ::GetDlgCtrlID(hwnd);
	if (iCtrlID == IDC_EDIT_TextContent)
	{
		CString strTextContent;
		m_ctrlTextContent.GetWindowTextW(strTextContent);
		if (strTextContent != m_pTextParam->GetTextContent())
		{
			m_pTextParam->SetTextContent(strTextContent);
			NotifyAllObserver(ID_EVENT_TextParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_TextHeight)
	{
		m_ctrlTextHeight.UpdateNumEdit_fromUI();
		double dTextHeight = m_ctrlTextHeight.Get_Number();
		if (fabs(dTextHeight - m_pTextParam->GetTextHeight()) > GEOM_TOLERANCE)
		{
			m_pTextParam->SetTextHeight(dTextHeight);
			NotifyAllObserver(ID_EVENT_TextParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_TextWidth)
	{
		m_ctrlTextWidth.UpdateNumEdit_fromUI();
		double dTextWidth = m_ctrlTextWidth.Get_Number();
		if (fabs(dTextWidth - m_pTextParam->GetTextWidth()) > GEOM_TOLERANCE)
		{
			m_pTextParam->SetTextWidth(dTextWidth);
			NotifyAllObserver(ID_EVENT_TextParamChanged, CComVariant());
		}
	}
	else if (iCtrlID == IDC_EDIT_TextTiltAngle)
	{
		m_ctrlTextTiltAngle.UpdateNumEdit_fromUI();
		double dTextTiltAngle = m_ctrlTextTiltAngle.Get_Number();
		if (fabs(dTextTiltAngle - m_pTextParam->GetTextTiltAngle()) > GEOM_TOLERANCE)
		{
			m_pTextParam->SetTextTiltAngle(dTextTiltAngle);
			NotifyAllObserver(ID_EVENT_TextParamChanged, CComVariant());
		}
	}

	return true;
}

void TextParamActionBar::OnSelchangeFontName()
{
	CString strFontName;
	m_ctrlFontComboBox.GetLBText(m_ctrlFontComboBox.GetCurSel(), strFontName);
	if (strFontName != m_pTextParam->GetFontName())
	{
		m_pTextParam->SetFontName(strFontName);
		CString strFontFilePath = Text2ContourHelper::GetFontFilePathByFontName(strFontName);
		CString strFontFileName = PathHelper::GetFileNameExt(strFontFilePath);
		m_pTextParam->SetFontFileName(strFontFileName);
		NotifyAllObserver(ID_EVENT_TextParamChanged, CComVariant());
	}
}

void TextParamActionBar::InitTextParam(TextParamPtr pTextParam)
{
	m_pTextParam = pTextParam;

	m_ctrlTextContent.SetWindowTextW(m_pTextParam->GetTextContent());
	m_ctrlFontComboBox.SetWindowTextW(m_pTextParam->GetFontName()); // 把字体名称显示在字体下拉框中。
	m_ctrlTextHeight.Init(m_pTextParam->GetTextHeight(), DIGITAL_NUM, false, .0);
	m_ctrlTextWidth.Init(m_pTextParam->GetTextWidth(), DIGITAL_NUM, false, .0);
	m_ctrlTextTiltAngle.Init(m_pTextParam->GetTextTiltAngle(), DIGITAL_NUM, true, .0, - DBL_MAX / 2.0, DBL_MAX / 2.0);
}

void TextParamActionBar::Show_TextParamActionBar(bool bExistActionBarBetyondMe, const CRect& existActionBarRect)
{
	m_bExistActionBarBetyondMe = bExistActionBarBetyondMe;
	m_existActionBarRect = existActionBarRect;

	// 调整位置。
	CRect parentRect, winRect;
	ClWindowFinder::GetCurrentView_1()->GetWindowRect(parentRect);
	this->GetWindowRect(winRect);
	if (!m_bExistActionBarBetyondMe) {
		this->MoveWindow(parentRect.Width() - winRect.Width() - 2, 2, winRect.Width(), winRect.Height());
	}
	else{
		this->MoveWindow(parentRect.Width() - winRect.Width() - 2, m_existActionBarRect.Height(), winRect.Width(), winRect.Height());
	}

	if (!this->IsWindowVisible()) {
		this->SetParent(ClWindowFinder::GetCurrentView_1());
		this->ShowWindow(SW_SHOW);
	}
}

END_CUTLEADER_NAMESPACE()
