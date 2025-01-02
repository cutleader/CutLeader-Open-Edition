#pragma once

#include "ClTaskCommon.h"
#include "XBarBase.h"
#include "TaskPartManagementDlg.h"
#include "TaskSheetManagementDlg.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartList)


BEGIN_CUTLEADER_NAMESPACE()

// �������Ի���
// ע��
//  1) ������ƶ�ʱ���öԻ����е�����Ͱ���ӶԻ���������֮ǰ��TrackMouseEvent�������ػ����뿪��Ϣ���Ӷ��ж��ӶԻ���ʧȥ���㡣
//     �������ַ�ʽ��̫�ȶ�����������ӶԻ����л�������С�ؼ�����༭�򡢰�ť����ЩС�ؼ�Ӱ����MouseLeave��Ϣ�Ŀɿ����͡�
//     Ŀǰ�ö�ʱ�������������磺��TaskPartInfoListDlg���˶�ʱ����Ψһ�Ŀ���ȱ�����������⣬��Ϊ��ʱ����һֱ������
//     �Ժ�Ҫ��������������ˣ���������TrackMouseEvent��������ʱ������֮���������԰Ѷ�ʱ��ʱ�����ó����Ͳ������������⡣
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

    // �������������Ϣ��
    void UpdateTaskPartsOnPanel();

    // ����ָ����ĵ�����ͼ��
    void RedrawSheetThumbnail(LONGLONG iSheetID);

	// �õ�ѡ�е������
	TaskPartListPtr GetSelectedTaskParts() const { return m_taskPartManagementDlg.GetSelectedTaskParts(); }

	// �õ�ָ���Ķ�����Ի���
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
