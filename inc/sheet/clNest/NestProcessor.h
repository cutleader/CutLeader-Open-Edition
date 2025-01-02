#pragma once

#include "clNestCommon.h"
#include "NestProcessor.h"
#include "AreaInfo.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(NestTask)
DECLARE_CUTLEADER_CLASS(Nestor)
DECLARE_CUTLEADER_CLASS(NestProgress)
DECLARE_CUTLEADER_CLASS(NestLogMgr)
DECLARE_CUTLEADER_CLASS(SharedDataPool)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(LibraryData)

BEGIN_CUTLEADER_NAMESPACE()


// 排版处理器。
class ClNest_Export NestProcessor
{
public:
	NestProcessor(NestTaskPtr pNestTask, const AreaInfo& areaInfo, LibraryDataPtr pLibraryData);
	~NestProcessor(void);

public:
	// stop the nesting.
	bool StopNest();

	// whether the nesting is stopped.
	bool IsStopped();

	// whether a better nesting result exist.
	// 注：
	//  1) 当“m_iCurRstIndex”位于结果集的最后一个元素时，返回false。
	bool BetterResultExist();

	// get the nesting result.
	// 注：
	//  1) if no result or better result generated, return null. So call BetterResultExist before call this.
	//  2) 客户端调用一次“GetNestResult”，“m_iCurRstIndex”增加一次，直到结果集的最后一个元素时，返回空值。
	SheetListPtr GetNestResult();

	// write the log.
	void WriteLog(const char* strMsg);

	// 启动排版线程。
	// 注：
	//  1) 每个排版器用一个单独的排版任务，他们之间的排版参数是不同的对象。
	bool StartNest();

private:
	// 排版任务。
	NestTaskPtr m_pNestTask;

	// 任务进度。
	// 注：
	//  1) 这个类中初始化“任务进度”，把它传递到各个“排版器”中。
	NestProgressPtr m_pNestProgress;

	// 使用到的排版器。
	std::vector<NestorPtr> m_pNestors;

	// 指明已经拿到结果集中哪一个结果。
	// 注：
	//  1) 客户端调用一次“GetNestResult”，这个变量增加一次，直到结果集的最后一个元素。
	int m_iCurRstIndex;

	// 日志管理器。
	NestLogMgrPtr m_pLogMgr;

	// 共享数据池。
	SharedDataPoolPtr m_pSharedDataPool;

	// 缓存零件面积。
	AreaInfo m_areaInfo;

	// 预先加载好的要用到的库信息。
	LibraryDataPtr m_pLibraryData;
};

END_CUTLEADER_NAMESPACE()