#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "Rect2D.h"
#include "ClData.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// “零件实例”集合。
class ClPartLayout_Export PartInstanceList : public std::vector<PartInstancePtr>, public ClData
{
public:
	PartInstanceList(void);
	virtual ~PartInstanceList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the part inst list.
	// notes:
	//   1) do not clone part placement within each part inst.
	virtual IDataPtr Clone() const override;

public:
	// add the part instance by the part placement
	// notes: 
	//   1) when we add a part placement into sheet, we should also add the relative part instances.
	void AddPartInstanceByPartPlacement(PartPlacementPtr pPartPlacement);

	void AppendPartInstances(const PartInstanceList* pPartInstList) { this->insert(this->end(), pPartInstList->begin(), pPartInstList->end()); }


	/************************************************************************/
	// 关于删除零件实例。

	// delete the part instance by the part placement
	// notes: 
	//   1) when we delete a part placement from sheet, we should also delete the relative part instances.
	void DeletePartInstanceByPartPlacement(const PartPlacement* pPartPlacement);

	// delete the part instance by ID.
	void DeletePartInstanceByID(LONGLONG iId);

	// 删除多个“零件实例”。
	void DeletePartInstanceList(const PartInstanceList* pPartInstances);

	// 根据零件ID删除零件实例。
	void DeletePartInstanceByPartID(LONGLONG iPartID);
	/************************************************************************/


	/************************************************************************/
	// about getting part instances.

	// get the part instance by id
	PartInstancePtr GetPartInstanceByID(LONGLONG iId) const;

	// get the index of "pPartInstance".
	int GetPartInstanceIndex(const PartInstance* pPartInstance) const;

	// get the part instance by the grid part placement and the row/column.
	// notes: 
	//   1) you know, for grid, part pmt and the row/column can decide a part, so only call this function for grid.
	PartInstancePtr GetGridPartInstance(const PartPlacement* pPartPlacement, int iRow, int iCol) const;

	// get part insts by the part pmt.
	PartInstanceListPtr GetPartInstanceByPartPlacement(const PartPlacement* pPartPlacement) const;

	// 得到距离点最近的“零件实例”。
	/* 参数:
	*  partRectMap: 零件实例中各个回路实例的外接矩形。
	*/
	// 注：
	//   1) 比较的依据是点到“外围回路实例”的距离。
	PartInstancePtr GetClosestPartInstance(const Point2D& pt, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>>& partRectMap);

	// 根据零件ID得到“零件实例”集合。
	PartInstanceListPtr GetPartInstances(LONGLONG iPartID) const;

	// 得到有“外围轮廓”的零件实例。
	PartInstanceListPtr GetPartInstancesHasOutLoop() const;

	// 得到所有“兄弟零件”。
	PartInstanceListPtr GetPartInstances_brother(PartInstancePtr pPartInstance) const;
	/************************************************************************/


	// whether the pattern loop exist.
	BOOL PatternLoopExist(LONGLONG iPatternLoopID);

	// 计算零件实例中各个回路实例的外接矩形。
	// 注：
	//  1) 开放的“回路实例”不处理。
	//  2) “阵列回路实例”会得到多个外接矩形。
	//  3) “pair.first”是放的“外围回路实例”的外接矩形。
	//  4) 对于没有“外围回路”的零件实例，“外围回路实例”的外接矩形用的是所有图形的外接矩形。
	std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D> > > GetLoopInstanceRect();
};

END_CUTLEADER_NAMESPACE()
