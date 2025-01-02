#pragma once

#include "ClTaskUICommon.h"
#include "CLResizableDialog.h"
#include "taoResource.h"
#include "ScrollHelper.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartList)
DECLARE_CUTLEADER_CLASS(TaskPartInfoDlg)


BEGIN_CUTLEADER_NAMESPACE()

class ClTaskUI_Export TaskPartInfoListDlg : public CLResizableDialog
{
	DECLARE_DYNCREATE(TaskPartInfoListDlg)

public:
	TaskPartInfoListDlg();
	~TaskPartInfoListDlg();

	enum { IDD = IDD_clTaskUI_TaskPartInfoList };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
    afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short iDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()

public:
    // 重新显示该对话框。
    void Redisplay_TaskPartInfoListDlg(TaskItemPtr pTaskItem);

	// 刷新显示所有零件的已排数量。这个函数会比较界面上的数字和数据结构中的数值，只重新显示变化的数值。
	void ReDisplayNestedCount_ofEachPart();

    std::vector<TaskPartInfoDlgPtr> GetTaskPartSubDlgs() const { return m_taskPartInfoDlgList; }

	// 销毁掉所有子窗口。
	void DestroyAllSubWindows() { m_taskPartInfoDlgList.clear(); }

private:
	// 得到可见的子窗口数量，在该对话框显示完成之前，这个数量和任务中零件种类数量不一定相等。
	unsigned int GetVisibleSubWindowCount() const;

private: // controls
	// use this brush to paint the background.
	CBrush m_brush;

    // 用来显示每个任务零件的子对话框。子对话框的个数可能比零件要多，显示出来的数量和零件数量一样，其余的被隐藏起来了。
    std::vector<TaskPartInfoDlgPtr> m_taskPartInfoDlgList;

    // 当没有零件可显示时，用这个字体显示提示信息。
    CFont m_msgFont;

    int m_nTimer;

	// 滚动条助手类
	CScrollHelper* m_scrollHelper;

private: // data member.
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
