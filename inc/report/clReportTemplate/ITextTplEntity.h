#pragma once

#include "clReportConfigCommon.h"
#include "ITplEntity.h"

DECLARE_CUTLEADER_CLASS(FontParam)

BEGIN_CUTLEADER_NAMESPACE()

class ITextTplEntity : virtual public ITplEntity
{
public:
	virtual TEXT_JUST_TYPE GetTextJustType() const = 0;
	virtual void SetTextJustType(TEXT_JUST_TYPE textJustType) = 0;

	virtual FontParamPtr GetFontParam() = 0;
	virtual void SetFontParam(FontParamPtr pFontParam) = 0;
};

END_CUTLEADER_NAMESPACE()
