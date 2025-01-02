#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)

BEGIN_CUTLEADER_NAMESPACE()

// �����������ߡ��������и����������
class ClCutFeature_Export ConditionNodeAddCommand : public CommandBase
{
public:
	ConditionNodeAddCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, ConditionNodeListPtr pNewCondNodeList);
	~ConditionNodeAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ�޸ĵ���������������
	LoopToolDataListPtr m_pLoopToolList;

	// Ҫ���ӵ������ڵ㡣
	ConditionNodeListPtr m_pNewCondNodeList;
};

END_CUTLEADER_NAMESPACE()
