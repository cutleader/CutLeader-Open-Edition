#pragma once

#include "clPostProcessorCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// one NC row.
class ClPostProcessor_Export NCRow
{
public:
	NCRow(void);
	~NCRow(void);

public: // get/set functions.
	void SetNCRowID(int iNCRowID) { m_iNCRowID = iNCRowID; }
	int GetNCRowID() const { return m_iNCRowID; }

	void SetNCRow(CString strNCRow) { m_strNCRow = strNCRow; }
	CString GetNCRow() const { return m_strNCRow; }

private:
	// the ID of the NC row, driver should generate the ID.
	int m_iNCRowID;

	// a row NC code.
	CString m_strNCRow;
};

END_CUTLEADER_NAMESPACE()
