#include "StdAfx.h"
#include "TextEditCommand.h"

#include "clGeometryFeatureResource.h"
#include "TextStruct.h"
#include "clGeometryFeatureUIResource.h"
#include "GeometryFeatureHelper.h"

BEGIN_CUTLEADER_NAMESPACE()

TextEditCommand::TextEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, LONGLONG iTextStructID, TextParamPtr pNewTextParam, Polygon2DListPtr pNewTextContours)
	: PatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditText), editorData, pPartCadData)
{
	m_iTextStructID = iTextStructID;
	m_pNewTextParam = pNewTextParam;
	m_pNewTextContours = pNewTextContours;
}

TextEditCommand::~TextEditCommand(void)
{
}

void TextEditCommand::Do()
{
	CloneCadData();

	// 修改一个文字对象的参数。
	GeometryFeatureHelper::ChangeParamOfTextStruct(m_pPartCadData, m_iTextStructID, m_pNewTextParam, m_pNewTextContours.get());
}

void TextEditCommand::UnDo()
{
	RestoreCadData();
}

END_CUTLEADER_NAMESPACE()
