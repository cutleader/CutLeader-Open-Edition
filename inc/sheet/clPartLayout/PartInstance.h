#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include "Matrix2D.h"
#include "Rect2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstance)

BEGIN_CUTLEADER_NAMESPACE()

// “零件实例”。
class ClPartLayout_Export PartInstance : public ClData
{
public:
	PartInstance(PartPlacementPtr pPartPlacement);
	virtual ~PartInstance(void);

public: // implement IData interface.
	// clone the part inst.
	// notes:
	//   1) do not clone part placement.
	virtual IDataPtr Clone() const override;

public: // get/set functions
	void SetPartPlacement(PartPlacementPtr pPartPlacement) { m_pPartPlacement = pPartPlacement; }
	PartPlacementPtr GetPartPlacement() const { return m_pPartPlacement; }

	void SetRow(int iRow) { m_iRow = iRow; }
	int GetRow() const { return m_iRow; }

	void SetColumn(int iColumn) { m_iColumn = iColumn; }
	int GetColumn() const { return m_iColumn; }

public:
	// whether the pattern loop exist.
	BOOL PatternLoopExist(LONGLONG iPatternLoopID) const;

	// get the transform matrix which can transform part instance to sheet mode.
	Matrix2D GetTransformMatrix() const;

	// 计算零件实例中各个回路实例的外接矩形。
	// 注：
	//  1) 开放的“回路实例”不处理。
	//  2) “阵列回路实例”会得到多个外接矩形。
	//  3) “pair.first”是放的“外围回路实例”的外接矩形。
	//  4) 对应回路的ID也返回。
	//  5) 对于没有“外围回路”的零件实例，“外围回路实例”的外接矩形用的是所有图形的外接矩形。
	std::pair<Rect2D, std::map<LONGLONG, Rect2D> > GetLoopInstanceRect() const;

	// 零件实例到点的距离。
	// 注：
	//  1) 对于有“外围回路”的零件实例，计算点到“外围回路实例”的距离。
	//  2) 对于没有“外围回路”的零件实例，计算点到“所有基本图形”的距离。
	double DistanceToPt(const Point2D& pt) const;

	// calculate the 2d line/arcs of part placement.
	// notes: 
	//  1) do not calculate cut feature.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// 得到零件实例的中心点。
	// 注：
	//  1) 要是“零件实例”外接矩形的中心点在零件上，则返回这个中心点。
	//     否则返回内轮廓上一点，要是没有内轮廓，返回外轮廓上一点。
	//  2) 如果零件实例没有“外围轮廓”，返回所有图形的中心点。
	Point2D GetCenterPt() const;

	// 零件实例是否有“外围轮廓”。
	BOOL HasBoundaryLoop() const;

	// 得到所有几何轮廓的拓扑关系，定义这个函数是为了方便，就是调用几次get函数即可。
	const LoopTopologyItemList* GetLoopTopologyItems() const;
	LoopTopologyItemListPtr GetLoopTopologyItems_FW() const;

private:
	// the part placement object.
	PartPlacementPtr m_pPartPlacement;

	// if the Part placement is grid, we should indicate the position of the part instance.
	int m_iRow;
	int m_iColumn;
};

END_CUTLEADER_NAMESPACE()
