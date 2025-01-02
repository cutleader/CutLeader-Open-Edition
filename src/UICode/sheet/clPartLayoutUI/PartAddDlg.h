#pragma once

#include "clPartLayoutUICommon.h"
#include "DataProperty.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(DataCenterItemList)
DECLARE_CUTLEADER_CLASS(ParamConfig)

BEGIN_CUTLEADER_NAMESPACE()

class PartAddDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(PartAddDlg)

public:
	PartAddDlg();
	~PartAddDlg();

	enum { IDD = IDD_clPartLayoutUI_PartAdd };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSearchPart();
	afx_msg void OnAddToSheet();

	DECLARE_MESSAGE_MAP()

public:
	// call this to update this dialog.
	void DisplayPartList(FolderNodePtr pWorkspace);

private:
	// init the style of the list ctrl
	void InitListStyle();

	// 更新表格。
	void UpdatePartGrid(DataCenterItemListPtr pProductItemList);

	// 释放表格数据。
	void ReleaseGridData();

	// 预览选中的零件。
	void PreviewPart();

private: // controls

	/************************************************************************/
	// about scroll bar.

	// dialog size as you see in the resource view (original size)
	CRect m_rcOriginalRect;

	// actual scroll position
	int m_nScrollPos;

	// actual dialog height
	int m_nCurHeight;
	/************************************************************************/

	// 提示。
	CString m_strMsg;

	// available parts group.
	SkinGridListCtrlGroups m_partList;

	// 预览窗口。
	CStatic m_previewWnd;

	// 零件列表是否初始化完。
	BOOL m_bFinishAddPart;

	SkinMFCButton m_btnSeach;
	SkinMFCButton m_btnAdd;

private: // data member.
	FolderNodePtr m_pWorkspace;
};

END_CUTLEADER_NAMESPACE()
