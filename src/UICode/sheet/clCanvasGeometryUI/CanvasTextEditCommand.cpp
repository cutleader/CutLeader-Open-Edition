#include "StdAfx.h"
#include "CanvasTextEditCommand.h"

#include "clGeometryFeatureResource.h"
#include "CanvasGeometryHelper.h"
#include "TextStruct.h"
#include "clGeometryFeatureUIResource.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasTextEditCommand::CanvasTextEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
	LONGLONG iTextStructID, TextParamPtr pNewTextParam, Polygon2DListPtr pNewTextContours)
	: CanvasPatternEditCommandBase(MultiLanguageMgr::GetInstance()->TranslateString(IDS_EditText), editorData, pPartCadData, pPartCamData)
{
	m_iTextStructID = iTextStructID;
	m_pNewTextParam = pNewTextParam;
	m_pNewTextContours = pNewTextContours;
}

CanvasTextEditCommand::~CanvasTextEditCommand(void)
{
}

void CanvasTextEditCommand::Do()
{
	CloneCadAndCamData();

	// 修改一个文字对象的参数。
	CanvasGeometryHelper::ChangeParamOfTextStruct(m_pPartCadData, m_pPartCamData, m_iTextStructID, m_pNewTextParam, m_pNewTextContours.get());
}

void CanvasTextEditCommand::UnDo()
{
	RestoreCadAndCamData();
}

END_CUTLEADER_NAMESPACE()
