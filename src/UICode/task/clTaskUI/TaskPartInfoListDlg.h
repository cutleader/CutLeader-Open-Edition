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
    // ������ʾ�öԻ���
    void Redisplay_TaskPartInfoListDlg(TaskItemPtr pTaskItem);

	// ˢ����ʾ����������������������������ȽϽ����ϵ����ֺ����ݽṹ�е���ֵ��ֻ������ʾ�仯����ֵ��
	void ReDisplayNestedCount_ofEachPart();

    std::vector<TaskPartInfoDlgPtr> GetTaskPartSubDlgs() const { return m_taskPartInfoDlgList; }

	// ���ٵ������Ӵ��ڡ�
	void DestroyAllSubWindows() { m_taskPartInfoDlgList.clear(); }

private:
	// �õ��ɼ����Ӵ����������ڸöԻ�����ʾ���֮ǰ������������������������������һ����ȡ�
	unsigned int GetVisibleSubWindowCount() const;

private: // controls
	// use this brush to paint the background.
	CBrush m_brush;

    // ������ʾÿ������������ӶԻ����ӶԻ���ĸ������ܱ����Ҫ�࣬��ʾ�������������������һ��������ı����������ˡ�
    std::vector<TaskPartInfoDlgPtr> m_taskPartInfoDlgList;

    // ��û���������ʾʱ�������������ʾ��ʾ��Ϣ��
    CFont m_msgFont;

    int m_nTimer;

	// ������������
	CScrollHelper* m_scrollHelper;

private: // data member.
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE()
