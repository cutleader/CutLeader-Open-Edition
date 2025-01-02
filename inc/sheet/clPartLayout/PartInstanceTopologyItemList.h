#pragma once

#include "clPartLayoutCommon.h"
#include "clCutFeatureCommon.h"
#include "Rect2D.h"
#include <vector>
#include <map>
#include <tuple>

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// the list of the part topology item.
class ClPartLayout_Export PartInstanceTopologyItemList : public std::vector<PartInstanceTopologyItemPtr>
{
public:
	PartInstanceTopologyItemList(void);
	~PartInstanceTopologyItemList(void);

public:
	/************************************************************************/
	// about get part inst.

	// get part inst by ID.
	// notes:
	//   1) this is a recursive function.
	PartInstancePtr GetPartInstanceByID(LONGLONG iPartInstID);

	// get the part insts.
	// notes:
	//   1) this is a not recursive function.
	PartInstanceListPtr GetPartInstances() const;

	// get all part insts.
	// notes:
	//   1) this is a recursive function.
	//   2) caller must allocate "pPartInstances".
	void GetAllPartInstList(PartInstanceListPtr pPartInstances) const;
	/************************************************************************/

	// this function do not recursive.
	void RemoveTopItem(PartInstanceTopologyItemPtr pPartInstTopItem);

	// get the <part inst, PartInstanceTopologyItem> map.
	// notes:
	//   1) user can use this to search PartInstanceTopologyItem by part inst quickly.
	std::map<LONGLONG, PartInstanceTopologyItemPtr> GetPartTopMap();

	// 得到放在同一个内孔中的零件。
	PartInstanceTopologyItemListPtr GetItemsByParentLoop(LONGLONG iParentLoopID);

	// 得到这一拓扑层所有零件的多边形外框和外轮廓上面的刀具尺寸。
	// 注：
	//  1) 对于没有外围轮廓的零件，不返回。
	std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>> GetPartOuterPolys() const;
};

END_CUTLEADER_NAMESPACE()
