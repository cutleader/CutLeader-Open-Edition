#include "StdAfx.h"
#include "ConditionEditPage.h"

#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clUtilityResource.h"

#include "baseConst.h"
#include "clUILibResource.h"

#include "ConditionType.h"
#include "ConditionTypeList.h"
#include "ConditionNameDlg.h"
#include "clMachineLibResource.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ConditionEditPage, CResizableDialog)

ConditionEditPage::ConditionEditPage(void) : CLResizableDialog(ConditionEditPage::IDD)
{
	m_menu.LoadMenu(IDR_POPUP_MAC_COND);
}

ConditionEditPage::~ConditionEditPage(void)
{
}

void ConditionEditPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_MACCOND, m_condList);
}

BEGIN_MESSAGE_MAP(ConditionEditPage, CResizableDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()

	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_MACCOND, OnEndLabelEdit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MACCOND, OnRClickList)

	/************************************************************************/
	// for the popup menu.

	ON_COMMAND(IDC_NEW_MAC_COND, OnCreateMacCond)
	ON_COMMAND(IDC_DEL_MAC_COND, OnDelMacCond)
	/************************************************************************/
END_MESSAGE_MAP()

BOOL ConditionEditPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	MultiLanguageMgr::GetInstance()->TranslateMenu(m_menu.GetSubMenu(0));

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_COND, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_MACCOND, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	InitGridStyle();
	SetBackgroundColor( SkinManager::GetBackgroundColor() );

	SetWindowTheme( GetDlgItem( IDC_STATIC_COND )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH ConditionEditPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

void ConditionEditPage::OnDestroy()
{
	ReleaseGridItemData();
	CResizableDialog::OnDestroy();
}

void ConditionEditPage::OnCreateMacCond()
{
	// new a condition
	ConditionNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strName = dlg.GetCondName();

		// we should add the new Condition type to "m_pMacConds"
		ConditionTypePtr pCondType(new ConditionType(strName));
		m_pMacConds->push_back(pCondType);
		m_pMacConds->SetModified(TRUE);

		// add it to grid.
		AddNewRow(pCondType);
	}
}

void ConditionEditPage::OnDelMacCond()
{
	int index = m_condList.GetSelectionMark();
	if (index != -1)
	{
		LONGLONG* pCondTypeID = (LONGLONG*)m_condList.GetItemData(index);

		// delete from list
		m_pMacConds->DeleteItemByID(*pCondTypeID);
		m_pMacConds->SetModified(TRUE);

		// delete from listctrl
		m_condList.DeleteItem(index);
	}
}

void ConditionEditPage::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = reinterpret_cast<LV_DISPINFO*>(pNMHDR);

	int item = pDispInfo->item.iItem;
	int subitem = pDispInfo->item.iSubItem;
	CString str = pDispInfo->item.pszText;

	// get the relative condition type.
	LONGLONG* pCondTypeID = (LONGLONG*)m_condList.GetItemData(item);
	ConditionTypePtr pCondType = m_pMacConds->GetItemByID(*pCondTypeID);

	BOOL bModified = FALSE;

	if (subitem == 2) // config name.
	{
		if (pCondType->GetCondName() != str)
		{
			pCondType->SetCondName(str);
			bModified = TRUE;
		}
	}
	if (subitem == 3) // condition category
	{
		if (pCondType->GetCateName() != str)
		{
			pCondType->SetCondCategory(str);
			bModified = TRUE;
		}
	}
	else if (subitem == 4) // data type
	{
		if (pCondType->GetDataTypeName() != str)
		{
			pCondType->SetCondDataType(str);
			bModified = TRUE;
		}
	}
	else if (subitem == 5) // Unique ID
	{
		if (pCondType->GetUniqueID() != _wtoi(str))
		{
			pCondType->SetUniqueID(_wtoi(str));
			bModified = TRUE;
		}
	}
	else if (subitem == 6) // comment
	{
		if (pCondType->GetCondComment() != str)
		{
			pCondType->SetCondComment(str);
			bModified = TRUE;
		}
	}

	if (bModified)
		m_pMacConds->SetModified(TRUE);
}

void ConditionEditPage::OnRClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	/************************************************************************/
	// select tree item which r-button clicked.

	POINT pos;
	if (!GetCursorPos(&pos))
		return;
	m_condList.ScreenToClient(&pos);

	UINT uFlags;
	int iIndex = m_condList.HitTest(pos, &uFlags);

	/************************************************************************/

	/************************************************************************/
	// display the popup menu.

	// figure out the position where the menu will pop up.
	CRect rect;
	this->GetClientRect(&rect);
	ClientToScreen(&rect);
	m_condList.ClientToScreen(&pos);

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

void ConditionEditPage::InitGridStyle()
{
	// Requires one never uses column 0
	m_condList.InsertHiddenLabelColumn();

	CString str;

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_condList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitEdit;
	m_condList.InsertColumnTrait(2, str, LVCFMT_LEFT, 90, 0, pNameCol);

	// "condition category" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COND_CATEGORYTYPE);
	CGridColumnTraitCombo* pCateCol = new CGridColumnTraitCombo;
	pCateCol->SetStyle(pCateCol->GetStyle() | CBS_DROPDOWNLIST);
	StringListPtr pList1 = ConditionType::GetAllCategoryType();
	for (unsigned int i = 0; i < pList1->size(); i++)
		pCateCol->AddItem(i, pList1->at(i));
	m_condList.InsertColumnTrait(3, str, LVCFMT_LEFT, 120, 0, pCateCol);

	// "data type" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_DATATYPE);
	CGridColumnTraitCombo* pTypeCol = new CGridColumnTraitCombo;
	pTypeCol->SetStyle(pTypeCol->GetStyle() | CBS_DROPDOWNLIST);
	StringListPtr pList2 = ConditionType::GetAllDataType();
	for (unsigned int j = 0; j < pList2->size(); j++)
		pTypeCol->AddItem(j, pList2->at(j));
	m_condList.InsertColumnTrait(4, str, LVCFMT_LEFT, 130, 0, pTypeCol);

	// Unique ID column.
	CGridColumnTrait* pUniqueIDCol = new CGridColumnTraitEdit(INTEDITBOX);
	m_condList.InsertColumnTrait(5, _T("Unique ID"), LVCFMT_LEFT, 70, 0, pUniqueIDCol);

	// comment column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMENT);
	CGridColumnTrait* pCommentCol = new CGridColumnTraitEdit;
	m_condList.InsertColumnTrait(6, str, LVCFMT_LEFT, 120, 0, pCommentCol);

	m_condList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void ConditionEditPage::InitGridData()
{
	int nItem = 0;
	for (unsigned int i = 0; i < m_pMacConds->size(); i++)
	{
		ConditionTypePtr pCondType = m_pMacConds->at(i);

		// get the category name of the condition type.
		CString cateName = pCondType->GetCateName();
		// get the name of the condition data type.
		CString typeName = pCondType->GetDataTypeName();

		//
		CString strUniqueID;
		strUniqueID.Format(_T("%d"), pCondType->GetUniqueID());

		// init a row.
		CString strNum;
		strNum.Format(_T("%d"), i+1);
		nItem = m_condList.InsertItem(++nItem, strNum);

		// column num is started from 1 because there is a hidden column.
		m_condList.SetItemText(nItem, 1, strNum);
		m_condList.SetItemText(nItem, 2, pCondType->GetCondName());
		m_condList.SetItemText(nItem, 3, cateName);
		m_condList.SetItemText(nItem, 4, typeName);
		m_condList.SetItemText(nItem, 5, strUniqueID);
		m_condList.SetItemText(nItem, 6, pCondType->GetCondComment());

		// store the ID in item data
		m_condList.SetItemData(nItem, (DWORD_PTR)(new LONGLONG(pCondType->GetID())));
	}

	m_condList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void ConditionEditPage::DisplayMacConds(ConditionTypeListPtr pMacConds)
{
	m_pMacConds = pMacConds;

	// clear grid.
	ReleaseGridItemData();
	m_condList.DeleteAllItems();

	InitGridData();
}

void ConditionEditPage::AddNewRow(ConditionTypePtr pCondType)
{
	int iRowCount = m_condList.GetItemCount();

	// init a row.
	CString strNum;
	strNum.Format(_T("%d"), iRowCount+1);
	iRowCount = m_condList.InsertItem(iRowCount, strNum);

	// column num is started from 1 because there is a hidden column.
	m_condList.SetItemText(iRowCount, 1, strNum);

	// name column
	CString strName = pCondType->GetCondName();
	m_condList.SetItemText(iRowCount, 2, strName);

	// "category" column
	m_condList.SetItemText(iRowCount, 3, pCondType->GetCateName());

	// "data type" column
	m_condList.SetItemText(iRowCount, 4, pCondType->GetDataTypeName());

	// the unique ID.
	CString strUniqueID;
	strUniqueID.Format(_T("%d"), pCondType->GetUniqueID());
	m_condList.SetItemText(iRowCount, 5, strUniqueID);

	// comment column
	m_condList.SetItemText(iRowCount, 6, pCondType->GetCondComment());

	m_condList.SetItemData(iRowCount, (DWORD_PTR)(new LONGLONG(pCondType->GetID())));
}

void ConditionEditPage::ReleaseGridItemData()
{
	for (int i = 0; i < m_condList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_condList.GetItemData(i);
		delete (LONGLONG*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()
