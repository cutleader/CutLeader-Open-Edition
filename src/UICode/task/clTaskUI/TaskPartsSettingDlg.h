#pragma once

#include "ClTaskUICommon.h"
#include "LineArc2DStaticControl.h"
#include "taoresource.h"
#include "CGridListCtrlGroups.h"
#include "CLDialog.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框用来编辑任务中的零件信息。
// 注：
//  1) 确定后比较任务数据和界面上的数据，对任务进行修改。
class ClTaskUI_Export TaskPartsSettingDlg : public CLDialog
{
	DECLARE_DYNAMIC(TaskPartsSettingDlg)

public:
	TaskPartsSettingDlg(CWnd* pParent = NULL);
	virtual ~TaskPartsSettingDlg();

	enum { IDD = IDD_clTaskUI_EditTaskPartInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBatchSetting();
	afx_msg void OnPartListchanged(NMHDR* pNMHDR, LRESULT* pResult);

    afx_msg void OnClickOk();
    afx_msg void OnClickCancel();

	DECLARE_MESSAGE_MAP()

public:
    void SetTaskItem(TaskItemPtr pTaskItem) { m_pTaskItem = pTaskItem; }

private:
	// 初始化“零件表格”的样式。
	void InitPartGridStyle();

	// 预览列表中选中的零件。
	void PreviewPart();

	// 释放“零件表格”中的数据。
	void RlsPartGridData();

private: // for controls
	CGridListCtrlGroups m_tableTaskParts;
	CStatic m_staticPartPreview;

	// use this brush to paint the background.
	CBrush m_brush;

	SkinMFCButton m_btnBatchSetting;
	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data member
    // “待排版零件”列表是否初始化完。
    BOOL m_bFinishAddNestPart;

    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
