#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasLoopRotateCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopRotateCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		const std::vector<LONGLONG>& patternLoops_willRotate, const std::vector<LONGLONG>& textStructs_willRotate, const Matrix2D& rotateMatrix);
	~CanvasLoopRotateCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 希望旋转的几何轮廓和文字对象，以及旋转矩阵。
	const std::vector<LONGLONG> m_patternLoops_willRotate;
	const std::vector<LONGLONG> m_textStructs_willRotate;
	const Matrix2D m_rotateMatrix;
};

END_CUTLEADER_NAMESPACE()
