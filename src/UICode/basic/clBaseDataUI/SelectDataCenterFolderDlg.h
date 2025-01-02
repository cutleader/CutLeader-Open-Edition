#pragma once

#include "clBaseDataUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(IDataCenterItem)

BEGIN_CUTLEADER_NAMESPACE()

// 用这个对话框来选择数据中心的一个目录。
class ClBaseDataUI_Export SelectDataCenterFolderDlg : public CLDialog
{
	DECLARE_DYNAMIC(SelectDataCenterFolderDlg)

public:
	SelectDataCenterFolderDlg(CWnd* pParent = NULL);
	virtual ~SelectDataCenterFolderDlg();

	// Dialog Data
	enum { IDD = IDD_clBaseDataUI_SelectDataCenterFolder };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnDestroy();
	afx_msg void OnCreateFolder();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	FolderNodePtr GetSelectFolder() { return m_pSelFolderNode; }

	// 初始化要定位到的目录节点
	void SetInitialFolderNode(LONGLONG iInitialFolderNodeID) { m_iInitialFolderNodeID = iInitialFolderNodeID; }

private:
	// init the product tree.
	void Init();

	// recursive to append "pProductItemList" under "hParentNode".
	void AppendSubNode(DataCenterItemListPtr pProductItemList, HTREEITEM hParentNode);

	// whether there are folders under this folder node.
	BOOL HasFolder(IDataCenterItemPtr pFolderItem);

	// release item data in tree.
	void ReleaseNodeData(HTREEITEM hParentItem);

	// 定位到指定的目录节点，这个函数先比较hItem，再比较hItem的孩子节点。
	bool LocateFolderNode(HTREEITEM hItem, LONGLONG iFolderNodeID);

private: // for control.
	// the tree control.
	SkinTreeCtrl m_dataCenterTree;

	// the image list.
	CImageList* m_pImageList;

private: // for data
	FolderNodePtr m_pWorkspace;
	LONGLONG m_iInitialFolderNodeID;

	// the selected node.
	FolderNodePtr m_pSelFolderNode;

	SkinMFCButton m_btnNewFolder;
	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;
};

END_CUTLEADER_NAMESPACE()
