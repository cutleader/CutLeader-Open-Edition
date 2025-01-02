#pragma once

#include "ClDataManagerCommon.h"
#include "taoresource.h"
#include "CLResizableDialog.h"
#include "InvCellGrid.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(ImgData)


BEGIN_CUTLEADER_NAMESPACE()


class ClDataManager_Export TaskInfoPage : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskInfoPage)

public:
	TaskInfoPage();
	~TaskInfoPage();

	enum { IDD = IDD_ClDataManagerUI_TaskInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	virtual void OnOK() { return; }
	virtual void OnCancel() { return; }

protected: // message mapping.
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnKillTaskName();
	afx_msg void OnKillTaskComment();

    afx_msg void OnSelchangedPart(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangedSheet(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnViewSelectedPart();
	afx_msg void OnChangeMaterial();
	afx_msg void OnChangeCamParam();
    afx_msg void OnExportSheetAsDxf();
	afx_msg void OnExportAllSheetsAsDxf();

	DECLARE_MESSAGE_MAP()

public:
	// 返回修改过的数据（当然可能没有修改）。
	// 注：
	// 1) 该函数需要把焦点设到本对话框，以确保编辑框能失去焦点。
	// 2）由于在回放脚本时没有给编辑框发送killFocus消息，该函数需要把编辑框的值更新到数据中。
	TaskItemPtr GetTaskItem();

public:
	void DisplayTaskItem(TaskItemPtr pTaskItem);
	void UpdateModifyDate();

private:
	// init the column header of the list ctrl.
	void InitGridStyle();

	// 在表格中显示内容。
	void InitGridData();

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

	// 预览窗口
	CStatic m_partPreviewWnd;
    CStatic m_sheetPreviewWnd;

	// 表格控件
	SkinInvCellGrid m_partGrid;
	InvCellGrid m_sheetGrid;

	SkinMFCButton m_ctrlViewSelectedPartButton;
	SkinMFCButton m_ctrlExportSheetButton;
	SkinMFCButton m_ctrlExportAllSheetsButton;
	SkinMFCButton m_ctrlChangeMaterialButton;
	SkinMFCButton m_ctrlSwitchCAMParamButton;

	// use this brush to paint the background.
	CBrush m_brush;

private: // data member
	TaskItemPtr m_pTaskItem;

    // 是否禁止表格的“选择变化”消息。
    bool m_bDisableSelectChangeMessageOfGrid;

    // 预览数据。
    ImgDataPtr m_pPartImgData;
    ImgDataPtr m_pSheetImgData;
};

END_CUTLEADER_NAMESPACE()
