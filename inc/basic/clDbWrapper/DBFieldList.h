#pragma once

#include "dbWrapperCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBField)

BEGIN_CUTLEADER_NAMESPACE()

class ClDbWrapper_Export DBFieldList : public std::vector<DBFieldPtr>
{
public:
	DBFieldList(void);
	~DBFieldList(void);

public:
	// get DBField by field name.
	DBFieldPtr GetFieldByName(CString strFieldName);
};

END_CUTLEADER_NAMESPACE()
