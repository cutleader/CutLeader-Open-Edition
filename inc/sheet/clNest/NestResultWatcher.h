#pragma once

#include "clNestCommon.h"
#include "NestResult.h"

DECLARE_CUTLEADER_CLASS(NestResult)
DECLARE_CUTLEADER_CLASS(NestProcessor)

BEGIN_CUTLEADER_NAMESPACE()

// 一个简单的提醒线程，负责通知主线程更新进度。
class ClNest_Export NestResultWatcher
{
public:
	NestResultWatcher(void);
	~NestResultWatcher(void);

public:
	// init the watcher.
	void Init(NestProcessorPtr pNestProcessor, NestResultPtr pNestResult, int iAllowedNestTime);
	
	// start the task.
	BOOL Start();
	
	// terminate the task.
	void Terminate();
	
	// the working process.
	void Run();
	
protected:
	// the low-level thread object
	HANDLE m_hThread;

	// the nesting processor.
	NestProcessorPtr m_pNestProcessor;

	// the nesting result.
	NestResultPtr m_pNestResult;
	
	// the start tick count.
	int m_iStartTickCount;
	
	// the allowed nesting time(s).
	int m_iAllowedNestTime;
};

DWORD WINAPI ThreadProc_4_nestWatcher(LPVOID pParam);

END_CUTLEADER_NAMESPACE()