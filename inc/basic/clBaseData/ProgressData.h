#pragma once

#include "clBaseDataCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

#define WM_UPDATE_PROGRESS	WM_USER + 0x20C1

// 任务的进度信息。
// 注：
//  1) 任务对象和进度对话框中都要访问这个信息，访问时要考虑同步。
class ClBaseData_Export ProgressData
{
public:
	ProgressData(double dTaskPercent, CString strSubTask = _T(""));
	~ProgressData(void);

public: // set/set functions.
	void SetTaskPercent(double dTaskPercent) { m_dTaskPercent = dTaskPercent; }
	double GetTaskPercent() const { return m_dTaskPercent; }

	void SetSubTask(CString strSubTask) { m_strSubTask = strSubTask; }
	CString GetSubTask() { return m_strSubTask; }

	void SetProgressDataMtx(HANDLE hProgressDataMtx) { m_hProgressDataMtx = hProgressDataMtx; }
	HANDLE GetProgressDataMtx() { return m_hProgressDataMtx; }

	void IsFinished(BOOL bFinished) { m_bFinished = bFinished; }
	BOOL IsFinished() { return m_bFinished; }

	void IsCanceled(BOOL bCanceled) { m_bCanceled = bCanceled; }
	BOOL IsCanceled() { return m_bCanceled; }

private:
	// the task percent, this will be within 0 to 100.
	double m_dTaskPercent;

	// the sub task desc.
	// notes:
	//   1) usually this is displayed in the multi-line text box.
	CString m_strSubTask;

	// the mutex which will protect the progress data.
	HANDLE m_hProgressDataMtx;

	// whether the task is finished.
	BOOL m_bFinished;

	// whether the task is canceled.
	// notes:
	//   1) this indicate user click the cancel button, the task thread should set 
	//      the progress as finished based on this.
	BOOL m_bCanceled;
};

END_CUTLEADER_NAMESPACE()
