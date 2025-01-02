#pragma once

#include "TplEntityBase.h"

DECLARE_CUTLEADER_CLASS(LineTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

typedef struct {
	CRect rect;
	BOOL hit;
} HitParamsRect;

typedef struct {
	int x;
	int y;
	BOOL hit;
} HitParams;

class ClReportTemplate_Export LineTplEntity : public TplEntityBase
{
public:
	LineTplEntity(void);
	~LineTplEntity(void);

public: // functions from IData interface.
	virtual IDataPtr Clone() const override;

public: // implement ITplEntity interface.
	virtual ENTITY_TYPE GetType() const { return ENTITY_LINE; }
	virtual BOOL BodyInRect(CRect rect) const;
	virtual ENTITY_POSITION_PT GetPtPosition(const CPoint& point);
	virtual HCURSOR GetCursorByPosPt(ENTITY_POSITION_PT entityPosPt);

public: // get/set functions.
	int GetLineThickness() const { return m_iLineThickness; }
	void SetLineThickness( int iLineThickness ) { m_iLineThickness = iLineThickness; }

	LINE_STYLE_TYPE GetLineStyle() const { return m_lineStyle; }
	void SetLineStyle( LINE_STYLE_TYPE lineStyle ) { m_lineStyle = lineStyle; }

	COLORREF GetLineColor() const { return m_lineColor; }
	void SetLineColor( COLORREF lineColor ) { m_lineColor = lineColor; }

private:
	// line thickness.
	// notes:
	//   1) use intData1.
	int	m_iLineThickness;

	// line style.
	// notes:
	//   1) use intData2.
	LINE_STYLE_TYPE	m_lineStyle;

	// line color.
	// notes:
	//   1) use intData3.
	COLORREF m_lineColor;
};

END_CUTLEADER_NAMESPACE()
