#include "StdAfx.h"
#include "TplPageSelDlg.h"

#include "baseConst.h"
#include "CGridColumnTraitText.h"
#include "clUtilityResource.h"
#include "baseConst.h"
#include "clBaseDataResource.h"

#include "clReportResource.h"
#include "ReportTplPage.h"
#include "ReportTplPageList.h"
#include "TplPageLoader.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TplPageSelDlg, CDialogEx)

TplPageSelDlg::TplPageSelDlg(void) : CLDialog(TplPageSelDlg::IDD)
{
}

TplPageSelDlg::~TplPageSelDlg(void)
{
}

void TplPageSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TPL_PAGE_TYPE, m_ctrlTplPageType);
	DDX_Control(pDX, IDC_LIST_TPL_PAGE, m_ctrlTplPageList);
	
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BOOL TplPageSelDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	// load all report template page.
	m_pReportTplPageList = TplPageLoader::LoadAllReportTplPage(FALSE);

	/************************************************************************/
	// init the combo box.

	for (unsigned int i = 0; i < m_tplPageTypeList.size(); i++)
	{
		REPORT_TPL_PAGE_TYPE tplPageType = m_tplPageTypeList.at(i);

		CString strTplPageTypeName = ReportTplPage::GetTplPageTypeName(tplPageType);
		m_ctrlTplPageType.AddString(strTplPageTypeName);
		m_ctrlTplPageType.SetItemData(i, (int)tplPageType);
	}

	m_ctrlTplPageType.SetCurSel(0);
	/************************************************************************/

	/************************************************************************/
	// init the style of the grid.

	CString str;

	// Requires one never uses column 0
	m_ctrlTplPageList.InsertHiddenLabelColumn();

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_ctrlTplPageList.InsertColumnTrait(1, str, LVCFMT_LEFT, 110, 0, pNameCol);

	// "template page type" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_TYPE);
	CGridColumnTrait* pSkeleCol = new CGridColumnTraitText;
	m_ctrlTplPageList.InsertColumnTrait(2, str, LVCFMT_LEFT, 120, 0, pSkeleCol);

	// "width"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_WIDTH);
	CGridColumnTrait* pWidthCol = new CGridColumnTraitText;
	m_ctrlTplPageList.InsertColumnTrait(3, str, LVCFMT_LEFT, 80, 0, pWidthCol);

	// "height"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_HEIGHT);
	CGridColumnTrait* pHeightCol = new CGridColumnTraitText;
	m_ctrlTplPageList.InsertColumnTrait(4, str, LVCFMT_LEFT, 80, 0, pHeightCol);

	m_ctrlTplPageList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	/************************************************************************/

	// init the data of the grid.
	REPORT_TPL_PAGE_TYPE tplPageType = (REPORT_TPL_PAGE_TYPE)m_ctrlTplPageType.GetItemData(m_ctrlTplPageType.GetCurSel());
	InitTplPageGrid(tplPageType);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_TEMPLATE_PAGES )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_TPL_PAGE_TYPE )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(TplPageSelDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_CBN_SELCHANGE(IDC_COMBO_TPL_PAGE_TYPE, OnSelchangePageType)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

HBRUSH TplPageSelDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void TplPageSelDlg::OnSelchangePageType()
{
	REPORT_TPL_PAGE_TYPE tplPageType = (REPORT_TPL_PAGE_TYPE)m_ctrlTplPageType.GetItemData(m_ctrlTplPageType.GetCurSel());
	InitTplPageGrid(tplPageType);
}

void TplPageSelDlg::OnOK()
{
	int iIndex = m_ctrlTplPageList.GetSelectionMark();
	if (iIndex == -1)
	{
		CString strSoluName, str;
		strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SELECT_TEMPLATE_PAGE);
		MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
	}
	else
	{
		// get the selected template page
		LONGLONG* pTplPageID = (LONGLONG*)m_ctrlTplPageList.GetItemData(iIndex);
		m_pReportTplPage = m_pReportTplPageList->GetPageByID(*pTplPageID);

		CDialogEx::OnOK();
	}
}

void TplPageSelDlg::InitTplPageGrid(REPORT_TPL_PAGE_TYPE tplPageType)
{
	m_ctrlTplPageList.DeleteAllItems();

	ReportTplPageListPtr pReportTplPageList = m_pReportTplPageList->GetPageByType(tplPageType);
	for (unsigned int i = 0; i < pReportTplPageList->size(); i++)
	{
		ReportTplPagePtr pReportTplPage = pReportTplPageList->at(i);

		// insert a new row.
		m_ctrlTplPageList.InsertItem(i, _T(""));

		// name.
		m_ctrlTplPageList.SetItemText(i, 1, pReportTplPage->GetName());

		// page type
		CString strType = ReportTplPage::GetTplPageTypeName(pReportTplPage->GetTplPageType());
		m_ctrlTplPageList.SetItemText(i, 2, strType);

		// paper width
		CString strWidth;
		strWidth.Format(INT_FORMAT, pReportTplPage->GetPaperWidth());
		m_ctrlTplPageList.SetItemText(i, 3, strWidth);

		// paper height
		CString strHeight;
		strHeight.Format(INT_FORMAT, pReportTplPage->GetPaperHeight());
		m_ctrlTplPageList.SetItemText(i, 4, strHeight);

		m_ctrlTplPageList.SetItemData(i, (DWORD_PTR)(new LONGLONG(pReportTplPage->GetID())));
	}
}

END_CUTLEADER_NAMESPACE()
