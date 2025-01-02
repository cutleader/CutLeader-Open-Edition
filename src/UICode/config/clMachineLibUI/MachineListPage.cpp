#include "StdAfx.h"
#include "MachineListPage.h"

#include "baseConst.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"
#include "clUtilityResource.h"
#include "clUILibResource.h"
#include "DateHelper.h"

#include "clMachineLibResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(MachineListPage, CResizableDialog)

MachineListPage::MachineListPage(void) : CLResizableDialog(MachineListPage::IDD)
{
}

MachineListPage::~MachineListPage(void)
{
}

void MachineListPage::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_MACLIST, m_macGrid);
}

BEGIN_MESSAGE_MAP(MachineListPage, CResizableDialog)
	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()

HBRUSH MachineListPage::OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor )
{
	return SkinManager::GetDialogCtrlColor( pDC, pWnd, nCtlColor );
}

BOOL MachineListPage::OnInitDialog()
{
	__super::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_MACLIST, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_MACLIST, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	InitGridStyle();

	SetBackgroundColor( SkinManager::GetBackgroundColor() );
	
	SetWindowTheme( GetDlgItem( IDC_STATIC_MACLIST )->GetSafeHwnd(), L"", L"" );

	UpdateData(FALSE);

	return TRUE;
}

void MachineListPage::InitGridStyle()
{
	// Requires one never uses column 0
	m_macGrid.InsertHiddenLabelColumn();

	CString str;

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_macGrid.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_macGrid.InsertColumnTrait(2, str, LVCFMT_LEFT, 150, 0, pNameCol);

	// comment column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMENT);
	CGridColumnTrait* pCommentCol = new CGridColumnTraitText;
	m_macGrid.InsertColumnTrait(3, str, LVCFMT_LEFT, 200, 0, pCommentCol);

	// "create date" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CREATE_DATE);
	CGridColumnTrait* pCreateDateCol = new CGridColumnTraitText;
	m_macGrid.InsertColumnTrait(4, str, LVCFMT_LEFT, 120, 0, pCreateDateCol);

	// "modify date" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MODIFY_DATE);
	CGridColumnTrait* pModifyDateCol = new CGridColumnTraitText;
	m_macGrid.InsertColumnTrait(5, str, LVCFMT_LEFT, 120, 0, pModifyDateCol);

	m_macGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void MachineListPage::InitGridData()
{
	int nItem = 0;
	for (unsigned int i = 0; i < m_propList.size(); i++)
	{
		DataProperty prop = m_propList[i];

		// init a row.
		CString strNum;
		strNum.Format(_T("%d"), i+1);
		nItem = m_macGrid.InsertItem(++nItem, strNum);

		// column num is started from 1 because there is a hidden column.
		m_macGrid.SetItemText(nItem, 1, strNum);
		m_macGrid.SetItemText(nItem, 2, prop.GetItem(1).GetValue().bstrVal);
		m_macGrid.SetItemText(nItem, 3, prop.GetItem(2).GetValue().bstrVal);

		// "create date" column.
		CString strTime = DateHelper::GetStrFromTimeStamp(prop.GetItem(3).GetValue().llVal);
		m_macGrid.SetItemText(nItem, 4, strTime);

		// "modify date" column.
		strTime = DateHelper::GetStrFromTimeStamp(prop.GetItem(4).GetValue().llVal);
		m_macGrid.SetItemText(nItem, 5, strTime);
	}
}

void MachineListPage::DisplayMacList(vector<DataProperty> propList)
{
	m_propList = propList;

	m_macGrid.DeleteAllItems();
	InitGridData();
}

END_CUTLEADER_NAMESPACE()
