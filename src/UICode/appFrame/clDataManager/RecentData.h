#pragma once

#include "clDataManagerCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the data which opened recently.
class ClDataManager_Export RecentData : public ClData
{
public:
	RecentData(void);
	RecentData(CString strDataPath, LONGLONG iDataID);
	~RecentData(void);

public: // get/set functions.
	void SetDataPath(CString strDataPath) { m_strDataPath = strDataPath; }
	CString GetDataPath() const { return m_strDataPath; }

	void SetDataID(LONGLONG iDataID) { m_iDataID = iDataID; }
	LONGLONG GetDataID() const { return m_iDataID; }

private:
	// the data path.
	CString m_strDataPath;

	// the data ID.
	LONGLONG m_iDataID;
};

END_CUTLEADER_NAMESPACE()
