#include "StdAfx.h"
#include "CanvasLoopMoveCommand.h"

#include "clCanvasGeometryUIResource.h"
#include "CanvasGeometryHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopMoveCommand::CanvasLoopMoveCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	const vector<LONGLONG>& patternLoops_willMove, const vector<LONGLONG>& textStructs_willMove, const Matrix2D& moveMatrix)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_MoveCanvasLoop), editorData, pPartCadData, pPartCamData),
	  m_patternLoops_willMove(patternLoops_willMove), m_textStructs_willMove(textStructs_willMove), m_moveMatrix(moveMatrix)
{
}

CanvasLoopMoveCommand::~CanvasLoopMoveCommand(void)
{
}

void CanvasLoopMoveCommand::Do()
{
	CloneCadAndCamData();

	// ±ä»»¼¸ºÎÂÖÀª¡£
	CanvasGeometryHelper::TransformPatternLoops(m_pPartCadData, m_pPartCamData, m_patternLoops_willMove, m_textStructs_willMove, m_moveMatrix);
}

void CanvasLoopMoveCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
