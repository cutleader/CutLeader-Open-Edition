#pragma once

#include "clBaseDataUICommon.h"
#include "taoresource.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(TaskEx)

BEGIN_CUTLEADER_NAMESPACE()

// 显示任务进度的对话框。
// 注:
//   1) 这个对话框中启动任务。
//   2) task should notify the dialog to update the progress.
class ClBaseDataUI_Export ProgressDlg : public CDialogEx
{
	DECLARE_DYNCREATE(ProgressDlg)

public:
	ProgressDlg();
	~ProgressDlg(void);

	enum { IDD = IDD_clBaseDataUI_Progress };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(CString strTitle, TaskExPtr pTask);

private:
	CProgressCtrl m_ctrlTaskProgress;
	CEdit m_ctrlSubTask;
	SkinMFCButton m_btnCancal;

private: // for data
	CString m_strTitle;

	// the task object.
	TaskExPtr m_pTask;
};

END_CUTLEADER_NAMESPACE()
