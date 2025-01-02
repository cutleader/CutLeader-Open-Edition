#pragma once

#include "clReportPrinterCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// the data of one column.
// notes:
//   1) each table column will have such object.
class ClReportPrinter_Export ColumnData
{
public:
	ColumnData(void);
	~ColumnData(void);

public: // get/set functions.
	void SetColumnIndex(int iColumnIndex) { m_iColumnIndex = iColumnIndex; }
	int GetColumnIndex() { return m_iColumnIndex; }

	void SetColumnValue(StringListPtr pColumnValue) { m_pColumnValue = pColumnValue; }
	StringListPtr GetColumnValue() { return m_pColumnValue; }

private:
	// the column index of the table.
	int m_iColumnIndex;

	// the values of this column.
	StringListPtr m_pColumnValue;
};

END_CUTLEADER_NAMESPACE()
