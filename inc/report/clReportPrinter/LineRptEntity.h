#pragma once

#include "clReportTemplateCommon.h"
#include "RptEntityBase.h"

DECLARE_CUTLEADER_CLASS(LineRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the line entity in the report.
class LineRptEntity : public RptEntityBase
{
public:
	LineRptEntity(void);
	~LineRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_LINE; }

public: // get/set functions.
	double GetLineThickness() const { return m_dLineThickness; }
	void SetLineThickness(double dLineThickness) { m_dLineThickness = dLineThickness; }

	LINE_STYLE_TYPE GetLineStyle() const { return m_lineStyle; }
	void SetLineStyle( LINE_STYLE_TYPE lineStyle ) { m_lineStyle = lineStyle; }

	COLORREF GetLineColor() const { return m_lineColor; }
	void SetLineColor( COLORREF lineColor ) { m_lineColor = lineColor; }

private:
	// line thickness.
	// notes:
	//   1) in inch unit.
	double m_dLineThickness;

	// line style.
	LINE_STYLE_TYPE	m_lineStyle;

	// line color.
	COLORREF m_lineColor;
};

END_CUTLEADER_NAMESPACE()
