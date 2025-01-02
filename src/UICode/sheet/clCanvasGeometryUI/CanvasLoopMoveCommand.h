#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasLoopMoveCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopMoveCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		const std::vector<LONGLONG>& patternLoops_willMove, const std::vector<LONGLONG>& textStructs_willMove, const Matrix2D& moveMatrix);
	~CanvasLoopMoveCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ϣ���ƶ��ļ������������֣��Լ��ƶ����ĸ�λ�á�
	const std::vector<LONGLONG> m_patternLoops_willMove;
	const std::vector<LONGLONG> m_textStructs_willMove;
	const Matrix2D m_moveMatrix;
};

END_CUTLEADER_NAMESPACE()
