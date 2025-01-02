#pragma once

#include "IBorderRptEntity.h"
#include "RptEntityBase.h"

BEGIN_CUTLEADER_NAMESPACE()

class BorderRptEntityBase : virtual public IBorderRptEntity,
							virtual public RptEntityBase
{
public:
	BorderRptEntityBase(void);
	~BorderRptEntityBase(void);

public: // implement interface IBorderRptEntity.
	virtual BOOL IsShowBorder() const { return m_bShowBorder; }
	virtual void IsShowBorder(BOOL bShowBorder) { m_bShowBorder = bShowBorder; }

	virtual double GetBorderThickness() const { return m_dBorderThickness; }
	virtual void SetBorderThickness(double dBorderThickness) { m_dBorderThickness = dBorderThickness; }

	virtual LINE_STYLE_TYPE GetBorderStyle() const { return m_borderLineStyle; }
	virtual void SetBorderStyle(LINE_STYLE_TYPE lineStyle) { m_borderLineStyle = lineStyle; }

	virtual COLORREF GetBorderColor() const { return m_borderColor; }
	virtual void SetBorderColor(COLORREF borderColor) { m_borderColor = borderColor; }

private:
	// whether show the border.
	BOOL m_bShowBorder;

	// the border thickness.
	// notes:
	//   1) in inch unit.
	double m_dBorderThickness;

	// the border line style.
	LINE_STYLE_TYPE m_borderLineStyle;

	// the border color.
	COLORREF m_borderColor;
};

END_CUTLEADER_NAMESPACE()
