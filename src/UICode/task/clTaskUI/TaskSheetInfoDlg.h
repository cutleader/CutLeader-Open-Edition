#pragma once

#include "ClTaskUICommon.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "XLabel.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

class TaskSheetInfoDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskSheetInfoDlg)

public:
	TaskSheetInfoDlg();
	~TaskSheetInfoDlg();

	enum { IDD = IDD_clTaskUI_SheetInfo };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
    virtual void OnOK() { return; }
    virtual void OnCancel() { return; }

protected: // message mapping.
    afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    afx_msg void OnSelectSheet();
    afx_msg void OnActiveSheet();

	afx_msg LRESULT OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKillFocusEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
    SheetPtr GetSheet() const { return m_pSheet; }

public:
    void Init(SheetPtr pSheet, TaskItemPtr pTaskItem);

	// 重置数据。
	// 注：
	// 1) 场景1：关闭某个任务后，下个任务可能会隐藏一些TaskSheetInfoDlg，在隐藏后还要重置TaskSheetInfoDlg中的共享数据，
	//    否则可能导致刚刚关闭的任务对象的内存无法释放，从而造成内存泄露。
	void ResetData();

    bool IsSelected() const;
    void SelectThisSheet();
    void UnSelectThisSheet();

    void DisplayDataOnDialog();

private: // controls
    CStatic m_previewWnd;
	XLabel m_ctrlSheetNameLabel;
    CNumEdit m_countEditBox;

	// 当前对话框是不是活动的，活动的话会有一个高亮的外框，鼠标进入本对话框就设置为true，离开即设置为false。
	bool m_bThisDialogIsActive;

private: // data member.
    SheetPtr m_pSheet;
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
