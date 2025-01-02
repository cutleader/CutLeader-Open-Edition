#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸��������ߵĳߴ硣
class ClCutFeature_Export ToolSizeModifyCommand : public CommandBase
{
public:
	ToolSizeModifyCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, double dNewToolSize);
	~ToolSizeModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;

	// �ϵĳߴ硣
	std::vector<double> m_oldToolSizeValues;

	// �µĳߴ�
	double m_dNewToolSize;
};

END_CUTLEADER_NAMESPACE()
