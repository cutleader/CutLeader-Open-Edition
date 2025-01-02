#pragma once

#include "CGridListCtrlGroups.h"

BEGIN_CUTLEADER_NAMESPACE()

// some cells of such grid are invalid and displayed as red color.
class ClUILib_Export InvCellGrid : public CGridListCtrlGroups
{
public:
	InvCellGrid(void);
	~InvCellGrid(void);

public:
	bool OnDisplayCellColor(int nRow, int nCol, COLORREF& textColor, COLORREF& backColor);
};

END_CUTLEADER_NAMESPACE()
