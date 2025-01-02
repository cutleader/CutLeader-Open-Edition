#include "StdAfx.h"
#include "NCConfigPage.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clUtilityResource.h"

#include "baseConst.h"
#include "clUILibResource.h"
#include "taoresource.h"
#include "clExpertLibResource.h"

#include "NCConfigValue.h"
#include "NCConfigValueList.h"
#include "NCConfigItem.h"
#include "NCConfigType.h"

#include "Uxtheme.h"
#pragma comment(lib, "Uxtheme.lib")

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(NCConfigPage, CResizableDialog)

NCConfigPage::NCConfigPage(void)
			: CLResizableDialog(NCConfigPage::IDD)
{
}

NCConfigPage::~NCConfigPage(void)
{
}

void NCConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_NCCONFIG, m_ncConfigValList);
}

BEGIN_MESSAGE_MAP(NCConfigPage, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()

	ON_MESSAGE(WM_GET_CELL_EDITOR_DATA, GetCellEditorData)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_NCCONFIG, OnEndLabelEdit)

END_MESSAGE_MAP()

BOOL NCConfigPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_NC_CONFIG, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_NCCONFIG, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	// init something
	InitListStyle();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_NC_CONFIG )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH NCConfigPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NCConfigPage::OnDestroy()
{
	ReleaseGridItemData();
	CResizableDialog::OnDestroy();
}

void NCConfigPage::DisplayNCConfig(NCConfigItemPtr pNCConfigItem)
{
	m_pNCConfigItem = pNCConfigItem;

	// init the data.
	InitListData();

	UpdateData(FALSE);
}

NCConfigItemPtr NCConfigPage::GetNCConfig() 
{
	UpdateData(TRUE);


	return m_pNCConfigItem; 
}

void NCConfigPage::InitListStyle()
{
	// init the style of the list ctrl

	CString str;

	// Requires one never uses column 0
	m_ncConfigValList.InsertHiddenLabelColumn();

	// "Name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_ncConfigValList.InsertColumnTrait(1, str, LVCFMT_LEFT, 150, 0, pNUMCol);

	// "Value" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_VALUE);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_ncConfigValList.InsertColumnTrait(2, str, LVCFMT_LEFT, 180, 0, pNameCol);

	// we should decide the cell editor for this column at run-time.
	m_ncConfigValList.AddDynamicCol(2);

	// set other style.
	m_ncConfigValList.SetExtendedStyle(LVS_EX_CHECKBOXES | m_ncConfigValList.GetExtendedStyle());
	m_ncConfigValList.EnableVisualStyles(TRUE);
}

void NCConfigPage::InitListData()
{
	NCConfigValueListPtr pNCConfigValList = m_pNCConfigItem->GetNCConfigVal();

	// clear the grid.
	ReleaseGridItemData();
	m_ncConfigValList.DeleteAllItems();

	// go through all the condition of current machine and init the grid.
	for (unsigned int i = 0; i < pNCConfigValList->size(); i++)
	{
		NCConfigValuePtr pNCConfigVal = pNCConfigValList->at(i);
		NCConfigTypePtr pNCConfigType = pNCConfigVal->GetNCConfigType();

		m_ncConfigValList.InsertItem(i, _T(""));
		m_ncConfigValList.SetItemText(i, 1, pNCConfigType->GetNCConfigName());
		m_ncConfigValList.SetItemText(i, 2, pNCConfigVal->GetStrValue(TRUE));

		// we put the id into grid item.
		m_ncConfigValList.SetItemData(i, (DWORD_PTR)(new LONGLONG(pNCConfigVal->GetID())));
	}
}

LRESULT NCConfigPage::GetCellEditorData(WPARAM wParam, LPARAM lParam)
{
	CellEditorData* pCellEditorData = reinterpret_cast<CellEditorData*>(wParam);

	// get the NC config val id.
	int iRow = pCellEditorData->iRow;
	LONGLONG* pNCConfigValId = (LONGLONG*)m_ncConfigValList.GetItemData(iRow);

	// get the NCConfigValue
	NCConfigValueListPtr pNCConfigValList = m_pNCConfigItem->GetNCConfigVal();
	NCConfigValuePtr pNCConfigVal = pNCConfigValList->GetNCConfigValByID(*pNCConfigValId);

	// build the proper cell editor data.
	NCConfigTypePtr pNCConfigType = pNCConfigVal->GetNCConfigType();
	if (pNCConfigType->GetNCConfigDataType() == BASIC_DATA_STRING)
	{
		pCellEditorData->m_cellEditorType = STREDITBOX;
	}
	else if (pNCConfigType->GetNCConfigDataType() == BASIC_DATA_INT)
	{
		pCellEditorData->m_cellEditorType = INTEDITBOX;
	}
	else if (pNCConfigType->GetNCConfigDataType() == BASIC_DATA_FLOAT)
	{
		pCellEditorData->m_cellEditorType = FLOATEDITBOX;
	}
	else if (pNCConfigType->GetNCConfigDataType() == BASIC_DATA_BOOL)
	{
		pCellEditorData->m_cellEditorType = COMBOBOX;

		// prepare the options for combo box.
		CString str1, str2, str3;
		str1 = EMPTY_STRING;
		str2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_YES);
		str3 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NO);

		pCellEditorData->m_optionValList.push_back(str1);
		pCellEditorData->m_optionValList.push_back(str2);
		pCellEditorData->m_optionValList.push_back(str3);
	}

	return true;
}

void NCConfigPage::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int item = pDispInfo->item.iItem;
	CString str = pDispInfo->item.pszText;

	// get the NCConfigValue.
	LONGLONG* pNCConfigValId = (LONGLONG*)m_ncConfigValList.GetItemData(item);
	NCConfigValueListPtr pNCConfigValList = m_pNCConfigItem->GetNCConfigVal();
	NCConfigValuePtr pNCConfigVal = pNCConfigValList->GetNCConfigValByID(*pNCConfigValId);

	// update the CondValInfo
	if (pNCConfigVal->GetStrValue(TRUE) != str)
	{
		_bstr_t bstr = str;
		pNCConfigVal->SetNCConfigVal(bstr, TRUE);
		m_pNCConfigItem->SetModified(TRUE);
	}
}

void NCConfigPage::ReleaseGridItemData()
{
	for (int i = 0; i < m_ncConfigValList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_ncConfigValList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()
