#include "StdAfx.h"
#include "CanvasLoopRotateCommand.h"

#include "clCanvasGeometryUIResource.h"
#include "CanvasGeometryHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopRotateCommand::CanvasLoopRotateCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	const vector<LONGLONG>& patternLoops_willRotate, const vector<LONGLONG>& textStructs_willRotate, const Matrix2D& rotateMatrix)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_RotateCanvasLoop), editorData, pPartCadData, pPartCamData),
	 m_patternLoops_willRotate(patternLoops_willRotate), m_textStructs_willRotate(textStructs_willRotate), m_rotateMatrix(rotateMatrix)
{
}

CanvasLoopRotateCommand::~CanvasLoopRotateCommand(void)
{
}

void CanvasLoopRotateCommand::Do()
{
	CloneCadAndCamData();

	// ±ä»»¼¸ºÎÂÖÀª¡£
	CanvasGeometryHelper::TransformPatternLoops(m_pPartCadData, m_pPartCamData, m_patternLoops_willRotate, m_textStructs_willRotate, m_rotateMatrix);
}

void CanvasLoopRotateCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
