#pragma once

#include "clMaterialLibCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

class ClMaterialLib_Export MaterialProcessor
{
public:
	// get the area of the material size.
	// notes:
	//   1) for skeleton material, we should deduct the used region.
	static double GetArea(const IMaterialSize* pMaterialSize);

	// 得到材料的外框。
	// 注：
	//  1) 返回的图形元素首尾连成一个回路。
	static LineArc2DListPtr GetBorderLineArcs(const IMaterialSize* pMaterialSize);

	// 得到材料边框的多边形表示。
	// 注：
	//  1) 多边形逆时针。
	static Polygon2DPtr GetBorderPoly(const IMaterialSize* pMaterialSize);

	// 得到材料的无效区域。
	static LineArc2DListPtr GetUselessRegions(const IMaterialSize* pMaterialSize);
	static std::vector<LineArc2DListPtr> GetUselessRegionsA(const IMaterialSize* pMaterialSize); // 每个无效回路作为一个元素。

	// 从“基本图形回路”集合生成“材料尺寸”对象。
	// 注：
	//  1) 这个函数没有设置材料尺寸的父亲ID。
	static IMaterialSizePtr BuildMatSize(CString strName, const LineArc2DLoopList* pGeomLoopList);

	// 逆时针旋转90度。
	static void Rot90Degree(IMaterialSizePtr pMaterialSize);

	// 从材料对象得到外围多边形。
	static Polygon2DPtr BuildOuterPolyFromMat(const IMaterialSize* pMaterialSize);
};

END_CUTLEADER_NAMESPACE()
