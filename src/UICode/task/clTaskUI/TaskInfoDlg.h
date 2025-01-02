#pragma once

#include "ClTaskUICommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "SkinManager.h"


DECLARE_CUTLEADER_CLASS(TaskItem)

BEGIN_CUTLEADER_NAMESPACE()

// use this dialog to create task.
class ClTaskUI_Export TaskInfoDlg : public CLDialog
{
	DECLARE_DYNAMIC(TaskInfoDlg)

public:
	TaskInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~TaskInfoDlg();

	enum { IDD = IDD_clTaskUI_TaskInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelectCamParam();
	afx_msg void OnSelectMat();

	DECLARE_MESSAGE_MAP()

public:
	// get the new task.
	TaskItemPtr GetTaskItem();

private: // for controls
	CString m_strTaskName;

	// Material Info group.
	CString m_strMat;
	CString m_strMatThick;
	CString m_strMatSize;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // data members
	LONGLONG m_iParamConfigID;
	LONGLONG m_iMatSizeID;
};

END_CUTLEADER_NAMESPACE()
