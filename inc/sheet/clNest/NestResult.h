#pragma once

#include "clNestCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(SheetList)

BEGIN_CUTLEADER_NAMESPACE()

// the nesting result.
class ClNest_Export NestResult
{
public:
	NestResult();
	~NestResult(void);

public:
	SheetListPtr GetNestResultByIndex(int iIndex) const;
	void AddNestResult(SheetListPtr pNestResult);
	unsigned int GetNestResultCount() const;

	bool TaskFinished() const;
	void TaskFinished(bool bTaskFinished);

	int GetElapsedTime() const { return m_iElapsedTime; }
	void SetElapsedTime(int iElapsedTime) { m_iElapsedTime = iElapsedTime; }

private:
	// the mutex which will protect the progress data.
	HANDLE m_hNestResultMutex;

	// the nesting results from the kernel.
	std::vector<SheetListPtr> m_nestResults;

	// whether the task is finished. means the kernel finished the nesting and the client got all results.
	bool m_bTaskFinished;

	// elapsed time.
	int m_iElapsedTime;
};

END_CUTLEADER_NAMESPACE()