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
	// 希望移动的几何轮廓和文字，以及移动到哪个位置。
	const std::vector<LONGLONG> m_patternLoops_willMove;
	const std::vector<LONGLONG> m_textStructs_willMove;
	const Matrix2D m_moveMatrix;
};

END_CUTLEADER_NAMESPACE()
