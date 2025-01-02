#pragma once

#include "dbWrapperCommon.h"

DECLARE_CUTLEADER_CLASS(DBFieldList)

BEGIN_CUTLEADER_NAMESPACE()

// this class stand for one row data of a table.
class ClDbWrapper_Export DBRecord
{
public:
	DBRecord(void);
	~DBRecord(void);

public: // set/get functions.
	DBFieldListPtr GetDBFieldList() const { return m_pDBFieldList; }

private:
	DBFieldListPtr m_pDBFieldList;
};

END_CUTLEADER_NAMESPACE()
