#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DLoop)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(LoopCutFeature)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// “回路实例”管理器。
class ClPartLayout_Export LoopInstanceManager
{
public:
	/************************************************************************/
	// 关于“回路实例”的切入切出点。

	// 得到“回路实例”的切入点。
	static Point2D GetPiercePt(LoopInstancePtr pLoopInstance);

	// 得到“回路实例”的切出点。
	/* params:
	*  bArrayLastInst: whether the un-pierce point will on the last inst.
	*/
	static Point2D GetUnPiercePt(LoopInstancePtr pLoopInstance, BOOL bArrayLastInst = TRUE);
	/************************************************************************/


	/************************************************************************/
	// 关于得到回路实例。

	// 从零件实例得到回路实例。
	static LoopInstanceListPtr GetLoopInstanceList(PartInstanceListPtr pPartInstances);
	static LoopInstanceListPtr GetLoopInstanceList(PartInstancePtr pPartInstance);

    // 得到轮廓实例，这些轮廓实例没有被设置为“不切割”。
    static LoopInstanceListPtr GetLoopInstances_hasValidTool(PartInstancePtr pPartInstance);

	// 得到零件实例的外围“回路实例”。
	// 注：
	//  1) 如果零件没有外围回路的话，不返回。
	static LoopInstanceListPtr GetOuterLoopInsts(PartInstanceListPtr pPartInstances);
	static LoopInstancePtr GetOuterLoopInst(PartInstancePtr pPartInstance);

	// 得到零件实例的内部“回路实例”。
	static LoopInstanceListPtr GetInnerLoopInsts(PartInstancePtr pPartInstance);

	// 得到距离点最近的“回路实例”。
	static LoopInstancePtr GetClosestLoopInst(LoopInstanceListPtr pLoopInstList, const Point2D& pt);
    // 得到距离点最近的“回路实例”，这个轮廓实例没有被设置为“不切割”。
    static LoopInstancePtr GetClosestLoopInst_hasValidTool(LoopInstanceListPtr pLoopInstList, const Point2D& pt);

	// 得到距离点最近的回路。
	// 注：
	//  1) 只有外围矩形包含点“pt”的“回路实例”才能被作为候选的，注意这里的“外围矩形”被校正成了正方形。
	static LoopInstancePtr GetClosestLoopInstEx(LoopInstanceListPtr pLoopInstList, const Point2D& pt);

	// 得到矩形框内的回路实例。
	static LoopInstanceListPtr GetLoopInstsInRect(LoopInstanceListPtr pLoopInstList, const Rect2D& rect);
	/************************************************************************/


	/************************************************************************/
	// 关于闭合“回路实例”的切割方向。

	// 得到闭合“回路实例”的切割方向。
	static DIRECTION GetCloseLoopInstDir(LoopInstancePtr pLoopInstance);

	// 这些闭合“回路实例”的切割方向是不是一样。
	static BOOL CloseLoopSameDir(LoopInstanceListPtr pLoopInstList);
	/************************************************************************/

	// 更新回路实例的缓存。
	static void UpdateCache(LoopInstancePtr pLoopInstance);
	static void UpdateCache(LoopInstancePtr pLoopInstance, LoopCutFeaturePtr pLoopFeature);

	// 把“轮廓实例”转化为多边形。
	// 注：
	//  1) 只能转封闭的轮廓。对于array loop，只返回了一个，也不知道是哪一个。
	static Polygon2DPtr LoopInst2Poly(LoopInstancePtr pLoopInstance);

	// 生成LineArc2DLoop。传进来封闭的loop。
	static LineArc2DLoopListPtr LoopInsts_2_GeomLoops(const LoopInstanceList* pLoopInstances);
	static LineArc2DLoopPtr LoopInst_2_GeomLoop(const LoopInstance* pLoopInstance);

	// 得到外轮廓膨胀后的LineArc2DLoop，需传入封闭的外轮廓。
	static LineArc2DLoopPtr ExpandLoopInst(LoopInstancePtr pLoopInstance, double dExpandValue);

	// 点是不是在某个零件内部。
	// 注：pLoopInsts_boundary是零件实例的外轮廓。
	static bool Point_is_insidePartInst(const Point2D& pt, LoopInstanceListPtr pLoopInsts_boundary);
};

END_CUTLEADER_NAMESPACE()
