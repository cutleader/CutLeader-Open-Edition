#pragma once

#include "clGeometryFeatureUICommon.h"
#include "PatternEditCommandBase.h"
#include "Vector2D.h"

DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(TextParam)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令往底图上添加一个文字对象。
class TextAddCommand : public PatternEditCommandBase
{
public:
	TextAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, TextParamPtr pTextParam, const Vector2D& transferVect, Polygon2DListPtr pTextContours);
	~TextAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TextParamPtr m_pTextParam;
	Vector2D m_transferVect;
	Polygon2DListPtr m_pTextContours;
};

END_CUTLEADER_NAMESPACE()
