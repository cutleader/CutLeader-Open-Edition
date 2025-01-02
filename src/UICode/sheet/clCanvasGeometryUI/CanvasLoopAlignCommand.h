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
	// ϣ���ƶ��ļ������������֣��Լ���������͡�
	const std::vector<LONGLONG> m_patternLoops_willAlign;
	const std::vector<LONGLONG> m_textStructs_willAlign; // ������m_patternLoops_willAlign�е����֡�
	const AlignLoopStyle m_emAlignLoopStyle;

	// �������������ֶ�����Եı任�����ڹ��캯���м����ɡ�
	std::map<LONGLONG, Matrix2D> m_transformMatrixs4Loops;
	std::map<LONGLONG, Matrix2D> m_transformMatrixs4Texts;
};

END_CUTLEADER_NAMESPACE()
