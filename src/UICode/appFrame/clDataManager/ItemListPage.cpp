#include "stdafx.h"
#include "ItemListPage.h"

#include "DateHelper.h"
#include "ClUtilityResource.h"
#include "CGridColumnManagerProfile.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridRowTraitXP.h"

#include "baseConst.h"
#include "ClBaseDataResource.h"
#include "DataCenterNode.h"
#include "DataCenterItemList.h"
#include "DataItem.h"
#include "FolderNode.h"
#include "DataCenterItemBase.h"
#include "ClBaseDataResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const size_t MAX_TIP_TEXT_LENGTH = 1024;

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNCREATE(ItemListPage, CLResizableDialog)

ItemListPage::ItemListPage() : CLResizableDialog(ItemListPage::IDD)
{
	m_pDataCenterItemList.reset();
}

ItemListPage::~ItemListPage()
{
}

void ItemListPage::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_ITEM, m_dataCenterItemGrid);
}

BEGIN_MESSAGE_MAP(ItemListPage, CLResizableDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL ItemListPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	/************************************************************************/
	// layout controls.

	AddAnchor(IDC_STATIC_SELECTED_ITEMS, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_LIST_ITEM, TOP_LEFT, BOTTOM_RIGHT);
	/************************************************************************/

	InitGridStyle();

	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_STATIC_SELECTED_ITEMS)->GetSafeHwnd(), L"", L"");

	UpdateData(FALSE);

	return TRUE;
}

HBRUSH ItemListPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void ItemListPage::InitGridStyle()
{
	// Requires one never uses column 0
	m_dataCenterItemGrid.InsertHiddenLabelColumn();

	CString str;

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_dataCenterItemGrid.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_dataCenterItemGrid.InsertColumnTrait(2, str, LVCFMT_LEFT, 100, 0, pNameCol);

	// "node datatype"
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NODE_DATATYPE);
	CGridColumnTrait* pNodeTypeCol = new CGridColumnTraitText;
	m_dataCenterItemGrid.InsertColumnTrait(3, str, LVCFMT_LEFT, 120, 0, pNodeTypeCol);

	// "create date" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CREATE_DATE);
	CGridColumnTrait* pCreateDateCol = new CGridColumnTraitText;
	m_dataCenterItemGrid.InsertColumnTrait(4, str, LVCFMT_LEFT, 120, 0, pCreateDateCol);

	// "modify date" column.
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MODIFY_DATE);
	CGridColumnTrait* pModifyDateCol = new CGridColumnTraitText;
	m_dataCenterItemGrid.InsertColumnTrait(5, str, LVCFMT_LEFT, 120, 0, pModifyDateCol);

	m_dataCenterItemGrid.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void ItemListPage::DisplayItemList(DataCenterItemListPtr pDataCenterItemList)
{
	m_pDataCenterItemList = pDataCenterItemList;

	//
	InitGridData();
}

void ItemListPage::InitGridData()
{
	m_dataCenterItemGrid.DeleteAllItems();

	int iItem = 0;
	for (unsigned int i = 0; i < m_pDataCenterItemList->size(); i++)
	{
		IDataCenterItemPtr pDataCenterItem = m_pDataCenterItemList->at(i);

		// get the product item type.
		CString strNodeType;
		if (pDataCenterItem->IsFolder())
		{
			strNodeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_FOLDER_NODE);
		}
		else
		{
			DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);

			if (pDataItem->GetDataType() == DATANODE_PART)
				strNodeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_NODE);
			else if (pDataItem->GetDataType() == DATANODE_Task)
				strNodeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Task_Node);
		}

		// insert a row.
		{
			CString strNum;
			strNum.Format(_T("%d"), iItem + 1);
			m_dataCenterItemGrid.InsertItem(iItem, strNum);
			m_dataCenterItemGrid.SetItemText(iItem, 1, strNum);
			m_dataCenterItemGrid.SetItemText(iItem, 2, pDataCenterItem->GetName());

			// "node data type" column.
			m_dataCenterItemGrid.SetItemText(iItem, 3, strNodeType);

			// "create date" column.
			CString strTime = DateHelper::GetStrFromTimeStamp(pDataCenterItem->GetCreateTime());
			m_dataCenterItemGrid.SetItemText(iItem, 4, strTime);

			// "modify date" column.
			strTime = DateHelper::GetStrFromTimeStamp(pDataCenterItem->GetModifyTime());
			m_dataCenterItemGrid.SetItemText(iItem, 5, strTime);

			iItem++;
		}
	}
}

END_CUTLEADER_NAMESPACE()
