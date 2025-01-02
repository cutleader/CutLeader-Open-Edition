#pragma once

#include "ClTaskCommon.h"
#include "XBarBase.h"
#include "TaskPartManagementDlg.h"
#include "TaskSheetManagementDlg.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartList)


BEGIN_CUTLEADER_NAMESPACE()

// 任务管理对话框。
// 注：
//  1) 当鼠标移动时，该对话框中的零件和板材子对话框会高亮。之前用TrackMouseEvent函数来截获光标离开消息，从而判断子对话框失去焦点。
//     不过这种方式不太稳定，比如零件子对话框中还有其他小控件，如编辑框、按钮，这些小控件影响了MouseLeave消息的可靠发送。
//     目前用定时器技术来做，如：在TaskPartInfoListDlg加了定时器，唯一的可能缺点是性能问题，因为定时器会一直开启。
//     以后要是这个问题严重了，还可以用TrackMouseEvent来做，定时器辅助之，这样可以把定时器时间设置长，就不会有性能问题。
class ClTask_Export TaskManagementBar : public XBarBase
{
private:
	TaskManagementBar(void);

public:
	~TaskManagementBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnReDisplayTaskParts(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnReDisplayTaskSheets(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	static TaskManagementBar* GetInstance(CWnd* pParent = NULL);

public:
    void UpdateTaskItem(TaskItemPtr pTaskItem);

    TaskItemPtr GetTaskItem() const;

    // 更新任务零件信息。
    void UpdateTaskPartsOnPanel();

    // 更新指定板材的缩略图。
    void RedrawSheetThumbnail(LONGLONG iSheetID);

	// 得到选中的零件。
	TaskPartListPtr GetSelectedTaskParts() const { return m_taskPartManagementDlg.GetSelectedTaskParts(); }

	// 得到指定的豆腐块对话框。
	CWnd* GetTaskPartInfoDlg(int iDialogIndex);
	CWnd* GetTaskSheetInfoDlg(int iDialogIndex);

private:
	// the singleton object
	static TaskManagementBar *m_pInteractBar;

private:
    TaskPartManagementDlg m_taskPartManagementDlg;
    TaskSheetManagementDlg m_taskSheetManagementDlg;
};

END_CUTLEADER_NAMESPACE()
