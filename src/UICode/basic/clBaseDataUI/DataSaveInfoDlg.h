#pragma once

#include "clBaseDataUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(IDataCenterItem)

BEGIN_CUTLEADER_NAMESPACE()

// 保存新数据时，用这个对话框选择保存目录和名称。
class ClBaseDataUI_Export DataSaveInfoDlg : public CLDialog
{
	DECLARE_DYNAMIC(DataSaveInfoDlg)

public:
	DataSaveInfoDlg(CWnd* pParent = NULL);
	virtual ~DataSaveInfoDlg();

	// Dialog Data
	enum { IDD = IDD_clBaseDataUI_SaveData };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(CString strTitle, FolderNodePtr pWorkspace, CString strName);

	FolderNodePtr GetSelectFolder() { return m_pSelFolderNode; }
	CString GetName() { return m_strName; }

private:
	// init the product tree.
	void InitTree();

	// recursive to append "pProductItemList" under "hParentNode".
	void AppendSubNode(DataCenterItemListPtr pProductItemList, HTREEITEM hParentNode);

	// whether there are folders under this folder node.
	BOOL HasFolder(IDataCenterItemPtr pFolderItem);

	// release item data in tree.
	void ReleaseNodeData(HTREEITEM hParentItem);

private: // for control.
	CString m_strName;

	// the tree control.
	SkinTreeCtrl m_productTree;

	// the image list.
	CImageList* m_pImageList;

	SkinMFCButton m_btnOk;
	SkinMFCButton m_btnCancel;

private: // for data
	CString m_strTitle;
	FolderNodePtr m_pWorkspace;

	// the selected node.
	FolderNodePtr m_pSelFolderNode;
};

END_CUTLEADER_NAMESPACE()
