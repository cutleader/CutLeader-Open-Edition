#include "StdAfx.h"
#include "CanvasPatternAddCommand.h"

#include "clGeometryFeatureResource.h"
#include "CanvasGeometryHelper.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternAddCommand::CanvasPatternAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, IPatternPtr pNewPattern)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_ADD_PAT), editorData, pPartCadData, pPartCamData)
{
	m_pNewPattern = pNewPattern;
}

CanvasPatternAddCommand::~CanvasPatternAddCommand(void)
{
}

void CanvasPatternAddCommand::Do()
{
	CloneCadAndCamData();

	// ��cad���������һ������������
	CanvasGeometryHelper::AddPattern2Canvas(m_pPartCadData, m_pPartCamData, m_pNewPattern);
}

void CanvasPatternAddCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
