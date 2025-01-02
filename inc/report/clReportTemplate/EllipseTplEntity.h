#pragma once

#include "BorderTplEntityBase.h"

DECLARE_CUTLEADER_CLASS(EllipseTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export EllipseTplEntity : public BorderTplEntityBase
{
public:
	EllipseTplEntity(void);
	virtual ~EllipseTplEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_ELLIPSE; }

public: // get/set functions.
	void IsFill(BOOL bFill) { m_bFill = bFill; }
	BOOL IsFill() { return m_bFill; }

	void SetFillColor(COLORREF fillColor) { m_fillColor = fillColor; }
	COLORREF GetFillColor() { return m_fillColor; }

private:
	// whether fill the box.
	// notes:
	//   1) use intData5.
	BOOL m_bFill;

	// the fill color.
	// notes:
	//   1) use intData6.
	COLORREF m_fillColor;
};

END_CUTLEADER_NAMESPACE()
