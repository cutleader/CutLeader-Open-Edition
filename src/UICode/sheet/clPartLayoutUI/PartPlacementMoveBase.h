#pragma once

#include "clPartLayoutUICommon.h"
#include "clCutFeatureCommon.h"
#include "Rect2D.h"
#include <vector>
#include <map>
#include "PartDistanceDetectActionBar.h"

DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(RemnantRegion)
DECLARE_CUTLEADER_CLASS(RemnantRegionList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(GlViewPort)

BEGIN_CUTLEADER_NAMESPACE()

// 某些动作中需要移动“零件放置”，这些动作需要从这个类继承。
// 注：
// 1) 这个类提供判断“智能靠碰”和“一键靠碰”的功能。“一键靠碰”的功能由几个静态函数提供。
// 2) 当共边靠碰时，如果移动的几个零件外轮廓不满足共边的条件（相同的刀具尺寸且都要是阳切），那么这些零件只能和材料边框以材料间距进行靠碰，
//    那么怎么膨胀零件和压缩材料边框呢？这里引入了第一个零件外轮廓上的刀具尺寸进行计算，即使几个移动的零件外轮廓的刀具尺寸不同，这样做的结果也是对的。
class PartPlacementMoveBase
{
public:
	PartPlacementMoveBase(GlViewPortPtr pViewPort, PartDistanceDetectActionBar* pPartDistanceDetectActionBar);
	~PartPlacementMoveBase(void);

protected:
	PartDistanceDetectActionBar* GetPartDistanceDetectActionBar() const { return m_pPartDistanceDetectActionBar; }

	// 为移动零件准备数据。
	void UpdateBeforeMov(PartPlacementListPtr pMovePartPlacements, const Point2D& refPtOfMovingPartPmts, IMaterialSizePtr pMaterialSize, PartInstanceListPtr pFixPartInstances);

	// 移动的过程中更新一些数据。
	void UpdateWhenMov(const Point2D& currentPt);

	// 旋转零件时更新数据。
	void UpdateDataWhenRotate(UINT nChar); // 根据按键值来旋转轮廓。
	void UpdateWhenRotate(double dRotateAngle); // 根据角度来旋转轮廓。

	// 把零件放置沿着一个方向做快速移动。
	static BOOL RapidMovePartPmt(IMaterialSizePtr pMaterialSize, const PartAttachConfig& partAttachConfig, PartPlacementListPtr pFixPartPmts, PartInstanceListPtr pFixPartInsts,
		POSITION_INFO iDir, PartPlacementPtr pMovPartPmt, PartInstanceListPtr pMovPartInsts, Matrix2D& retMoveMat);

	// 得到移动的结果。
	PartPlacementListPtr GetMovePartPlacements() const { return m_pMovePartPlacements; }
	const Point2D& GetRefPtOfMovingPartPmts() const { return m_refPtOfMovingPartPmts; }

	// 绘制辅助信息。
	// 注：
	// 1) 在光标附近绘制提示文字。
	// 3) strExtraTip不为空的话，会附加到提示的最后。
	void DrawAssistInfo(const CString& strExtraTip);

private: // 一些静态函数。
	/************************************************************************/
	// 这些函数用于计算多边形的压缩量和膨胀量。

	// 得到“区域内孔”要膨胀的大小。dToolSize是形成该内孔的零件外轮廓上面的刀具尺寸。
	static double GetExpandVal_of_regionHole(const PartAttachConfig& partAttachConfig, double dToolSize);

	// 得到“区域边框”要压缩的大小。
	// 注: 
	//  1) 当零件间距大而材料间距小时，返回的值可能是负的。
	static double GetCompressVal_of_regionBoundary_1(const PartAttachConfig& partAttachConfig, BOOL bOuterRegion, const PartPlacementList* pMovePartPlacements);
	static double GetCompressVal_of_regionBoundary_2(const PartAttachConfig& partAttachConfig, BOOL bOuterRegion, const PartPlacement* pMovePartPmt);

	// 得到“移动的零件”要膨胀的大小。
	static double GetExpandVal_of_movParts(const PartAttachConfig& partAttachConfig, const PartPlacementList* pMovePartPlacements);
	/************************************************************************/


	/************************************************************************/
	// 这些函数用于判断能不能共边。

	// 选中的零件实例能不能做共边。
	// 注：
	// 1) 选中的零件实例的外围轮廓实例需要使用相同的刀具尺寸且都要是阳切。
	static bool IsSelectedPartInstsCanComCut(const LoopInstanceList* pOuterLoopInsts);

	// 选中的零件实例能不能和指定的区域内孔进行共边。
	// 注：
	// 1) 调用这个函数之前要保证选中的零件实例符合做共边的基本条件，也就是IsSelectedPartInstsCanComCut函数返回true。
	// 2) 该函数还隐含了一个意图，就是轮廓不能和包含它的轮廓进行共边。
	// 3) 想和选中的零件实例共边的零件实例的外围轮廓实例要阳切，还要和选中的零件实例的外围轮廓实例的刀具尺寸相同。
	static bool IsSelectedPartInstsCanComCutWithCanvasHoleRegion(const LoopInstanceList* pOuterLoopInsts,
		const std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>& holeRegion);
	/************************************************************************/

private:
	// 当前没有靠碰时检查“排版多边形”的靠碰。
	Point2D CheckStickA(const Point2D& currentPt);

	// 当前有靠碰时检查“排版多边形”的靠碰。
	Point2D CheckStickB(const Point2D& currentPt);


    /************************************************************************/
    // 一键靠碰相关。

	// 把零件按照移动方向快速移动到“余料区域”内。
	// 注：
	//  1) 零件不能和“区域边框”和“区域内孔”重合。
	//  2) 成功的话返回移动到的位置点，这个点相对movPartRect。
	static bool RapidMovInRegion(const PartAttachConfig& partAttachConfig, RemnantRegionPtr pRemRegion, double dCompressVal_of_regionBoundary, Polygon2DListPtr pPartOuterPolys,
        const Rect2D& movPartsRect, POSITION_INFO iDir, Point2D& retPt);

	// 过滤出一些“余料区域”。
	static RemnantRegionListPtr FilterRemRegions(RemnantRegionListPtr pRemRegions, const Rect2D& movPartRect, POSITION_INFO iDir);

	// 过滤出“余料区域”的一些“区域内孔”。
	static std::vector<std::pair<Polygon2DPtr, double>> FilterHoleRegions(RemnantRegionPtr pRemRegion, const Rect2D& movPartRect, POSITION_INFO iDir);

	// 结果点pt1是不是比pt2好。
	// 注：
	//  1) 如果向下快速移动，高的较好。
	static BOOL BetterThan(const Point2D& retPt1, const Point2D& retPt2, POSITION_INFO iDir);
    /************************************************************************/


private:
	/************************************************************************/
	// 移动零件前，初始化这些变量，移动时无需更新。

	// 材料。
	IMaterialSizePtr m_pMaterialSize;

	// 板材上不动的零件放置对应的零件实例。
	PartInstanceListPtr m_pFixPartInstances;

	// 这里缓存移动的零件实例的外围轮廓实例，为了后续的加速。
	LoopInstanceListPtr m_pMovingLoopInsts;

	// 一些“余料区域”，它们的外接矩形尺寸大于“移动的零件实例”的外接矩形尺寸。
	RemnantRegionListPtr m_pRemnantRegionList;
	/************************************************************************/


	/************************************************************************/
	// 移动零件前要初始化或重置这些变量，移动过程中要更新这些变量。

	// “移动的零件放置”的参考点。
	// 注：
	// 1) 一般是要移动的零件的中心点，不过也不一定，拖拽移动时就不一定。
	// 2) 可以想象为手抓住这个位置移动零件。零件被吸附住时，这个点和当前鼠标位置不一定一样。
	// 3) 吸附成功后需要把这个变量更新为吸附点。
	// 4) 视图中看到的零件的位置和该变量是一致的。
	Point2D m_refPtOfMovingPartPmts;

	// 移动的零件的外接矩形。虽然可以从m_pMovePartPlacements得到，不过效率低。
	Rect2D m_rectOfMovingPartPmts;

	// 每个“移动的零件实例”的外围多边形。
	Polygon2DListPtr m_pPartOuterPolys;
	Polygon2DListPtr m_pNewPartOuterPolys; // 被膨胀了半个零件距离，共边时就是半个刀缝。

	// 缓存选中零件与区域边框的内靠碰轨迹，以及与区域内孔的外靠碰轨迹。
	// 注：
	// 1) key是区域外轮廓或内轮廓的ID。
	// 2) 只有在检测距离吸附点时才更新这些数据。
	std::map<LONGLONG, Polygon2DPtr> m_outerStickPolys;
	std::map<LONGLONG, Polygon2DListPtr> m_innerStickPolys;
	/************************************************************************/


	/************************************************************************/
	// 吸附的结果。

	// 用来移动的“零件放置”集合，这就是最后的结果。
	// 注：
	//  1) 在选中零件准备移动前初始化这个变量，这是一份克隆出来的。
	PartPlacementListPtr m_pMovePartPlacements;

	// 靠碰到这个“余料区域”中的多边形。
	RemnantRegionPtr m_pStickRemRegion; // 用这个变量判断当前是不是找到了距离吸附点。
	/************************************************************************/


	// 进行零件位置探测的参数。
	// 注：
	// 1) 参数来自PartDistanceDetectActionBar中。
	// 2) UpdateBeforeMove函数开头初始化这些值，UpdateWhenMove的开始会检查这些参数变化了没有，如果变化了，需要更新数据。
	PartAttachConfig m_partAttachConfig;

	GlViewPortPtr m_pViewPort4ActionBar;
	PartDistanceDetectActionBar* m_pPartDistanceDetectActionBar;
};

END_CUTLEADER_NAMESPACE()
