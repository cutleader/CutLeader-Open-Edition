#include "StdAfx.h"
#include "CanvasPatternDeleteCommand.h"

#include "clGeometryFeatureResource.h"
#include "CanvasGeometryHelper.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternDeleteCommand::CanvasPatternDeleteCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	PatternListPtr pDeletingPatterns, const vector<LONGLONG>& textStructs_willDelete)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_DEL_PAT), editorData, pPartCadData, pPartCamData)
{
	m_pDeletingPatterns = pDeletingPatterns;
	m_textStructs_willDelete = textStructs_willDelete;
}

CanvasPatternDeleteCommand::~CanvasPatternDeleteCommand(void)
{
}

void CanvasPatternDeleteCommand::Do()
{
	CloneCadAndCamData();

	// 从cad数据中删除集合特征。
	CanvasGeometryHelper::DeletePatternsOnCanvas(m_pPartCadData, m_pPartCamData, m_pDeletingPatterns.get(), m_textStructs_willDelete);
}

void CanvasPatternDeleteCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
