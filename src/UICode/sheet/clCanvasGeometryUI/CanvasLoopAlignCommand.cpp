#include "StdAfx.h"
#include "CanvasLoopAlignCommand.h"

#include "LogMgr.h"
#include "IntegerList.h"

#include "PatternLoopCache.h"
#include "PatternLoopList.h"
#include "PartCadData.h"
#include "clCanvasGeometryUIResource.h"
#include "CanvasGeometryHelper.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "clGeometryFeatureResource.h"
#include "GeometryFeatureHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopAlignCommand::CanvasLoopAlignCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	const vector<LONGLONG>& patternLoops_willAlign, const vector<LONGLONG>& textStructs_willAlign, AlignLoopStyle emAlignLoopStyle)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_AlignLoop), editorData, pPartCadData, pPartCamData),
	  m_patternLoops_willAlign(patternLoops_willAlign), m_textStructs_willAlign(textStructs_willAlign), m_emAlignLoopStyle(emAlignLoopStyle)
{
	// 根据对齐类型，计算出几何轮廓和文字对象各自的变换矩阵。
	GeometryFeatureHelper::CalcTransformMatrixs_ForAlignLoops(m_pPartCadData.get(), m_patternLoops_willAlign, m_textStructs_willAlign,
		emAlignLoopStyle, m_transformMatrixs4Loops, m_transformMatrixs4Texts);
}

CanvasLoopAlignCommand::~CanvasLoopAlignCommand(void)
{
}

void CanvasLoopAlignCommand::Do()
{
	CloneCadAndCamData();

	// 对齐几何轮廓。
	CanvasGeometryHelper::TransformPatternLoops_2(m_pPartCadData, m_pPartCamData, m_patternLoops_willAlign, m_textStructs_willAlign,
		m_transformMatrixs4Loops, m_transformMatrixs4Texts);
}

void CanvasLoopAlignCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
