#include "StdAfx.h"
#include "NCConfigTypeEditPage.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clUtilityResource.h"

#include "baseConst.h"

#include "NCConfigTypeNameDlg.h"
#include "NCConfigTypeList.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(NCConfigTypeEditPage, CResizableDialog)

NCConfigTypeEditPage::NCConfigTypeEditPage() 
						: CLResizableDialog(NCConfigTypeEditPage::IDD)
{
	m_menu.LoadMenu(IDR_POPUP_NCCONF_TYPE);
}

NCConfigTypeEditPage::NCConfigTypeEditPage(NCConfigTypeListPtr pNCConfigTypeList) 
						: CLResizableDialog(NCConfigTypeEditPage::IDD)
{
	m_pNCConfigTypeList = pNCConfigTypeList;
	m_menu.LoadMenu(IDR_POPUP_NCCONF_TYPE);
}

NCConfigTypeEditPage::~NCConfigTypeEditPage(void)
{
}

void NCConfigTypeEditPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_NCCONFIG_TYPE, m_ncConfigList);
}

BEGIN_MESSAGE_MAP(NCConfigTypeEditPage, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()

	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_NCCONFIG_TYPE, OnEndLabelEdit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_NCCONFIG_TYPE, OnRClickList)

	ON_COMMAND(IDC_NEW_NCCONF_TYPE, OnNewNCConfig)
	ON_COMMAND(IDC_DEL_NCCONF_TYPE, OnDelNCConfig)
END_MESSAGE_MAP()

BOOL NCConfigTypeEditPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	MultiLanguageMgr::GetInstance()->TranslateMenu(m_menu.GetSubMenu(0));

	// layout controls.
	AddAnchor(IDC_STATIC_NCTYPE, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_NCCONFIG_TYPE, TOP_LEFT, BOTTOM_RIGHT);

	InitGridStyle();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_NCTYPE )->GetSafeHwnd(), L"", L"" );

	return TRUE;
}

HBRUSH NCConfigTypeEditPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void NCConfigTypeEditPage::OnDestroy()
{
	ReleaseGridItemData();
	CResizableDialog::OnDestroy();
}

void NCConfigTypeEditPage::OnRClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	/************************************************************************/
	// select tree item which r-button clicked.

	POINT pos;
	if (!GetCursorPos(&pos))
		return;
	m_ncConfigList.ScreenToClient(&pos);

	UINT uFlags;
	int iIndex = m_ncConfigList.HitTest(pos, &uFlags);

	/************************************************************************/

	/************************************************************************/
	// display the popup menu.

	// figure out the position where the menu will pop up.
	CRect rect;
	this->GetClientRect(&rect);
	ClientToScreen(&rect);
	m_ncConfigList.ClientToScreen(&pos);

	// pop up the menu.
	CMenu* pPopupMenu = NULL;
	if (rect.PtInRect(pos))
	{
		pPopupMenu = (CMenu*)m_menu.GetSubMenu(0);

		if (pPopupMenu != NULL)
			pPopupMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pos.x, pos.y, this, NULL);
	}
	/************************************************************************/

	*pResult = 0;
}

CString NCConfigTypeEditPage::GetDataTypeName(BASIC_DATA_TYPE ncConfigDataType) const
{
	CString typeName;
	if (ncConfigDataType == BASIC_DATA_STRING)
	{
		typeName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_STRING);
	}
	else if (ncConfigDataType == BASIC_DATA_INT)
	{
		typeName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_INTEGER);
	}
	else if (ncConfigDataType == BASIC_DATA_BOOL)
	{
		typeName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_BOOLEAN);
	}
	else if (ncConfigDataType == BASIC_DATA_FLOAT)
	{
		typeName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FLOAT);
	}

	return typeName;
}

void NCConfigTypeEditPage::InitGridStyle()
{
	// Requires one never uses column 0
	m_ncConfigList.InsertHiddenLabelColumn();

	CString str;

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_ncConfigList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitEdit;
	m_ncConfigList.InsertColumnTrait(2, str, LVCFMT_LEFT, 90, 0, pNameCol);

	// "data type" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATATYPE);
	CGridColumnTraitCombo* pTypeCol = new CGridColumnTraitCombo;
	pTypeCol->SetStyle(pTypeCol->GetStyle() | CBS_DROPDOWNLIST);
	StringListPtr pList2 = NCConfigType::GetAllDataType();
	for (unsigned int j = 0; j < pList2->size(); j++)
		pTypeCol->AddItem(j, pList2->at(j));
	m_ncConfigList.InsertColumnTrait(3, str, LVCFMT_LEFT, 130, 0, pTypeCol);

	// Unique ID column.
	CGridColumnTrait* pUniqueIDCol = new CGridColumnTraitEdit(INTEDITBOX);
	m_ncConfigList.InsertColumnTrait(4, _T("Unique ID"), LVCFMT_LEFT, 70, 0, pUniqueIDCol);

	// comment column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMENT);
	CGridColumnTrait* pCommentCol = new CGridColumnTraitEdit;
	m_ncConfigList.InsertColumnTrait(5, str, LVCFMT_LEFT, 260, 0, pCommentCol);

	m_ncConfigList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void NCConfigTypeEditPage::DisplayNCConfigTypeList(NCConfigTypeListPtr pNCConfigTypeList)
{
	m_pNCConfigTypeList = pNCConfigTypeList;

	// clean the grid.
	ReleaseGridItemData();
	m_ncConfigList.DeleteAllItems();

	InitGridData();
}

void NCConfigTypeEditPage::InitGridData()
{
	int nItem = 0;
	for (unsigned int i = 0; i < m_pNCConfigTypeList->size(); i++)
	{
		NCConfigTypePtr pNCConfigType = m_pNCConfigTypeList->at(i);

		// get the name of the condition data type.
		CString typeName = GetDataTypeName(pNCConfigType->GetNCConfigDataType());

		//
		CString strUniqueID;
		strUniqueID.Format(_T("%d"), pNCConfigType->GetUniqueID());

		// init a row.
		CString strNum;
		strNum.Format(_T("%d"), i+1);
		nItem = m_ncConfigList.InsertItem(++nItem, strNum);

		// column num is started from 1 because there is a hidden column.
		m_ncConfigList.SetItemText(nItem, 1, strNum);
		m_ncConfigList.SetItemText(nItem, 2, pNCConfigType->GetNCConfigName());
		m_ncConfigList.SetItemText(nItem, 3, typeName);
		m_ncConfigList.SetItemText(nItem, 4, strUniqueID);
		m_ncConfigList.SetItemText(nItem, 5, pNCConfigType->GetComment());

		// store the ID in item data
		m_ncConfigList.SetItemData(nItem, (DWORD_PTR)(new LONGLONG(pNCConfigType->GetID())));
	}
}

void NCConfigTypeEditPage::ReleaseGridItemData()
{
	for (int i = 0; i < m_ncConfigList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_ncConfigList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

void NCConfigTypeEditPage::AddNewRow(NCConfigTypePtr pNCConfigType)
{
	int iRowCount = m_ncConfigList.GetItemCount();

	// init a row.
	CString strNum;
	strNum.Format(_T("%d"), iRowCount+1);
	iRowCount = m_ncConfigList.InsertItem(iRowCount, strNum);

	// column num is started from 1 because there is a hidden column.
	m_ncConfigList.SetItemText(iRowCount, 1, strNum);

	// name column
	CString strName = pNCConfigType->GetNCConfigName();
	m_ncConfigList.SetItemText(iRowCount, 2, strName);

	// "data type" column
	m_ncConfigList.SetItemText(iRowCount, 3, GetDataTypeName(pNCConfigType->GetNCConfigDataType()));

	// Unique ID column.
	CString strUniqueID;
	strUniqueID.Format(_T("%d"), pNCConfigType->GetUniqueID());
	m_ncConfigList.SetItemText(iRowCount, 4, strUniqueID);

	// comment column.
	m_ncConfigList.SetItemText(iRowCount, 5, pNCConfigType->GetComment());

	m_ncConfigList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pNCConfigType->GetID())));
}

void NCConfigTypeEditPage::OnNewNCConfig()
{
	// new a condition
	NCConfigTypeNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strName = dlg.GetNCConfigName();

		// we should add the new type to "m_pNCConfigTypeList"
		NCConfigTypePtr pNCConfigType(new NCConfigType(strName));
		m_pNCConfigTypeList->push_back(pNCConfigType);
		m_pNCConfigTypeList->SetModified(TRUE);

		// add it to grid.
		AddNewRow(pNCConfigType);
	}
}

void NCConfigTypeEditPage::OnDelNCConfig()
{
	int index = m_ncConfigList.GetSelectionMark();
	if (index != -1)
	{
		LONGLONG* pCondTypeID = (LONGLONG*)m_ncConfigList.GetItemData(index);

		// delete from list
		m_pNCConfigTypeList->DeleteItemByID(*pCondTypeID);
		m_pNCConfigTypeList->SetModified(TRUE);

		//
		m_ncConfigList.DeleteItem(index);
	}
}

void NCConfigTypeEditPage::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	CString str = pDispInfo->item.pszText;

	// get the relative condition type.
	LONGLONG* pNCConfigTypeID = (LONGLONG*)m_ncConfigList.GetItemData(item);
	NCConfigTypePtr pNCConfigType = m_pNCConfigTypeList->GetItemByID(*pNCConfigTypeID);

	// 修改。
	BOOL bModified = FALSE;
	if (subitem == 2) // config name.
	{
		if (pNCConfigType->GetNCConfigName() != str)
		{
			pNCConfigType->SetNCConfigName(str);
			bModified = TRUE;
		}
	}
	if (subitem == 3) // data type
	{
		if (pNCConfigType->GetDataTypeName() != str)
		{
			pNCConfigType->SetDataType(str);
			bModified = TRUE;
		}
	}
	else if (subitem == 4) // Unique ID.
	{
		if (pNCConfigType->GetUniqueID() != _wtoi(str))
		{
			pNCConfigType->SetUniqueID(_wtoi(str));
			bModified = TRUE;
		}
	}
	else if (subitem == 5) // comment
	{
		if (pNCConfigType->GetComment() != str)
		{
			pNCConfigType->SetComment(str);
			bModified = TRUE;
		}
	}
	if (bModified)
		m_pNCConfigTypeList->SetModified(TRUE);
}

END_CUTLEADER_NAMESPACE()
