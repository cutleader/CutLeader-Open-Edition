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
    // ������ʾ�öԻ���
    void Redisplay_TaskSheetInfoListDlg(TaskItemPtr pTaskItem);

    std::vector<TaskSheetInfoDlgPtr> GetSheetSubDlgs() const { return m_taskSheetInfoDlgList; }

private:
	// �õ��ɼ����Ӵ����������ڸöԻ�����ʾ���֮ǰ����������������а������������һ����ȡ�
	unsigned int GetVisibleSubWindowCount() const;

private: // controls
    // ������ʾÿ����ĵ��ӶԻ����ӶԻ���ĸ������ܱȰ��Ҫ�࣬��ʾ�����������Ͱ������һ��������ı����������ˡ�
    std::vector<TaskSheetInfoDlgPtr> m_taskSheetInfoDlgList;

    int m_nTimer;

	// ������������
	CScrollHelper* m_scrollHelper;

private: // data member.
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
