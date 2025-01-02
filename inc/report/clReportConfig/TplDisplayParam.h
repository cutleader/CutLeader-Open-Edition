#pragma once

#include "clReportConfigCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClReportConfig_Export TplDisplayParam : public ClData
{
public:
	TplDisplayParam(void);
	~TplDisplayParam(void);

public: // get/set functions.
	TPL_UNIT_TYPE GetTplUnit() { return m_unit; }
	void SetTplUnit(TPL_UNIT_TYPE unit) { m_unit = unit; }

	void IsShowMargin(BOOL bShowMargin) { m_bShowMargin = bShowMargin; }
	BOOL IsShowMargin() { return m_bShowMargin; }

	void SetMarginColor(COLORREF marginColor) { m_marginColor = marginColor; }
	COLORREF GetMarginColor() { return m_marginColor; }

	void IsShowGrid(BOOL bShowGrid) { m_bShowGrid = bShowGrid; }
	BOOL IsShowGrid() { return m_bShowGrid; }

	void SetGridColor(COLORREF gridColor) { m_gridColor = gridColor; }
	COLORREF GetGridColor() { return m_gridColor; }

private:
	// the unit.
	TPL_UNIT_TYPE m_unit;

	// whether display the paper margin.
	BOOL m_bShowMargin;

	// the color of the margin.
	COLORREF m_marginColor;

	// whether display the grid.
	BOOL m_bShowGrid;

	// the color of the grid.
	COLORREF m_gridColor;
};

END_CUTLEADER_NAMESPACE()
