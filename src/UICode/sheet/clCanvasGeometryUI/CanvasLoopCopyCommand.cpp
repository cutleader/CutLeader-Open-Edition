#include "StdAfx.h"
#include "CanvasLoopCopyCommand.h"

#include "clCanvasGeometryUIResource.h"
#include "CanvasGeometryHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopCopyCommand::CanvasLoopCopyCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	PatternLoopListPtr pPatternLoops_willCopy, const vector<LONGLONG>& textStructs_willCopy, const Matrix2D& transformMatrix)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_CopyCanvasLoop), editorData, pPartCadData, pPartCamData)
{
	m_pPatternLoops_willCopy = pPatternLoops_willCopy;
	m_textStructs_willCopy = textStructs_willCopy;
	m_transformMatrix = transformMatrix;
}

CanvasLoopCopyCommand::~CanvasLoopCopyCommand(void)
{
}

void CanvasLoopCopyCommand::Do()
{
	CloneCadAndCamData();

	// 把几何轮廓拷贝到一个新的位置。
	m_newCadData = CanvasGeometryHelper::ClonePatternLoops2NewPosition(m_pPartCadData, m_pPartCamData, m_pPatternLoops_willCopy.get(), m_textStructs_willCopy, m_transformMatrix);
}

void CanvasLoopCopyCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
