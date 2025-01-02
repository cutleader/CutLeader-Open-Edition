#include "StdAfx.h"
#include "TplPageDlg.h"

#include "DateHelper.h"
#include "baseConst.h"
#include "UnitHelper.h"
#include "clBaseDataResource.h"

#include "clReportResource.h"
#include "reportConst.h"
#include "ReportTplPageList.h"
#include "TplPageLoader.h"
#include "TplPageWriter.h"
#include "TplPageTracker.h"

#include "TplDisplayParam.h"
#include "TplDesignConfig.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(TplPageDlg, CDialogEx)

TplPageDlg::TplPageDlg(void) : CLDialog(TplPageDlg::IDD)
{
}

TplPageDlg::~TplPageDlg(void)
{
}

void TplPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TPL_PAGE_TYPE, m_ctrlTplPageType);
	DDX_Control(pDX, IDC_LIST_TPL_PAGE, m_ctrlTplPageList);

	// basic info.
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTplPageName);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_strTplPageType);
	DDX_Text(pDX, IDC_EDIT_COM, m_strComment);

	// paper size.
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_dPaperWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_dPaperHeight);

	// paper margin
	DDX_Control(pDX, IDC_EDIT_LEFT_MARGIN, m_dLeftMargin);
	DDX_Control(pDX, IDC_EDIT_RIGHT_MARGIN, m_dRightMargin);
	DDX_Control(pDX, IDC_EDIT_TOP_MARGIN, m_dTopMargin);
	DDX_Control(pDX, IDC_EDIT_BOTTOM_MARGIN, m_dBottomMargin);

	DDX_Control(pDX, IDC_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}

BOOL TplPageDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	// load all report template page.
	m_pReportTplPageList = TplPageLoader::LoadAllReportTplPage(FALSE);

	// get the diaply unit.
	TplDisplayParamPtr pDisplayParam = TplDesignConfig::GetInstance()->GetTplDisplayParam();
	m_unitType = pDisplayParam->GetTplUnit();

	/************************************************************************/
	// init the combo box.

	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_TPL_PAGE_ALLTYPE);
	m_ctrlTplPageType.AddString(str);
	m_ctrlTplPageType.SetItemData(0, -1);

	vector<REPORT_TPL_PAGE_TYPE> tplPageTypeList = ReportTplPage::GetAllPageType();
	for (unsigned int i = 0; i < tplPageTypeList.size(); i++)
	{
		REPORT_TPL_PAGE_TYPE tplPageType = tplPageTypeList.at(i);

		CString strTplPageTypeName = ReportTplPage::GetTplPageTypeName(tplPageType);
		m_ctrlTplPageType.AddString(strTplPageTypeName);
		m_ctrlTplPageType.SetItemData(i+1, (int)tplPageType);
	}

	m_ctrlTplPageType.SetCurSel(0);
	/************************************************************************/

	// init the template page list box.
	InitListBox(m_pReportTplPageList);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_TEMPLATE_PAGES )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_TPL_PAGE_TYPE )->GetSafeHwnd(), L"", L"" );
	
	SetWindowTheme( GetDlgItem( IDC_STATIC_BASIC_INFO )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_PAPER_SIZE )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_STATIC_PAPER_MARGIN )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(TplPageDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_CBN_SELCHANGE(IDC_COMBO_TPL_PAGE_TYPE, OnSelchangePageType)
	ON_LBN_SELCHANGE(IDC_LIST_TPL_PAGE, OnSelchangePage)

	ON_BN_CLICKED(IDC_BTN_DEL, OnDelTplPage)

	ON_BN_CLICKED(IDC_BTN_APPLY, OnApply)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnCancel)
END_MESSAGE_MAP()

HBRUSH TplPageDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void TplPageDlg::OnSelchangePageType()
{
	ReportTplPageListPtr pReportTplPageList;
	int iIndex = m_ctrlTplPageType.GetCurSel();
	if (iIndex == 0)
		pReportTplPageList = m_pReportTplPageList;
	else
	{
		REPORT_TPL_PAGE_TYPE tplPageType = (REPORT_TPL_PAGE_TYPE)m_ctrlTplPageType.GetItemData(m_ctrlTplPageType.GetCurSel());
		pReportTplPageList = m_pReportTplPageList->GetPageByType(tplPageType);
	}

	InitListBox(pReportTplPageList);
}

void TplPageDlg::OnSelchangePage()
{
	// init the template page info.
	InitTplPageInfo();
}

void TplPageDlg::OnDelTplPage()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	int iIndex = m_ctrlTplPageList.GetCurSel();
	if (iIndex == -1)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SELECT_TEMPLATE_PAGE);
		MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_TEMPLATE_PAGE);
		if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			// delete the selected template page and save the change.
			LONGLONG* pTplPageID = (LONGLONG*)m_ctrlTplPageList.GetItemData(iIndex);
			m_pReportTplPageList->DeleteReportTplPage(*pTplPageID);
			TplPageWriter::DeleteTplPage(*pTplPageID);

			// update the UI.
			m_ctrlTplPageList.DeleteString(iIndex);
			if (m_ctrlTplPageList.GetCount() > 0)
				m_ctrlTplPageList.SetCurSel(m_ctrlTplPageList.GetCount() - 1);
			InitTplPageInfo();
		}
	}
}

void TplPageDlg::OnApply()
{
	// check whether the current template page is modified.
	if (m_pReportTplPage && IsTplPageModified())
	{
		m_pReportTplPage->SetModified(TRUE);

		// save the change to database.
		SaveChange();

		// update the list box.
		int iIndex = m_ctrlTplPageList.GetCurSel();
		if (iIndex != -1)
		{
			m_ctrlTplPageList.DeleteString(iIndex);
			m_ctrlTplPageList.InsertString(iIndex, m_pReportTplPage->GetName());
			m_ctrlTplPageList.SetItemData(iIndex, (DWORD_PTR)(new LONGLONG(m_pReportTplPage->GetID())));
			m_ctrlTplPageList.SetCurSel(iIndex);
		}
	}
}

void TplPageDlg::OnOpen()
{
	if (m_pReportTplPage)
	{
		if (!TplPageTracker::GetInstance()->IsItemOpened(m_pReportTplPage->GetID()))
		{
			// mark it is opened.
			TplPageTracker::GetInstance()->AddItem(m_pReportTplPage);
			EndDialog(IDC_BTN_OPEN);
		}
		else
		{
			CString strProductName, strMsgContent;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			strMsgContent = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TEMPLATE_PAGE_OPENED);
			strMsgContent.Replace(_T("WILLBEREPLACED1"), m_pReportTplPage->GetName());
			MessageBox(strMsgContent, strProductName, MB_OK | MB_ICONINFORMATION);
		}		
	}
}

void TplPageDlg::OnCancel()
{
	EndDialog(IDC_BTN_CANCEL);
}

void TplPageDlg::InitListBox(ReportTplPageListPtr pReportTplPageList)
{
	m_ctrlTplPageList.ResetContent();

	for (unsigned int i = 0; i < pReportTplPageList->size(); i++)
	{
		ReportTplPagePtr m_pReportTplPage = pReportTplPageList->at(i);
		m_ctrlTplPageList.InsertString(i, m_pReportTplPage->GetName());
		m_ctrlTplPageList.SetItemData(i, (DWORD_PTR)(new LONGLONG(m_pReportTplPage->GetID())));
	}

	// select the first item.
	if (pReportTplPageList->size() > 0)
		m_ctrlTplPageList.SetCurSel(0);

	// init the template page info.
	InitTplPageInfo();
}

void TplPageDlg::InitTplPageInfo()
{
	// check whether the current template page is modified.
	if (m_pReportTplPage && IsTplPageModified())
	{
		m_pReportTplPage->SetModified(TRUE);

		CString strSoluName, str;
		strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TEMPLATE_PAGE);
		if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			// save the change to database.
			SaveChange();
		}
	}

	int iIndex = m_ctrlTplPageList.GetCurSel();
	if (iIndex == -1)
	{
		m_pReportTplPage.reset();

		/************************************************************************/
		// clear all data.

		m_strTplPageName = _T("");
		m_strTplPageType = _T("");
		m_strComment = _T("");

		m_dPaperWidth.Init(0, 0, FALSE);
		m_dPaperHeight.Init(0, 0, FALSE);

		m_dLeftMargin.Init(0, 0, FALSE);
		m_dRightMargin.Init(0, 0, FALSE);
		m_dTopMargin.Init(0, 0, FALSE);
		m_dBottomMargin.Init(0, 0, FALSE);
		/************************************************************************/

		/************************************************************************/
		// disable all controls.

		((CWnd*)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_TYPE))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_COM))->EnableWindow(FALSE);

		((CWnd*)GetDlgItem(IDC_EDIT_WIDTH))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_HEIGHT))->EnableWindow(FALSE);

		((CWnd*)GetDlgItem(IDC_EDIT_LEFT_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_RIGHT_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_TOP_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_BOTTOM_MARGIN))->EnableWindow(FALSE);
		/************************************************************************/
	}
	else
	{
		/************************************************************************/
		// enable all controls.

		((CWnd*)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_TYPE))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_COM))->EnableWindow(TRUE);

		((CWnd*)GetDlgItem(IDC_EDIT_WIDTH))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_HEIGHT))->EnableWindow(TRUE);

		((CWnd*)GetDlgItem(IDC_EDIT_LEFT_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_RIGHT_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_TOP_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_BOTTOM_MARGIN))->EnableWindow(TRUE);
		/************************************************************************/

		// get the selected template page.
		LONGLONG* pTplPageID = (LONGLONG*)m_ctrlTplPageList.GetItemData(iIndex);
		m_pReportTplPage = m_pReportTplPageList->GetPageByID(*pTplPageID);

		m_strTplPageName = m_pReportTplPage->GetName();
		m_strTplPageType = ReportTplPage::GetTplPageTypeName(m_pReportTplPage->GetTplPageType());

		// paper size and paper margin.
		if (m_unitType == TPL_UNIT_PIXEL)
		{
			m_dPaperWidth.Init(m_pReportTplPage->GetPaperWidth(), 0, FALSE);
			m_dPaperHeight.Init(m_pReportTplPage->GetPaperHeight(), 0, FALSE);

			m_dLeftMargin.Init(m_pReportTplPage->GetLeftMargin(), 0, FALSE);
			m_dRightMargin.Init(m_pReportTplPage->GetRightMargin(), 0, FALSE);
			m_dTopMargin.Init(m_pReportTplPage->GetTopMargin(), 0, FALSE);
			m_dBottomMargin.Init(m_pReportTplPage->GetBottomMargin(), 0, FALSE);
		}
		else if (m_unitType == TPL_UNIT_INCH)
		{
			m_dPaperWidth.Init(UnitHelper::PixelsToInches(m_pReportTplPage->GetPaperWidth()), TPL_DIGITAL_NUM, FALSE);
			m_dPaperHeight.Init(UnitHelper::PixelsToInches(m_pReportTplPage->GetPaperHeight()), TPL_DIGITAL_NUM, FALSE);

			m_dLeftMargin.Init(UnitHelper::PixelsToInches(m_pReportTplPage->GetLeftMargin()), TPL_DIGITAL_NUM, FALSE);
			m_dRightMargin.Init(UnitHelper::PixelsToInches(m_pReportTplPage->GetRightMargin()), TPL_DIGITAL_NUM, FALSE);
			m_dTopMargin.Init(UnitHelper::PixelsToInches(m_pReportTplPage->GetTopMargin()), TPL_DIGITAL_NUM, FALSE);
			m_dBottomMargin.Init(UnitHelper::PixelsToInches(m_pReportTplPage->GetBottomMargin()), TPL_DIGITAL_NUM, FALSE);
		}
		else if (m_unitType == TPL_UNIT_CM)
		{
			m_dPaperWidth.Init(UnitHelper::PixelsToCentimeters(m_pReportTplPage->GetPaperWidth()), TPL_DIGITAL_NUM, FALSE);
			m_dPaperHeight.Init(UnitHelper::PixelsToCentimeters(m_pReportTplPage->GetPaperHeight()), TPL_DIGITAL_NUM, FALSE);

			m_dLeftMargin.Init(UnitHelper::PixelsToCentimeters(m_pReportTplPage->GetLeftMargin()), TPL_DIGITAL_NUM, FALSE);
			m_dRightMargin.Init(UnitHelper::PixelsToCentimeters(m_pReportTplPage->GetRightMargin()), TPL_DIGITAL_NUM, FALSE);
			m_dTopMargin.Init(UnitHelper::PixelsToCentimeters(m_pReportTplPage->GetTopMargin()), TPL_DIGITAL_NUM, FALSE);
			m_dBottomMargin.Init(UnitHelper::PixelsToCentimeters(m_pReportTplPage->GetBottomMargin()), TPL_DIGITAL_NUM, FALSE);
		}

		UpdateCtrlStatus();
	}

	UpdateData(FALSE);
}

BOOL TplPageDlg::IsTplPageModified()
{
	BOOL bRet = FALSE;

	UpdateData(TRUE);

	// the name.
	if (m_strTplPageName != m_pReportTplPage->GetName())
		bRet = TRUE;

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

	// compare.
	if (iPaperWidth != m_pReportTplPage->GetPaperWidth())
		bRet = TRUE;
	if (iPaperHeight != m_pReportTplPage->GetPaperHeight())
		bRet = TRUE;
	if (iLeftMargin != m_pReportTplPage->GetLeftMargin())
		bRet = TRUE;
	if (iRightMargin != m_pReportTplPage->GetRightMargin())
		bRet = TRUE;
	if (iTopMargin != m_pReportTplPage->GetTopMargin())
		bRet = TRUE;
	if (iBottomMargin != m_pReportTplPage->GetBottomMargin())
		bRet = TRUE;
	/************************************************************************/

	return bRet;
}

void TplPageDlg::SaveChange()
{
	// basic info.
	m_pReportTplPage->SetName(m_strTplPageName);

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

	m_pReportTplPage->SetPaperWidth(iPaperWidth);
	m_pReportTplPage->SetPaperHeight(iPaperHeight);
	m_pReportTplPage->SetLeftMargin(iLeftMargin);
	m_pReportTplPage->SetRightMargin(iRightMargin);
	m_pReportTplPage->SetTopMargin(iTopMargin);
	m_pReportTplPage->SetBottomMargin(iBottomMargin);
	/************************************************************************/

	// update database.
	TplPageWriter::UpdateBasicData(m_pReportTplPage);

	// message.
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TEMPLATE_PAGE_OK);
	MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
}

void TplPageDlg::UpdateCtrlStatus()
{
	// check whether the template page is opened.
	if (TplPageTracker::GetInstance()->IsItemOpened(m_pReportTplPage->GetID()))
	{
		((CWnd*)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_COM))->EnableWindow(FALSE);

		((CWnd*)GetDlgItem(IDC_EDIT_WIDTH))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_HEIGHT))->EnableWindow(FALSE);

		((CWnd*)GetDlgItem(IDC_EDIT_TOP_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_BOTTOM_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_LEFT_MARGIN))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_RIGHT_MARGIN))->EnableWindow(FALSE);
	}
	else
	{
		((CWnd*)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_COM))->EnableWindow(TRUE);

		((CWnd*)GetDlgItem(IDC_EDIT_WIDTH))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_HEIGHT))->EnableWindow(TRUE);

		((CWnd*)GetDlgItem(IDC_EDIT_TOP_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_BOTTOM_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_LEFT_MARGIN))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_RIGHT_MARGIN))->EnableWindow(TRUE);
	}
}

END_CUTLEADER_NAMESPACE()
