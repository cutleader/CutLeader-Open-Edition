#pragma once

#include "BorderRptEntityBase.h"

DECLARE_CUTLEADER_CLASS(BoxRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the box entity in the report.
class BoxRptEntity : public BorderRptEntityBase
{
public:
	BoxRptEntity(void);
	~BoxRptEntity(void);

public: // implement IRptEntity interface.
	virtual RPT_ENTITY_TYPE GetType() const { return RPT_ENTITY_BOX; }

public: // get/set functions.
	void IsFill(BOOL bFill) { m_bFill = bFill; }
	BOOL IsFill() { return m_bFill; }

	void SetFillColor(COLORREF fillColor) { m_fillColor = fillColor; }
	COLORREF GetFillColor() { return m_fillColor; }

private:
	// whether fill the box.
	BOOL m_bFill;

	// the fill color.
	COLORREF m_fillColor;
};

END_CUTLEADER_NAMESPACE()
