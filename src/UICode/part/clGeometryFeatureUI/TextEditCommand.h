#pragma once

#include "clGeometryFeatureUICommon.h"
#include "PatternEditCommandBase.h"
#include "Vector2D.h"

DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(TextParam)
DECLARE_CUTLEADER_CLASS(TextStruct)

BEGIN_CUTLEADER_NAMESPACE()

// ��������޸�һ�����ֶ���Ĳ�����
class TextEditCommand : public PatternEditCommandBase
{
public:
	TextEditCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, LONGLONG iTextStructID, TextParamPtr pNewTextParam, Polygon2DListPtr pNewTextContours);
	~TextEditCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LONGLONG m_iTextStructID;
	TextParamPtr m_pNewTextParam;
	Polygon2DListPtr m_pNewTextContours;
};

END_CUTLEADER_NAMESPACE()
