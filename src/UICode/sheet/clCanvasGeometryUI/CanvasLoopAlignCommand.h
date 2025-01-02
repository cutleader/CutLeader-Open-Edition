#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"
#include "Matrix2D.h"
#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasLoopAlignCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopAlignCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		const std::vector<LONGLONG>& patternLoops_willAlign, const std::vector<LONGLONG>& textStructs_willAlign, AlignLoopStyle emAlignLoopStyle);
	~CanvasLoopAlignCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	// 希望移动的几何轮廓和文字，以及对齐的类型。
	const std::vector<LONGLONG> m_patternLoops_willAlign;
	const std::vector<LONGLONG> m_textStructs_willAlign; // 包含于m_patternLoops_willAlign中的文字。
	const AlignLoopStyle m_emAlignLoopStyle;

	// 几何轮廓和文字对象各自的变换矩阵，在构造函数中即生成。
	std::map<LONGLONG, Matrix2D> m_transformMatrixs4Loops;
	std::map<LONGLONG, Matrix2D> m_transformMatrixs4Texts;
};

END_CUTLEADER_NAMESPACE()
