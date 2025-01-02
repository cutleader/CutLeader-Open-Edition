#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class writes the pattern loops into database.
class ClGeometryFeature_Export PatternLoopWriter
{
public:
	// save pattern loops.
	// notes:
	//   1) before call this, make sure the "delete" SQL is executed for all loops of this part.
	static void SavePatternLoop(DBConnectPtr pDBConnect, PatternLoopListPtr pPatternLoopList, BOOL bTransaction = TRUE);
};

END_CUTLEADER_NAMESPACE()
