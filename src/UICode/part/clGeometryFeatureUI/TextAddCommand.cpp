#include "StdAfx.h"
#include "TextAddCommand.h"

#include "clGeometryFeatureResource.h"
#include "clGeometryFeatureUIResource.h"
#include "GeometryFeatureHelper.h"

BEGIN_CUTLEADER_NAMESPACE()

TextAddCommand::TextAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours)
	: PatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_DrawText), editorData, pPartCadData)
{
	m_pTextParam = pTextParam;
	m_transferVect = transferVect;
	m_pTextContours = pTextContours;
}

TextAddCommand::~TextAddCommand(void)
{
}

void TextAddCommand::Do()
{
	CloneCadData();

	// ��cad���������һ�����ֽṹ��
	GeometryFeatureHelper::AddText2Canvas(m_pPartCadData, m_pTextParam, m_transferVect, m_pTextContours);
}

void TextAddCommand::UnDo()
{
	RestoreCadData();
}

END_CUTLEADER_NAMESPACE()
