#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryFeature_Export TextStructWriter
{
public:
	static void SaveTextStructs(DBConnectPtr pDBConnect, TextStructListPtr pTextStructs, BOOL bTransaction = TRUE);
};

END_CUTLEADER_NAMESPACE()
