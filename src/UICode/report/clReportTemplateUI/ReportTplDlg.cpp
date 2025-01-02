#include "StdAfx.h"
#include "ReportTplDlg.h"

#include "baseConst.h"
#include "CGridColumnTraitText.h"
#include "DateHelper.h"
#include "baseConst.h"
#include "DateHelper.h"
#include "clUtilityResource.h"
#include "clBaseDataResource.h"

#include "clReportResource.h"
#include "reportConst.h"
#include "ReportTplPage.h"
#include "ReportTplPageList.h"
#include "ReportTpl.h"
#include "ReportTplList.h"
#include "ReportTplLoader.h"
#include "ReportTplWriter.h"
#include "TplTypeSelDlg.h"
#include "TplPageSelDlg.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ReportTplDlg, CDialogEx)

ReportTplDlg::ReportTplDlg(void) : CLDialog(ReportTplDlg::IDD)
{
	m_pReportTpl.reset();
	m_pTmpTplPageList.reset(new ReportTplPageList);
}

ReportTplDlg::~ReportTplDlg(void)
{
}

void ReportTplDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_TPL_TYPE, m_ctrlTplType);
	DDX_Control(pDX, IDC_LIST_TPL, m_ctrlTplList);

	// basic info.
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTplName);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_strTplType);
	DDX_Text(pDX, IDC_EDIT_CREATE_TIME, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_MODIFY_TIME, m_strModifyDate);
	DDX_Text(pDX, IDC_EDIT_COM, m_strComment);

	DDX_Control(pDX, IDC_LIST_TPL_PAGE, m_ctrlTplPageList);

	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BTN_ADD_PAGE, m_btnAddPage);
	DDX_Control(pDX, IDC_BTN_DEL_PAGE, m_btnDelPage);
	DDX_Control(pDX, IDC_BTN_MOVE_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BTN_MOVE_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}

BEGIN_MESSAGE_MAP(ReportTplDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_CBN_SELCHANGE(IDC_COMBO_TPL_TYPE, OnSelchangeTplType)
	ON_LBN_SELCHANGE(IDC_LIST_TPL, OnSelchangeTpl)

	ON_BN_CLICKED(IDC_BTN_ADD, OnAddTpl)
	ON_BN_CLICKED(IDC_BTN_DEL, OnDelTpl)
	ON_BN_CLICKED(IDC_BTN_MOVE_UP, OnMoveUp)
	ON_BN_CLICKED(IDC_BTN_MOVE_DOWN, OnMoveDown)

	ON_BN_CLICKED(IDC_BTN_ADD_PAGE, OnAddPage)
	ON_BN_CLICKED(IDC_BTN_DEL_PAGE, OnDelPage)

	ON_BN_CLICKED(IDC_BTN_APPLY, OnApply)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

BOOL ReportTplDlg::OnInitDialog()
{
	CLDialog::OnInitDialog();

	CString str;

	// load all report template.
	m_pReportTplList = ReportTplLoader::LoadAllReportTpl(FALSE);

	/************************************************************************/
	// init the combo box.

	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RPT_TPL_ALLTYPE);
	m_ctrlTplType.AddString(str);
	m_ctrlTplType.SetItemData(0, -1);

	vector<REPORT_TPL_TYPE> tplTypeList = ReportTpl::GetAllTplType();
	for (unsigned int i = 0; i < tplTypeList.size(); i++)
	{
		REPORT_TPL_TYPE tplType = tplTypeList.at(i);

		CString strTplTypeName = ReportTpl::GetReportTplTypeName(tplType);
		m_ctrlTplType.AddString(strTplTypeName);
		m_ctrlTplType.SetItemData(i+1, (int)tplType);
	}

	m_ctrlTplType.SetCurSel(0);
	/************************************************************************/	

	/************************************************************************/
	// init the style of the grid.

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

	// init the template list box.
	InitListBox(m_pReportTplList);

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_REPORT_TEMPLATES )->GetSafeHwnd(), L"", L"" );
	SetWindowTheme( GetDlgItem( IDC_COMBO_TPL_TYPE )->GetSafeHwnd(), L"", L"" );	
	SetWindowTheme( GetDlgItem( IDC_GROUP_PAPER_SIZE2 )->GetSafeHwnd(), L"", L"" );	
	SetWindowTheme( GetDlgItem( IDC_STATIC_TEMPLATE_PAGES )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH ReportTplDlg::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ReportTplDlg::OnSelchangeTplType()
{
	ReportTplListPtr pReportTplList;
	int iIndex = m_ctrlTplType.GetCurSel();
	if (iIndex == 0)
		pReportTplList = m_pReportTplList;
	else
	{
		REPORT_TPL_TYPE tplType = (REPORT_TPL_TYPE)m_ctrlTplType.GetItemData(m_ctrlTplType.GetCurSel());
		pReportTplList = m_pReportTplList->GetTplByType(tplType);
	}

	InitListBox(pReportTplList);
}

void ReportTplDlg::OnSelchangeTpl()
{
	// init the template info.
	InitTplInfo();
}

void ReportTplDlg::OnAddTpl()
{
	// decide the template type which will be created.
	REPORT_TPL_TYPE reportTplType;
	int iIndex = m_ctrlTplType.GetCurSel();
	if (iIndex == 0)
	{
		TplTypeSelDlg dlg;
		if (dlg.DoModal() == IDOK)
			reportTplType = dlg.GetSelTplType();
		else
			return;
	}
	else
		reportTplType = (REPORT_TPL_TYPE)m_ctrlTplType.GetItemData(m_ctrlTplType.GetCurSel());

	// create the new report template.
	CString strTplName;
	strTplName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_RPT_NEW_TPL_NAME);
	ReportTplPtr pNewReportTpl(new ReportTpl);
	pNewReportTpl->SetName(strTplName);
	pNewReportTpl->SetReportTplType(reportTplType);
	pNewReportTpl->SetCreateTime(DateHelper::GetCurrentTime());
	pNewReportTpl->SetModifyTime(DateHelper::GetCurrentTime());
	ReportTplPageListPtr pReportTplPageList(new ReportTplPageList);
	pNewReportTpl->SetReportTplPageList(pReportTplPageList);
	m_pReportTplList->push_back(pNewReportTpl);

	ReportTplWriter::CreateReportTpl(pNewReportTpl);

	int iCount = m_ctrlTplList.GetCount();
	m_ctrlTplList.InsertString(iCount, pNewReportTpl->GetName());
	m_ctrlTplList.SetItemData(iCount, (DWORD_PTR)(new LONGLONG(pNewReportTpl->GetID())));

	// select the first item.
	m_ctrlTplList.SetCurSel(iCount);

	// init the template info.
	InitTplInfo();
}

void ReportTplDlg::OnDelTpl()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	int iIndex = m_ctrlTplList.GetCurSel();
	if (iIndex == -1)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SELECT_TEMPLATE);
		MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_DELETE_TEMPLATE);
		if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			// delete the selected template page from the template and save the change.
			LONGLONG* pTplID = (LONGLONG*)m_ctrlTplList.GetItemData(iIndex);
			m_pReportTplList->DeleteReportTpl(*pTplID);
			ReportTplWriter::DeleteReportTpl(*pTplID);

			// update the UI.
			m_ctrlTplList.DeleteString(iIndex);
			if (m_ctrlTplList.GetCount() > 0)
				m_ctrlTplList.SetCurSel(m_ctrlTplList.GetCount() - 1);
			InitTplInfo();
		}
	}
}

void ReportTplDlg::OnAddPage()
{
	// the type of pages which can be added into the report template.
	REPORT_TPL_TYPE reportTplType = m_pReportTpl->GetReportTplType();
	vector<REPORT_TPL_PAGE_TYPE> tplPageTypeList = ReportTpl::GetLegalPageType(reportTplType);

	// select the template page.
	TplPageSelDlg dlg;
	dlg.Init(tplPageTypeList);
	if (dlg.DoModal() == IDOK)
	{
		ReportTplPagePtr pReportTplPage = dlg.GetSelTplPage();

		// check whether exist.
		if (m_pTmpTplPageList->GetPageByID(pReportTplPage->GetID()))
		{
			CString strSoluName, str;
			strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TEMPLATE_INCLUDE_PAGE);
			MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
			return;
		}

		// add the selected template page.
		m_pTmpTplPageList->push_back(pReportTplPage);

		// append a new item.
		int iCount = m_ctrlTplPageList.GetItemCount();
		InsertGridRow(iCount, pReportTplPage);
		m_ctrlTplPageList.SelectRow(iCount, TRUE);
		m_ctrlTplPageList.SetSelectionMark(iCount);
	}
}

void ReportTplDlg::OnDelPage()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	int iIndex = m_ctrlTplPageList.GetSelectionMark();
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
			// delete the selected template page.
			LONGLONG* pTplPageID = (LONGLONG*)m_ctrlTplPageList.GetItemData(iIndex);
			m_pTmpTplPageList->DeleteReportTplPage(*pTplPageID);

			// update the grid.
			m_ctrlTplPageList.DeleteItem(iIndex);
		}
	}
}

void ReportTplDlg::OnMoveUp()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	int iIndex = m_ctrlTplPageList.GetSelectionMark();
	if (iIndex == -1)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SELECT_TEMPLATE_PAGE);
		MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
	}
	else if (iIndex > 0)
	{
		LONGLONG* pTplPageID = (LONGLONG*)m_ctrlTplPageList.GetItemData(iIndex);

		// delete this item from the grid.
		m_ctrlTplPageList.DeleteItem(iIndex);

		// insert this item upper and select it.
		InsertGridRow(iIndex-1, m_pTmpTplPageList->GetPageByID(*pTplPageID));
		m_ctrlTplPageList.SelectRow(iIndex-1, TRUE);
		m_ctrlTplPageList.SetSelectionMark(iIndex-1);

		// update data.
		iter_swap(m_pTmpTplPageList->begin()+iIndex, m_pTmpTplPageList->begin()+iIndex-1);
	}
}

void ReportTplDlg::OnMoveDown()
{
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	int iIndex = m_ctrlTplPageList.GetSelectionMark();
	if (iIndex == -1)
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SELECT_TEMPLATE_PAGE);
		MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
	}
	else if (iIndex != m_ctrlTplPageList.GetItemCount()-1)
	{
		LONGLONG* pTplPageID = (LONGLONG*)m_ctrlTplPageList.GetItemData(iIndex);

		// delete this item from the grid.
		m_ctrlTplPageList.DeleteItem(iIndex);

		// insert this item upper and select it.
		InsertGridRow(iIndex+1, m_pTmpTplPageList->GetPageByID(*pTplPageID));
		m_ctrlTplPageList.SelectRow(iIndex+1, TRUE);
		m_ctrlTplPageList.SetSelectionMark(iIndex+1);

		// update data.
		iter_swap(m_pTmpTplPageList->begin()+iIndex, m_pTmpTplPageList->begin()+iIndex+1);
	}
}

void ReportTplDlg::OnApply()
{
	// check whether the current template page is modified.
	if (m_pReportTpl && IsTplModified())
	{
		m_pReportTpl->SetModified(TRUE);

		// save the change to database.
		SaveChange();

		// update the list box.
		int iIndex = m_ctrlTplList.GetCurSel();
		if (iIndex != -1)
		{
			m_ctrlTplList.DeleteString(iIndex);
			m_ctrlTplList.InsertString(iIndex, m_pReportTpl->GetName());
			m_ctrlTplList.SetItemData(iIndex, (DWORD_PTR)(new LONGLONG(m_pReportTpl->GetID())));
			m_ctrlTplList.SetCurSel(iIndex);
		}
	}
	else
	{
		CString strSoluName, str;
		strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_TEMPLATE_NOT_CHANGED);
		MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
	}
}

void ReportTplDlg::OnCancel()
{
	CDialogEx::OnCancel();
}

void ReportTplDlg::InitListBox(ReportTplListPtr pReportTplList)
{
	m_ctrlTplList.ResetContent();

	for (unsigned int i = 0; i < pReportTplList->size(); i++)
	{
		ReportTplPtr m_pReportTpl = pReportTplList->at(i);
		m_ctrlTplList.InsertString(i, m_pReportTpl->GetName());
		m_ctrlTplList.SetItemData(i, (DWORD_PTR)(new LONGLONG(m_pReportTpl->GetID())));
	}

	// select the first item.
	if (pReportTplList->size() > 0)
		m_ctrlTplList.SetCurSel(0);

	// init the template info.
	InitTplInfo();
}

void ReportTplDlg::InitTplInfo()
{
	// check whether the current template is modified.
	if (m_pReportTpl && IsTplModified())
	{
		m_pReportTpl->SetModified(TRUE);

		CString strSoluName, str;
		strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TEMPLATE);
		if (MessageBox(str, strSoluName, MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			// save the change to database.
			SaveChange();
		}
	}

	int iIndex = m_ctrlTplList.GetCurSel();
	if (iIndex == -1)
	{
		m_pReportTpl.reset();

		/************************************************************************/
		// clear all data.

		m_strTplName = _T("");
		m_strTplType = _T("");
		m_strCreateDate = _T("");
		m_strModifyDate = _T("");
		m_strComment = _T("");
		m_ctrlTplPageList.DeleteAllItems();
		/************************************************************************/

		/************************************************************************/
		// disable all controls.

		((CWnd*)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_TYPE))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_CREATE_TIME))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_MODIFY_TIME))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_EDIT_COM))->EnableWindow(FALSE);

		((CWnd*)GetDlgItem(IDC_LIST_TPL_PAGE))->EnableWindow(FALSE);
		/************************************************************************/
	}
	else
	{
		/************************************************************************/
		// enable all controls.

		((CWnd*)GetDlgItem(IDC_EDIT_NAME))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_TYPE))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_CREATE_TIME))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_MODIFY_TIME))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_EDIT_COM))->EnableWindow(TRUE);

		((CWnd*)GetDlgItem(IDC_LIST_TPL_PAGE))->EnableWindow(TRUE);
		/************************************************************************/

		/************************************************************************/
		// get the selected template.

		// the current selected template.
		LONGLONG* pTplID = (LONGLONG*)m_ctrlTplList.GetItemData(iIndex);
		m_pReportTpl = m_pReportTplList->GetTplByID(*pTplID);

		// the copy of the pages of the current template.
		ReportTplPageListPtr pTplPageList = m_pReportTpl->GetReportTplPageList();
		m_pTmpTplPageList->clear();
		m_pTmpTplPageList->insert(m_pTmpTplPageList->end(), pTplPageList->begin(), pTplPageList->end());
		/************************************************************************/

		// init data in "basic info" group.
		m_strTplName = m_pReportTpl->GetName();
		m_strTplType = ReportTpl::GetReportTplTypeName(m_pReportTpl->GetReportTplType());
		m_strCreateDate = DateHelper::GetStrFromTimeStamp(m_pReportTpl->GetCreateTime());
		m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pReportTpl->GetModifyTime());
		m_strComment = m_pReportTpl->GetComment();

		// init the pages grid.
		m_ctrlTplPageList.DeleteAllItems();
		for (unsigned int i = 0; i < m_pTmpTplPageList->size(); i++)
		{
			ReportTplPagePtr pReportTplPage = m_pTmpTplPageList->at(i);
			InsertGridRow(i, pReportTplPage);
		}
	}

	UpdateData(FALSE);
}

void ReportTplDlg::InsertGridRow(int iRowIndex, ReportTplPagePtr pReportTplPage)
{
	// insert a new row.
	m_ctrlTplPageList.InsertItem(iRowIndex, _T(""));

	// name.
	m_ctrlTplPageList.SetItemText(iRowIndex, 1, pReportTplPage->GetName());

	// page type
	CString strType = ReportTplPage::GetTplPageTypeName(pReportTplPage->GetTplPageType());
	m_ctrlTplPageList.SetItemText(iRowIndex, 2, strType);

	// paper width
	CString strWidth;
	strWidth.Format(INT_FORMAT, pReportTplPage->GetPaperWidth());
	m_ctrlTplPageList.SetItemText(iRowIndex, 3, strWidth);

	// paper height
	CString strHeight;
	strHeight.Format(INT_FORMAT, pReportTplPage->GetPaperHeight());
	m_ctrlTplPageList.SetItemText(iRowIndex, 4, strHeight);

	m_ctrlTplPageList.SetItemData(iRowIndex, (DWORD_PTR)(new LONGLONG(pReportTplPage->GetID())));
}

BOOL ReportTplDlg::IsTplModified()
{
	BOOL bRet = FALSE;

	UpdateData(TRUE);

	// the name.
	if (m_strTplName != m_pReportTpl->GetName())
		bRet = TRUE;

	// the comment.
	if (m_strComment != m_pReportTpl->GetComment())
		bRet = TRUE;

	// the pages in the template.
	ReportTplPageListPtr pReportTplPageList = m_pReportTpl->GetReportTplPageList();
	if (pReportTplPageList->size() != m_pTmpTplPageList->size())
		bRet = TRUE;
	else
	{
		for (unsigned int i = 0; i < pReportTplPageList->size(); i++)
		{
			ReportTplPagePtr pReportTplPage = pReportTplPageList->at(i);
			if (pReportTplPage->GetID() != m_pTmpTplPageList->at(i)->GetID())
			{
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

void ReportTplDlg::SaveChange()
{
	/************************************************************************/
	// update the data.

	// basic info.
	m_pReportTpl->SetName(m_strTplName);
	m_pReportTpl->SetComment(m_strComment);

	// pages.
	ReportTplPageListPtr pReportTplPageList = m_pReportTpl->GetReportTplPageList();
	pReportTplPageList->clear();
	pReportTplPageList->insert(pReportTplPageList->end(), m_pTmpTplPageList->begin(), m_pTmpTplPageList->end());
	/************************************************************************/

	// update database.
	ReportTplWriter::UpdateBasicData(m_pReportTpl);
	ReportTplWriter::UpdateModifyDate(m_pReportTpl->GetID(), DateHelper::GetCurrentTime());
	ReportTplWriter::UpdateTplPage(m_pReportTpl->GetID(), m_pReportTpl->GetReportTplPageList());

	// message.
	CString strSoluName, str;
	strSoluName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_SAVE_TEMPLATE_OK);
	MessageBox(str, strSoluName, MB_OK | MB_ICONWARNING);
}

END_CUTLEADER_NAMESPACE()
