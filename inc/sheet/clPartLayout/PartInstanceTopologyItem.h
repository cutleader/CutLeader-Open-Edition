#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include "ClData.h"
#include <map>

DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstance)

BEGIN_CUTLEADER_NAMESPACE()

// “零件实例拓扑项”。
class ClPartLayout_Export PartInstanceTopologyItem : public ClData
{
public:
	PartInstanceTopologyItem(PartInstancePtr pPartInstance);
	virtual ~PartInstanceTopologyItem(void);

public: // get/set functions.
	void SetPartInstance(PartInstancePtr pPartInstance) { m_pPartInstance = pPartInstance;}
	PartInstancePtr GetPartInstance() const { return m_pPartInstance;}

	void SetParentPartInstance(PartInstancePtr pParentPartInstance) { m_pParentPartInstance = pParentPartInstance; }
	PartInstancePtr GetParentPartInstance() const { return m_pParentPartInstance; }

	void SetParentLoopID(LONGLONG iParentLoopID) { m_iParentLoopID = iParentLoopID; }
	LONGLONG GetParentLoopID() const { return m_iParentLoopID; }

	void SetSubItemList(PartInstanceTopologyItemListPtr pSubItemList) { m_pSubItemList = pSubItemList; }
	PartInstanceTopologyItemListPtr GetSubItemList() const { return m_pSubItemList; }

	void SetBoundaryRect(Rect2D boundaryRect) { m_boundaryRect = boundaryRect; }
	const Rect2D& GetBoundaryRect() const { return m_boundaryRect; }

	void SetInnerRect(std::map<LONGLONG, Rect2D> innerRectList) { m_innerRectList = innerRectList; }
	std::map<LONGLONG, Rect2D> GetInnerRect() const { return m_innerRectList; }

public:
	// get all child top item under this top item.
	// notes:
	//   1) this function should be a recursive one.
	//   2) caller must allocate "pPartInstanceTopologyItemList".
	void GetLeafTopItem(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList);

private:
	// 当前零件实例。
	PartInstancePtr m_pPartInstance;

	// 父亲零件实例。
	// 注:
	//   1) if this top item do not have parent item, this will be null.
	PartInstancePtr m_pParentPartInstance;

	// 当前零件实例在父亲零件实例的哪个回路中。
	LONGLONG m_iParentLoopID;

	// 孩子“拓扑项”。
	PartInstanceTopologyItemListPtr m_pSubItemList;

	// 外围“回路实例”的外接矩形。
	// 注：
	//  1) 对于没有“外围轮廓”的零件实例，是所有图形的外接矩形。
	Rect2D m_boundaryRect;

	// 各个内部“回路实例”的外接矩形，记录了对应的回路ID。
	std::map<LONGLONG, Rect2D> m_innerRectList;
};

END_CUTLEADER_NAMESPACE()
