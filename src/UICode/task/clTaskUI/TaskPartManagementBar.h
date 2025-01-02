#pragma once

#include "ClTaskUICommon.h"
#include "XBarBase.h"
#include "TaskPartManagementDlg.h"
#include "TaskSheetManagementDlg.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartList)


BEGIN_CUTLEADER_NAMESPACE()

// “任务零件”管理面板。
// 注：
//  1) “任务板材”管理面板也使用类似机制。当鼠标移动时，该面板中的零件子对话框会高亮。之前用TrackMouseEvent函数来截获光标离开消息，从而判断子对话框失去焦点。
//     不过这种方式不太稳定，比如零件子对话框中还有其他小控件，如编辑框、按钮，这些小控件影响了MouseLeave消息的可靠发送。
//     目前用定时器技术来做，如：在TaskPartInfoListDlg加了定时器，唯一的可能缺点是性能问题，因为定时器会一直开启。
//     以后要是这个问题严重了，还可以用TrackMouseEvent来做，定时器辅助之，这样可以把定时器时间设置长，就不会有性能问题。
class ClTaskUI_Export TaskPartManagementBar : public XBarBase
{
private:
	TaskPartManagementBar(void);

public:
	~TaskPartManagementBar(void);

public:
	virtual BOOL CanBeClosed() const { return FALSE; }
	virtual BOOL IsDragMode() const { return FALSE; }
	virtual void ShowBar(BOOL bShow, BOOL bDelay = FALSE) override;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	static TaskPartManagementBar* GetInstance(CWnd* pParent = NULL);

public:
	TaskPartManagementDlg* GetTaskPartManagementDlg() { return &m_taskPartManagementDlg; }

	// 得到指定的豆腐块对话框。
	CWnd* GetTaskPartInfoDlg(int iDialogIndex);

private:
	// the singleton object
	static TaskPartManagementBar *m_pTaskPartManagementBar;

private:
    TaskPartManagementDlg m_taskPartManagementDlg;
};

END_CUTLEADER_NAMESPACE()
