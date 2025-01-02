#include "StdAfx.h"
#include "CutFeatureHelper.h"

#include "Point2DList.h"
#include "DataBaseManager.h"
#include "LogMgr.h"

#include "ConfigItemLoader.h"
#include "ConfigItemWriter.h"

#include "IPattern.h"
#include "IPatternLoop.h"
#include "RectHolePattern.h"
#include "PatternLoopNodeList.h"
#include "PatternLoopNode.h"
#include "IArrayPattern.h"
#include "IStdPattern.h"
#include "PolygonPatternLoop.h"
#include "LoopToolDataList.h"
#include "LoopProcessor.h"
#include "LoopTopologyItemList.h"
#include "PatternLoopList.h"
#include "LoopToolManager.h"
#include "LoopToolData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

LoopToolDataListPtr CutFeatureHelper::AssignLoopTools4Part(const PatternLoopList* pPatternLoops, const LoopTopologyItemList* pLoopTopologyItems,
	const ToolAssignConfigItem* pToolAssignConfigItem)
{
	LoopToolDataListPtr pLoopToolDataList(new LoopToolDataList);

	for (unsigned int i = 0; i < pPatternLoops->size(); i++)
	{
		IPatternLoopPtr pPatternLoop = pPatternLoops->operator [](i);
		LoopToolDataPtr pLoopToolData(new LoopToolData(pPatternLoop));
		LoopToolManager::AssignLoopTool(pLoopToolData, pLoopTopologyItems, pToolAssignConfigItem);
		pLoopToolDataList->push_back(pLoopToolData);
	}

	return pLoopToolDataList;
}

END_CUTLEADER_NAMESPACE()
