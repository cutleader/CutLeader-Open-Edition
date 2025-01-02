#pragma once

#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// this class loads the pattern from database.
class ClGeometryFeature_Export PatternLoader
{
public:
	// load all patterns of "iPartCadDataID".
	// notes:
	//   1) after loading a pattern, update its cache data.
	static void LoadPatternList(DBConnectPtr pDBConnect, PatternListPtr pPatList);

private:
	// load the Relation between the patterns.
	static void LoadPatternRelation(DBConnectPtr pDBConnect, PatternListPtr pPatList);
};

END_CUTLEADER_NAMESPACE()
