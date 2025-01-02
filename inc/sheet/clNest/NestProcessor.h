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


// �Ű洦������
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
	// ע��
	//  1) ����m_iCurRstIndex��λ�ڽ���������һ��Ԫ��ʱ������false��
	bool BetterResultExist();

	// get the nesting result.
	// ע��
	//  1) if no result or better result generated, return null. So call BetterResultExist before call this.
	//  2) �ͻ��˵���һ�Ρ�GetNestResult������m_iCurRstIndex������һ�Σ�ֱ������������һ��Ԫ��ʱ�����ؿ�ֵ��
	SheetListPtr GetNestResult();

	// write the log.
	void WriteLog(const char* strMsg);

	// �����Ű��̡߳�
	// ע��
	//  1) ÿ���Ű�����һ���������Ű���������֮����Ű�����ǲ�ͬ�Ķ���
	bool StartNest();

private:
	// �Ű�����
	NestTaskPtr m_pNestTask;

	// ������ȡ�
	// ע��
	//  1) ������г�ʼ����������ȡ����������ݵ��������Ű������С�
	NestProgressPtr m_pNestProgress;

	// ʹ�õ����Ű�����
	std::vector<NestorPtr> m_pNestors;

	// ָ���Ѿ��õ����������һ�������
	// ע��
	//  1) �ͻ��˵���һ�Ρ�GetNestResult���������������һ�Σ�ֱ������������һ��Ԫ�ء�
	int m_iCurRstIndex;

	// ��־��������
	NestLogMgrPtr m_pLogMgr;

	// �������ݳء�
	SharedDataPoolPtr m_pSharedDataPool;

	// ������������
	AreaInfo m_areaInfo;

	// Ԥ�ȼ��غõ�Ҫ�õ��Ŀ���Ϣ��
	LibraryDataPtr m_pLibraryData;
};

END_CUTLEADER_NAMESPACE()