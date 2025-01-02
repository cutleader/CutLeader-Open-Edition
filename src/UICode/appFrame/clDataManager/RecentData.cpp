#include "StdAfx.h"
#include "RecentData.h"

BEGIN_CUTLEADER_NAMESPACE()

RecentData::RecentData(void)
{
}

RecentData::RecentData(CString strDataPath, LONGLONG iDataID)
{
	m_strDataPath = strDataPath;
	m_iDataID = iDataID;
}

RecentData::~RecentData(void)
{
}

END_CUTLEADER_NAMESPACE()
