#pragma once

#include "clReportPrinterCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(TableColumn)

BEGIN_CUTLEADER_NAMESPACE()

class TableColumnList : public std::vector<TableColumnPtr>
{
public:
	TableColumnList(void);
	~TableColumnList(void);
};

END_CUTLEADER_NAMESPACE()
