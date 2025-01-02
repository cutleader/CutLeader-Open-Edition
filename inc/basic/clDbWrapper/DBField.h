#pragma once

#include "dbWrapperCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// field data within a record.
class ClDbWrapper_Export DBField
{
public:
	DBField(void);
	~DBField(void);

public: // get/set functions.
	void SetFieldName(const CString& strFieldName) { m_strFieldName = strFieldName; }
	const CString& GetFieldName() const { return m_strFieldName; }

	void SetFieldVal(const CComVariant& var) { m_val = var; }
	const CComVariant& GetFieldVal() const { return m_val; }

private:
	// the name of one column.
	CString m_strFieldName;

	CComVariant m_val;
};

END_CUTLEADER_NAMESPACE()
