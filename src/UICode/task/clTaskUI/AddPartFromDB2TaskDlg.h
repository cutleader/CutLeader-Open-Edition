#pragma once

#include "ClTaskUICommon.h"
#include "taoresource.h"
#include "CGridListCtrlGroups.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(TaskItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClTaskUI_Export AddPartFromDB2TaskDlg : public CLDialog
{
	DECLARE_DYNAMIC(AddPartFromDB2TaskDlg)

public:
	AddPartFromDB2TaskDlg(CWnd* pParent = NULL);
	virtual ~AddPartFromDB2TaskDlg();

	enum { IDD = IDD_clTaskUI_AddPartFromDB2Task };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	afx_msg void OnCheckFolderFilter();
	afx_msg void OnCheckNameFilter();
	afx_msg void OnSelectFilterFolder();
	afx_msg void OnSearch();

	afx_msg void OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAll();

	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

public:
	void Init(FolderNodePtr pWorkspace) { m_pFilterFolder = pWorkspace; }

	// 得到选中的零件。
	IntegerListPtr GetSelectedPart() { return m_pSelPartIDList; }

    void SetTaskItem(TaskItemPtr pTaskItem) { m_pTaskItem = pTaskItem; }

private:
	// 初始化表格样式。
	void InitListStyle();

	// 初始化表格数据。
	void InitListData();

	// 释放表格中的数据。
	void ReleaseGridData();

	// 预览列表中选中的零件。
	void PreviewPart();

private: // for controls
	// 目录。
	BOOL m_bFolderFilter;

	// 名称
	BOOL m_bNameFilter;
	CString m_strNameFilter;

	// 零件列表。
	CGridListCtrlGroups m_partList;

	// 零件预览窗口。
	CStatic m_partPreviewWnd;

	// 是否全选。
	BOOL m_bSelectAll;

	SkinMFCButton m_btnSearch;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data member
	// 搜索的位置。
	FolderNodePtr m_pFilterFolder;

	// use this brush to paint the background.
	CBrush m_brush;

	// 零件列表是否初始化完。
	BOOL m_bFinishAddPart;

	// 选中的零件。
	IntegerListPtr m_pSelPartIDList;

    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
