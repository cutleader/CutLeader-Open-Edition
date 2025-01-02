#pragma once

#include "ClDataManagerCommon.h"
#include "DataProperty.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)


BEGIN_CUTLEADER_NAMESPACE()


//
enum VIEW_DATA_OPTION
{
	VIEW_DATA_ALL = 0,
	VIEW_DATA_PART = 1,
	VIEW_DATA_TASK = 2,
};

// this page will display the content of the folder.
class ClDataManager_Export FolderContentPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(FolderContentPage)

public:
	FolderContentPage();
	~FolderContentPage();

	enum { IDD = IDD_clDataManagerUI_FolderContent };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnKillName();
	afx_msg void OnKillComment();

	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);

	// view type
	afx_msg void OnViewAll();
	afx_msg void OnViewPart();
	afx_msg void OnViewTask();

	DECLARE_MESSAGE_MAP()

public:
	// 返回修改过的数据（当然可能没有修改）。
	// 注：
	// 1) 该函数需要把焦点设到本对话框，以确保编辑框能失去焦点。
	// 2）由于在回放脚本时没有给编辑框发送killFocus消息，该函数需要把编辑框的值更新到数据中。
	FolderNodePtr GetFolderItem();

public:
	// display data node list.
	void DisplayDataItemList(FolderNodePtr pParentFolder, DataCenterItemListPtr pDataCenterItems);

	// update the modify date.
	// notes:
	//   1) get the modify date from FolderNode object.
	void UpdateModifyDate();

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// init the content of the list ctrl.
	void InitGridData(VIEW_DATA_OPTION viewOption = VIEW_DATA_ALL);

	// release the data which stored in grid.
	void ReleaseGridItemData();

private: // controls
	// basic info group.
	CString m_strName;
	CEdit m_ctrlNameEditBox;
	CString m_strComment;
	CEdit m_ctrlCommentEditBox;
	CString m_strCreateDate;
	CString m_strModifyDate;

	SkinGridListCtrlGroups m_dataItemList;
	int m_iViewType;
	CButton m_ctrlViewAllRadioButton;
	CButton m_ctrlViewPartRadioButton;
	CButton m_ctrlViewTaskRadioButton;

	CBrush m_brush;

private: // data member.
	//
	FolderNodePtr m_pParentFolder;

	// data items under "m_pParentFolder".
	DataCenterItemListPtr m_pDataCenterItems;
};

END_CUTLEADER_NAMESPACE()
