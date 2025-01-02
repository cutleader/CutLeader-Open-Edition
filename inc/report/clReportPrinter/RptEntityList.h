#pragma once

#include "clReportPrinterCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IRptEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the report entity list.
class ClReportPrinter_Export RptEntityList : public std::vector<IRptEntityPtr>
{
public:
	RptEntityList(void);
	~RptEntityList(void);
};

END_CUTLEADER_NAMESPACE()
