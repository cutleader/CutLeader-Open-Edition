#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(ConditionNode)

BEGIN_CUTLEADER_NAMESPACE()

// �ӡ��������ߡ���ɾ���и����������
class ClCutFeature_Export ConditionNodeDeleteCommand : public CommandBase
{
public:
	ConditionNodeDeleteCommand(const EditorData& editorData, LoopToolDataListPtr pLoopToolList, LONGLONG iDelCondTypeID);
	~ConditionNodeDeleteCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	LoopToolDataListPtr m_pLoopToolList;

	// Ҫɾ���������ڵ㡣
	LONGLONG m_iDelCondTypeID;

	// ��ɾ���������ڵ㡣
	// ע��
	//   1) �е�Ϊ�ա�
	std::vector<ConditionNodePtr> m_delConNodeList;
};

END_CUTLEADER_NAMESPACE()
