#include "stdafx.h"
#include "FolderContentPage.h"

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

IMPLEMENT_DYNCREATE(FolderContentPage, CLResizableDialog)

FolderContentPage::FolderContentPage() : CLResizableDialog(FolderContentPage::IDD)
{
}

FolderContentPage::~FolderContentPage()
{
}

void FolderContentPage::DoDataExchange(CDataExchange* pDX)
{
	CLResizableDialog::DoDataExchange(pDX);

	// basic info group.
	DDX_Text(pDX, IDC_EDIT_FOLDER_NAME, m_strName);
	DDX_Control(pDX, IDC_EDIT_FOLDER_NAME, m_ctrlNameEditBox);
	DDX_Text(pDX, IDC_EDIT_FOLDER_COMMENT, m_strComment);
	DDX_Control(pDX, IDC_EDIT_FOLDER_COMMENT, m_ctrlCommentEditBox);
	DDX_Text(pDX, IDC_EDIT_CREATE_DATE, m_strCreateDate);
	DDX_Text(pDX, IDC_EDIT_MODIFY_DATE, m_strModifyDate);

	DDX_Control(pDX, IDC_LIST_DATAITEM, m_dataItemList);
	DDX_Radio(pDX, IDC_RADIO_VIEW_ALL, m_iViewType);
	DDX_Control(pDX, IDC_RADIO_VIEW_ALL, m_ctrlViewAllRadioButton);
	DDX_Control(pDX, IDC_RADIO_VIEW_PART, m_ctrlViewPartRadioButton);
	DDX_Control(pDX, IDC_RADIO_VIEW_TASK, m_ctrlViewTaskRadioButton);
}

BEGIN_MESSAGE_MAP(FolderContentPage, CLResizableDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATAITEM, OnDblclkList)

	ON_EN_KILLFOCUS(IDC_EDIT_FOLDER_NAME, OnKillName)
	ON_EN_KILLFOCUS(IDC_EDIT_FOLDER_COMMENT, OnKillComment)

	ON_BN_CLICKED(IDC_RADIO_VIEW_ALL, OnViewAll)
	ON_BN_CLICKED(IDC_RADIO_VIEW_PART, OnViewPart)
	ON_BN_CLICKED(IDC_RADIO_VIEW_TASK, OnViewTask)
END_MESSAGE_MAP()


BOOL FolderContentPage::OnInitDialog()
{
	CLResizableDialog::OnInitDialog();

	// layout controls.
	{
		// "basic info" group
		AddAnchor(IDC_STATIC_BASE_INFO, TOP_LEFT, TOP_RIGHT);
		// name
		AddAnchor(IDC_EDIT_FOLDER_NAME, TOP_LEFT);
		AddAnchor(IDC_STATIC_NAME, TOP_LEFT);
		// comment
		AddAnchor(IDC_EDIT_FOLDER_COMMENT, TOP_LEFT);
		AddAnchor(IDC_STATIC_COMMENT, TOP_LEFT);
		// date
		AddAnchor(IDC_STATIC_CREATE_DATE, TOP_LEFT);
		AddAnchor(IDC_EDIT_CREATE_DATE, TOP_LEFT);
		AddAnchor(IDC_STATIC_MODIFY_DATE, TOP_LEFT);
		AddAnchor(IDC_EDIT_MODIFY_DATE, TOP_LEFT);

		// "data items" group
		AddAnchor(IDC_STATIC_DATAITEM, TOP_LEFT, BOTTOM_RIGHT);
		// radio buttons.
		AddAnchor(IDC_RADIO_VIEW_ALL, TOP_CENTER);
		AddAnchor(IDC_RADIO_VIEW_PART, TOP_CENTER);
		AddAnchor(IDC_RADIO_VIEW_TASK, TOP_CENTER);
		//
		AddAnchor(IDC_LIST_DATAITEM, TOP_LEFT, BOTTOM_RIGHT);
	}

	InitGridStyle();

	SetBackgroundColor(SkinManager::GetBackgroundColor());

	SetWindowTheme(GetDlgItem(IDC_STATIC_BASE_INFO)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_STATIC_DATAITEM)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_RADIO_VIEW_ALL)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_RADIO_VIEW_PART)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_RADIO_VIEW_TASK)->GetSafeHwnd(), L"", L"");

	UpdateData(FALSE);

	return TRUE;
}

void FolderContentPage::OnDestroy()
{
	ReleaseGridItemData();
	CLResizableDialog::OnDestroy();
}

HBRUSH FolderContentPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return SkinManager::GetDialogCtrlColor(pDC, pWnd, nCtlColor);
}

void FolderContentPage::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// get the current row.
	int iItem = pNMLV->iItem;
	if (iItem == -1)
		return;

	// 发送消息让父窗口在数据中心中定位该节点。
	int iItemData = m_dataItemList.GetItemData(iItem);
	DataCenterNode* pDataCenterNode = (DataCenterNode*)iItemData;
	GetParent()->SendMessage(WM_DATA_ITEM_DB_CLICK, (WPARAM)pDataCenterNode, 0);
}

void FolderContentPage::OnKillName()
{
	UpdateData(TRUE);

	if (m_pParentFolder->GetName() != m_strName)
	{
		m_pParentFolder->SetName(m_strName);
		m_pParentFolder->SetModified(TRUE);
	}
}

void FolderContentPage::OnKillComment()
{
	UpdateData(TRUE);

	if (m_pParentFolder->GetComment() != m_strComment)
	{
		m_pParentFolder->SetComment(m_strComment);
		m_pParentFolder->SetModified(TRUE);
	}
}

FolderNodePtr FolderContentPage::GetFolderItem()
{
	SetFocus();
	UpdateData(TRUE);

	return m_pParentFolder;
}

void FolderContentPage::OnViewAll()
{
	InitGridData(VIEW_DATA_ALL);
}

void FolderContentPage::OnViewPart()
{
	InitGridData(VIEW_DATA_PART);
}

void FolderContentPage::OnViewTask()
{
	InitGridData(VIEW_DATA_TASK);
}

void FolderContentPage::DisplayDataItemList(FolderNodePtr pParentFolder, DataCenterItemListPtr pDataCenterItems)
{
	m_pParentFolder = pParentFolder;
	m_pDataCenterItems = pDataCenterItems;

	// init the basic and scheme data.
	{
		//
		m_strName = pParentFolder->GetName();
		m_strComment = pParentFolder->GetComment();
		if (m_pParentFolder->GetID() == WorkSpaceFolderID || m_pParentFolder->GetID() == PartsLibraryFolderID)
		{
			((CWnd*)GetDlgItem(IDC_EDIT_FOLDER_NAME))->EnableWindow(FALSE);
			((CWnd*)GetDlgItem(IDC_EDIT_FOLDER_COMMENT))->EnableWindow(FALSE);
		}
		else
		{
			((CWnd*)GetDlgItem(IDC_EDIT_FOLDER_NAME))->EnableWindow(TRUE);
			((CWnd*)GetDlgItem(IDC_EDIT_FOLDER_COMMENT))->EnableWindow(TRUE);
		}

		//
		m_strCreateDate = DateHelper::GetStrFromTimeStamp(pParentFolder->GetCreateTime());
		m_strModifyDate = DateHelper::GetStrFromTimeStamp(pParentFolder->GetModifyTime());
	}

	// init the view type.
	m_iViewType = 0;
	UpdateData(FALSE);

	//
	InitGridData();

	// we still not change anything yet.
	m_pParentFolder->SetModified(FALSE);
}

void FolderContentPage::UpdateModifyDate()
{
	m_strModifyDate = DateHelper::GetStrFromTimeStamp(m_pParentFolder->GetModifyTime());
	UpdateData(FALSE);
}

void FolderContentPage::InitGridStyle()
{
	// Requires one never uses column 0
	m_dataItemList.InsertHiddenLabelColumn();

	CString str;

	// "num" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NUM);
	CGridColumnTrait* pNUMCol = new CGridColumnTraitText;
	m_dataItemList.InsertColumnTrait(1, str, LVCFMT_LEFT, 40, 0, pNUMCol);

	// "name" column
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NAME);
	CGridColumnTrait* pNameCol = new CGridColumnTraitText;
	m_dataItemList.InsertColumnTrait(2, str, LVCFMT_LEFT, 150, 0, pNameCol);

	// node data type
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NODE_DATATYPE);
	CGridColumnTrait* pNodeTypeCol = new CGridColumnTraitText;
	m_dataItemList.InsertColumnTrait(3, str, LVCFMT_LEFT, 120, 0, pNodeTypeCol);

	//
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CREATE_DATE);
	CGridColumnTrait* pCreateDateCol = new CGridColumnTraitText;
	m_dataItemList.InsertColumnTrait(4, str, LVCFMT_LEFT, 120, 0, pCreateDateCol);

	//
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MODIFY_DATE);
	CGridColumnTrait* pModifyDateCol = new CGridColumnTraitText;
	m_dataItemList.InsertColumnTrait(5, str, LVCFMT_LEFT, 120, 0, pModifyDateCol);
}

void FolderContentPage::InitGridData(VIEW_DATA_OPTION viewOption)
{
	ReleaseGridItemData();
	m_dataItemList.DeleteAllItems();

	int iItem = 0;
	for (unsigned int i = 0; i < m_pDataCenterItems->size(); i++)
	{
		IDataCenterItemPtr pDataCenterItem = m_pDataCenterItems->at(i);
		DataItemPtr pDataItem = boost::dynamic_pointer_cast<DataItem>(pDataCenterItem);

		DATANODE_TYPE dataNodeType = pDataItem->GetDataType();
		if (viewOption == VIEW_DATA_PART && dataNodeType != DATANODE_PART)
			continue;
		else if (viewOption == VIEW_DATA_TASK && dataNodeType != DATANODE_Task)
			continue;

		if (pDataItem->GetName() == HiddenDataItemName) // 不显示隐藏的数据项。
			continue;

		// prepare something.
		CString strNodeType;
		DataCenterNode* pDataCenterNode(new DataCenterNode);
		if (pDataItem->GetDataType() == DATANODE_PART)
		{
			pDataCenterNode->SetNodeType(DataCenterNodeType_Part);
			strNodeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PART_NODE);
		}
		else if (pDataItem->GetDataType() == DATANODE_Task)
		{
			pDataCenterNode->SetNodeType(DataCenterNodeType_Task);
			strNodeType = MultiLanguageMgr::GetInstance()->TranslateString(IDS_Task_Node);
		}
		pDataCenterNode->SetDataCenterItemID(pDataItem->GetID());

		// insert a row.
		{
			CString strNum;
			strNum.Format(_T("%d"), iItem + 1);
			m_dataItemList.InsertItem(iItem, strNum);

			m_dataItemList.SetItemText(iItem, 1, strNum);
			m_dataItemList.SetItemText(iItem, 2, pDataItem->GetName());
			m_dataItemList.SetItemText(iItem, 3, strNodeType);

			// "create date" column.
			CString strTime = DateHelper::GetStrFromTimeStamp(pDataItem->GetCreateTime());
			m_dataItemList.SetItemText(iItem, 4, strTime);

			// "modify date" column.
			strTime = DateHelper::GetStrFromTimeStamp(pDataItem->GetModifyTime());
			m_dataItemList.SetItemText(iItem, 5, strTime);

			// store DataCenterNode object in item data.
			m_dataItemList.SetItemData(iItem, (int)pDataCenterNode);

			iItem++;
		}
	}

	// To be remembered.
	// notes:
	//   1) if put this in "InitGridStyle" function, when grid cannot display all rows(scroll bar will display), the column header will missing, so put it here.
	m_dataItemList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void FolderContentPage::ReleaseGridItemData()
{
	for (int i = 0; i < m_dataItemList.GetItemCount(); i++)
	{
		DWORD_PTR pItemData = m_dataItemList.GetItemData(i);
		delete (DataCenterNode*)pItemData;
	}
}

END_CUTLEADER_NAMESPACE()
