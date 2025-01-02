#include "StdAfx.h"
#include "PatternLoopAlignCommand.h"

#include "LogMgr.h"
#include "IntegerList.h"

#include "PatternLoopCache.h"
#include "PatternLoopList.h"
#include "PartCadData.h"
#include "GeometryFeatureHelper.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "clGeometryFeatureResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternLoopAlignCommand::PatternLoopAlignCommand(const EditorData& editorData, PartCadDataPtr pPartCadData,
	const vector<LONGLONG>& patternLoops_willAlign, const vector<LONGLONG>& textStructs_willAlign, AlignLoopStyle emAlignLoopStyle)
	: PatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_AlignLoop), editorData, pPartCadData), m_patternLoops_willAlign(patternLoops_willAlign),
	m_textStructs_willAlign(textStructs_willAlign), m_emAlignLoopStyle(emAlignLoopStyle)
{
	// 根据对齐类型，计算出几何轮廓和文字对象各自的变换矩阵。
	GeometryFeatureHelper::CalcTransformMatrixs_ForAlignLoops(m_pPartCadData.get(), m_patternLoops_willAlign, m_textStructs_willAlign,
		emAlignLoopStyle, m_transformMatrixs4Loops, m_transformMatrixs4Texts);
}

PatternLoopAlignCommand::~PatternLoopAlignCommand(void)
{
}

void PatternLoopAlignCommand::Do()
{
	CloneCadData();

	// 对齐几何轮廓。
	GeometryFeatureHelper::TransformPatternLoops_2(m_pPartCadData, m_patternLoops_willAlign, m_textStructs_willAlign, m_transformMatrixs4Loops, m_transformMatrixs4Texts);
}

void PatternLoopAlignCommand::UnDo()
{
	RestoreCadData();
}

END_CUTLEADER_NAMESPACE()
