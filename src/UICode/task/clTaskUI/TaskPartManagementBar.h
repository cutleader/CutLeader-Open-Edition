#pragma once

#include "ClTaskUICommon.h"
#include "XBarBase.h"
#include "TaskPartManagementDlg.h"
#include "TaskSheetManagementDlg.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartList)


BEGIN_CUTLEADER_NAMESPACE()

// �����������������塣
// ע��
//  1) �������ġ��������Ҳʹ�����ƻ��ơ�������ƶ�ʱ��������е�����ӶԻ���������֮ǰ��TrackMouseEvent�������ػ����뿪��Ϣ���Ӷ��ж��ӶԻ���ʧȥ���㡣
//     �������ַ�ʽ��̫�ȶ�����������ӶԻ����л�������С�ؼ�����༭�򡢰�ť����ЩС�ؼ�Ӱ����MouseLeave��Ϣ�Ŀɿ����͡�
//     Ŀǰ�ö�ʱ�������������磺��TaskPartInfoListDlg���˶�ʱ����Ψһ�Ŀ���ȱ�����������⣬��Ϊ��ʱ����һֱ������
//     �Ժ�Ҫ��������������ˣ���������TrackMouseEvent��������ʱ������֮���������԰Ѷ�ʱ��ʱ�����ó����Ͳ������������⡣
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

	// �õ�ָ���Ķ�����Ի���
	CWnd* GetTaskPartInfoDlg(int iDialogIndex);

private:
	// the singleton object
	static TaskPartManagementBar *m_pTaskPartManagementBar;

private:
    TaskPartManagementDlg m_taskPartManagementDlg;
};

END_CUTLEADER_NAMESPACE()
