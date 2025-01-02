#pragma once

#include "ClTaskUICommon.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "ScrollHelper.h"
#include <vector>
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(TaskSheetInfoDlg)


BEGIN_CUTLEADER_NAMESPACE()

class TaskSheetInfoListDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskSheetInfoListDlg)

public:
	TaskSheetInfoListDlg();
	~TaskSheetInfoListDlg();

	enum { IDD = IDD_clTaskUI_SheetInfoList };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
    afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short iDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()

public:
    // 重新显示该对话框。
    void Redisplay_TaskSheetInfoListDlg(TaskItemPtr pTaskItem);

    std::vector<TaskSheetInfoDlgPtr> GetSheetSubDlgs() const { return m_taskSheetInfoDlgList; }

private:
	// 得到可见的子窗口数量，在该对话框显示完成之前，这个数量和任务中板材种类数量不一定相等。
	unsigned int GetVisibleSubWindowCount() const;

private: // controls
    // 用来显示每个板材的子对话框。子对话框的个数可能比板材要多，显示出来的数量和板材数量一样，其余的被隐藏起来了。
    std::vector<TaskSheetInfoDlgPtr> m_taskSheetInfoDlgList;

    int m_nTimer;

	// 滚动条助手类
	CScrollHelper* m_scrollHelper;

private: // data member.
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
