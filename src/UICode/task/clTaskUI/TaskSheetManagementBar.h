#pragma once

#include "ClTaskUICommon.h"
#include "XBarBase.h"
#include "TaskPartManagementDlg.h"
#include "TaskSheetManagementDlg.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(TaskPartList)


BEGIN_CUTLEADER_NAMESPACE()

// �������ġ�������塣
class ClTaskUI_Export TaskSheetManagementBar : public XBarBase
{
private:
	TaskSheetManagementBar(void);

public:
	~TaskSheetManagementBar(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	static TaskSheetManagementBar* GetInstance(CWnd* pParent = NULL);

public:
	TaskSheetManagementDlg* GetTaskSheetManagementDlg() { return &m_taskSheetManagementDlg; }

	// �õ�ָ���Ķ�����Ի���
	CWnd* GetTaskSheetInfoDlg(int iDialogIndex);

private:
	// the singleton object
	static TaskSheetManagementBar *m_pInteractBar;

private:
    TaskSheetManagementDlg m_taskSheetManagementDlg;
};

END_CUTLEADER_NAMESPACE()
