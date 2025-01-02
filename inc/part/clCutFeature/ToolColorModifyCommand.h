#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸��������ߵ���ɫ��
class ClCutFeature_Export ToolColorModifyCommand : public CommandBase
{
public:
	ToolColorModifyCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, COLORREF iNewToolColor);
	~ToolColorModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;

	// �ϵ���ɫ
	std::vector<COLORREF> m_oldToolColors;

	// �µ���ɫ
	COLORREF m_iNewToolColor;
};

END_CUTLEADER_NAMESPACE()
