#pragma once

#include "clReportConfigCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClReportConfig_Export TplEditParam  : public ClData
{
public:
	TplEditParam(void);
	~TplEditParam(void);

public: // get/set functions.
	void IsSnapToGrid(BOOL bSnapToGrid) { m_bSnapToGrid = bSnapToGrid; }
	BOOL IsSnapToGrid() { return m_bSnapToGrid; }

	void SetGridCellSizeX(int iGridCellSizeX) { m_iGridCellSizeX = iGridCellSizeX; }
	int GetGridCellSizeX() { return m_iGridCellSizeX; }

	void SetGridCellSizeY(int iGridCellSizeY) { m_iGridCellSizeY = iGridCellSizeY; }
	int GetGridCellSizeY() { return m_iGridCellSizeY; }

private:
	BOOL m_bSnapToGrid;

	// the grid cell size.
	int m_iGridCellSizeX;
	int m_iGridCellSizeY;
};

END_CUTLEADER_NAMESPACE()
