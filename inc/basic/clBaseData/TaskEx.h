#pragma once

#include "clBaseDataCommon.h"

DECLARE_CUTLEADER_CLASS(ProgressData)

BEGIN_CUTLEADER_NAMESPACE()

// 任务。
class ClBaseData_Export TaskEx
{
public:
	TaskEx(void);
	virtual ~TaskEx(void);

public: // virtual functions.
	virtual void Run() {}

public: // get/set functions.
	void SetProgressWnd(HWND hProgressWnd) { m_hProgressWnd = hProgressWnd; }
	ProgressDataPtr GetProgressData() const { return m_pProgressData; }

public:
	// start the task.
	BOOL Start();

protected:
	// the low-level thread object
	HANDLE m_hThread;

	// the handle of the progress window.
	HWND m_hProgressWnd;

	// 任务的进度信息。
	ProgressDataPtr m_pProgressData;
};

DWORD WINAPI ThreadProc(LPVOID pParam);

END_CUTLEADER_NAMESPACE()
