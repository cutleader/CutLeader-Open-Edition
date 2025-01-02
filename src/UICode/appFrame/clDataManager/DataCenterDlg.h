#pragma once

#include "clDataManagerCommon.h"
#include "DataCenterNode.h"
#include "DataCenterTree.h"
#include "DataCenterHelper.h"
#include "FolderContentPage.h"
#include "PartInfoPage.h"
#include "ItemListPage.h"
#include "TaskInfoPage.h"


BEGIN_CUTLEADER_NAMESPACE()

// the resource explorer view.
class ClDataManager_Export DataCenterDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(DataCenterDlg)

public:
	DataCenterDlg(CWnd* pParent = NULL);
	virtual ~DataCenterDlg();

	enum { IDD = IDD_clDataManager_Explorer };

public: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// handle the message for tree.
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTreeNodeMoved(WPARAM wParam, LPARAM lParam); // ClTreeCtrl object will move the tree node, so this class will care all other processes.

	// handler of the menu items.
	afx_msg void OnCreateFolder();
	afx_msg void OnImportClTask();
	afx_msg void OnImportClPart();
	afx_msg void OnImportDxfDwg();
	afx_msg void OnExportOneBusinessData();
	afx_msg void OnDeleteItem();

	// FolderContentPage send WM_DATA_ITEM_DB_CLICK message to notify this function that we should navigate to the DataNode.
	afx_msg LRESULT OnDataItemDbClick(WPARAM wParam, LPARAM lParam);

	afx_msg void OnExplorerOpen();
	afx_msg void OnExplorerSave();
	afx_msg void OnExplorerCancel();

	DECLARE_MESSAGE_MAP()

public: // get/set functions.
	void InitNaviNodeID(LONGLONG iNaviNodeID) { m_iNaviNodeID = iNaviNodeID; }

	DataItemPtr GetOpeningDataItem() { return m_pOpeningDataItem; }

public:
	// save the content of the current page.
	// notes:
	//   1) if no changes made, do not save.
	void SaveCurrentPage();

	// check whether some page is modified in resource explorer.
	BOOL DocModified();

private:
	/************************************************************************/
	// about sub-pages.

	// init sub-dialogs, these dialogs will be shown
	// when user select the proper tree node.
	void InitSubDialog();

	// hide all sub-dialogs.
	void HideAllSubDialog();

	// hide all sub pages and display the proper page according to 
	// "m_expLibNodeType" on the proper position.
	void RepositionSubDlg();
	/************************************************************************/


	// save modifications that were made to "hItem".
	/* params:
	*  bNeedConfirm: whether need user to confirm save.
	*/
	// notes:
	//   1) if no modifications made to "hItem", return FALSE.
	BOOL SaveModification(HTREEITEM hItem, BOOL bNeedConfirm = TRUE);

	// open the data item.
	void OpenDataItem(DataItemPtr pDataItem);

private: // for UI.
	/************************************************************************/
	// for the product tree.

	// the tree control.
	DataCenterTree m_dataCenterTree;

	// use this ID to navigate a node.
	// notes:
	//   1) if "m_iNaviNodeID" is INVALID_ID, navigate to the root node.
	LONGLONG m_iNaviNodeID;
	/************************************************************************/


	/************************************************************************/
	// the sub pages in this sheet.

	// this page display the content of the folder node.
	FolderContentPage m_folderContentPage;

	// this page display the content of the part node.
	PartInfoPage m_partInfoPage;

	// this page display the content of the task node.
	TaskInfoPage m_taskInfoPage;

	// this page display the content of the selected nodes.
	ItemListPage m_itemListPage;
	/************************************************************************/


	SkinMFCButton m_btnExplorerOpen;
	SkinMFCButton m_btnExplorerSave;
	SkinMFCButton m_btnExplorerCancel;

private: // data member
	// indicate which node has been selected.
	DataCenterNodeType m_productNodeType;

	// the data item which will be opened.
	DataItemPtr m_pOpeningDataItem;

	// the workspace node, we can search any node through this node.
	FolderNodePtr m_pWorkspace;

	// the helper class.
	DataCenterHelper m_dataCenterHelper;

	// use this brush to paint the background.
	CBrush m_brush;
};

END_CUTLEADER_NAMESPACE()
