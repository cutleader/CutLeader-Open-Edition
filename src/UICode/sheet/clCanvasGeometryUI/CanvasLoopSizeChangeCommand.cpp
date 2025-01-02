#include "StdAfx.h"
#include "CanvasLoopSizeChangeCommand.h"

#include "clGeometryFeatureUIResource.h"
#include "CanvasGeometryHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasLoopSizeChangeCommand::CanvasLoopSizeChangeCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	PolygonPatternLoopPtr pPolygonPatternLoop, double dNewLoopWidth, double dNewLoopHeight)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_SetLoopSize), editorData, pPartCadData, pPartCamData),
	m_pPolygonPatternLoop(pPolygonPatternLoop), m_dNewLoopWidth(dNewLoopWidth), m_dNewLoopHeight(dNewLoopHeight)
{
}

CanvasLoopSizeChangeCommand::~CanvasLoopSizeChangeCommand(void)
{
}

void CanvasLoopSizeChangeCommand::Do()
{
	CloneCadAndCamData();

	CanvasGeometryHelper::ChangeSizeOfRectPolygonLoop(m_pPartCadData, m_pPartCamData, m_pPolygonPatternLoop, m_dNewLoopWidth, m_dNewLoopHeight);
}

void CanvasLoopSizeChangeCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
