#pragma once

#include "clReportPrinterCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ColumnData)
DECLARE_CUTLEADER_CLASS(ColumnDataList)

BEGIN_CUTLEADER_NAMESPACE()

// the data of all columns.
class ColumnDataList : public std::vector<ColumnDataPtr>
{
public:
	ColumnDataList(void);
	~ColumnDataList(void);

public:
	/************************************************************************/
	// about get column data.

	// get column data bu column index.
	ColumnDataPtr GetColumnData(int iColumnIndex);

	// get column data by the row range.
	ColumnDataListPtr GetColumnDataList(UINT iStartRowIndex, UINT iEndRowIndex);
	/************************************************************************/

	// get the row count.
	int GetRowCount();
};

END_CUTLEADER_NAMESPACE()
