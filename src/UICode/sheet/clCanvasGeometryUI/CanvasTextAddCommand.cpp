#include "StdAfx.h"
#include "CanvasTextAddCommand.h"

#include "clGeometryFeatureResource.h"
#include "CanvasGeometryHelper.h"
#include "clGeometryFeatureUIResource.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasTextAddCommand::CanvasTextAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DrawText), editorData, pPartCadData, pPartCamData)
{
	m_pTextParam = pTextParam;
	m_transferVect = transferVect;
	m_pTextContours = pTextContours;
}

CanvasTextAddCommand::~CanvasTextAddCommand(void)
{
}

void CanvasTextAddCommand::Do()
{
	CloneCadAndCamData();

	// 往cad数据中添加一个文字结构。
	CanvasGeometryHelper::AddText2Canvas(m_pPartCadData, m_pPartCamData, m_pTextParam, m_transferVect, m_pTextContours);
}

void CanvasTextAddCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
