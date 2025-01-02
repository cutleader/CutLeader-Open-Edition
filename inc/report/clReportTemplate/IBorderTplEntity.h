#pragma once

#include "clReportTemplateCommon.h"
#include "ITplEntity.h"

BEGIN_CUTLEADER_NAMESPACE()

class IBorderTplEntity : virtual public ITplEntity
{
public:
	virtual BOOL IsShowBorder() const = 0;
	virtual void IsShowBorder(BOOL bShowBorder) = 0;

	virtual int GetBorderThickness() const = 0;
	virtual void SetBorderThickness(int iBorderThickness) = 0;

	virtual LINE_STYLE_TYPE GetBorderStyle() const = 0;
	virtual void SetBorderStyle(LINE_STYLE_TYPE lineStyle) = 0;

	virtual COLORREF GetBorderColor() const = 0;
	virtual void SetBorderColor(COLORREF borderColor) = 0;
};

END_CUTLEADER_NAMESPACE()
