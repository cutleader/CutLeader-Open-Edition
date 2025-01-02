#pragma once

#include "clGeometryFeatureCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

struct LoopNodeData
{
	LONGLONG iID;
	LONGLONG iPatternID;
	LONGLONG iLoopID;
	BOOL bDirection;
};

// this class loads the pattern loops from database.
class ClGeometryFeature_Export PatternLoopLoader
{
public:
	// load all pattern loops of "iPartCadDataID".
	/* params:
	 * pPatList : "in" param.
	 * pPatternLoopList : "out" param.
	*/
	// notes:
	// 1) after loading the loop, update its cache data.
	// 2) pLoopTopologyItems传入时是个空集合。
	static void LoadPatternLoops(DBConnectPtr pDBConnect, PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems);

private:
	// load all loop nodes of the part.
	// notes:
	//   1) the nodes are sorted by the index.
	static std::vector<LoopNodeData> LoadLoopNodesOfPart(DBConnectPtr pDBConnect);

	// get all nodes of the loop.
	static std::vector<LoopNodeData> GetNodesOfLoop(std::vector<LoopNodeData>& nodeDataList, LONGLONG iLoopID);
};

END_CUTLEADER_NAMESPACE()
