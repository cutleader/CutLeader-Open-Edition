#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// ���û����������ϵĵ��ߣ�Ҳ���Ǽӹ��򲻼ӹ�������
class ClCutFeature_Export DisableToolOnLoopCommand : public CommandBase
{
public:
	DisableToolOnLoopCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, bool bDisable);
	~DisableToolOnLoopCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;
    bool m_bDisable;

	// �ϵ�����
	std::vector<bool> m_oldData;
};

END_CUTLEADER_NAMESPACE()
