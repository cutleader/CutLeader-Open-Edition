#pragma once

#include "ClTaskUICommon.h"
#include "CGridListCtrlGroups.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "TaskSheetInfoListDlg.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

class ClTaskUI_Export TaskSheetManagementDlg : public CLResizableDialog
{
    DECLARE_DYNCREATE(TaskSheetManagementDlg)

public:
    TaskSheetManagementDlg();
    ~TaskSheetManagementDlg();

    enum { IDD = IDD_clTaskUI_TaskSheetManagement };

protected: // inherit from the super class.
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

protected: // message mapping.
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnSelectOrUnSelectSheetInTaskSheetDlg(WPARAM wParam, LPARAM lParam);

    afx_msg void OnNewSheet2Task();
    afx_msg void OnDeleteSheetFromTask();

    DECLARE_MESSAGE_MAP()

public:
    // 重新显示该对话框的数据。
    void Redisplay_TaskSheetManagementDlg(TaskItemPtr pTaskItem);

    // 更新指定板材的缩略图。
    void RedrawSheetThumbnail(LONGLONG iSheetID);

	const TaskSheetInfoListDlg* GetTaskSheetInfoListDlg() const { return &m_taskSheetInfoListDlg; }

	TaskItemPtr GetTaskItem() const;

private: // controls
    CBrush m_brush;
    TaskSheetInfoListDlg m_taskSheetInfoListDlg;

	SkinMFCButton m_btnAddSheet;
	SkinMFCButton m_btnDeleteSheet;

private: // data member.
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
