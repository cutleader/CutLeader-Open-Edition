#pragma once

#include "clReportTemplateCommon.h"
#include "IRptEntity.h"

BEGIN_CUTLEADER_NAMESPACE()

// the interface for all report entities which can display the border.
class IBorderRptEntity : virtual public IRptEntity
{
public:
	virtual BOOL IsShowBorder() const = 0;
	virtual void IsShowBorder(BOOL bShowBorder) = 0;

	virtual double GetBorderThickness() const = 0;
	virtual void SetBorderThickness(double dBorderThickness) = 0;

	virtual LINE_STYLE_TYPE GetBorderStyle() const = 0;
	virtual void SetBorderStyle(LINE_STYLE_TYPE lineStyle) = 0;

	virtual COLORREF GetBorderColor() const = 0;
	virtual void SetBorderColor(COLORREF borderColor) = 0;
};

END_CUTLEADER_NAMESPACE()
