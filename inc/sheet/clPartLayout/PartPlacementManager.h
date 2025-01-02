#pragma once

#include "clPartLayoutCommon.h"
#include "Matrix2D.h"
#include "Rect2D.h"
#include <vector>
#include <map>


DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(CondConfigItem)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItem)
DECLARE_CUTLEADER_CLASS(PartInstanceTopologyItemList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(RemnantRegion)
DECLARE_CUTLEADER_CLASS(RemnantRegionList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(NestPolygonInfoList)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(Part)


BEGIN_CUTLEADER_NAMESPACE()

// 零件阵列的延伸方向。
typedef enum tagPartGridDirType
{
	PART_GRID_RIGHT_TOP		= 0,
	PART_GRID_RIGHT_BOTTOM	= 1,
	PART_GRID_LEFT_TOP		= 2,
	PART_GRID_LEFT_BOTTOM	= 3,
} PART_GRID_DIR;

// “零件放置”管理器。
class ClPartLayout_Export PartPlacementManager
{
public:
	/************************************************************************/
	// 关于创建“零件放置/实例”对象。

	// 根据“零件项”和变换矩阵创建“零件放置”对象。
	// notes:
	//   1) this is not a grid PartPmt.
	//   2) bClonePartCamData参数控制是否克隆cam数据，目前只有排版过程中生成的临时sheet不需要克隆cam数据。
	static PartPlacementPtr BuildPartPmt(const PartItem* pPartItem, const Matrix2D& mat, bool bClonePartCamData);

	// 炸散阵列零件。
	static PartPlacementListPtr ExplodeGridPart(const PartPlacement* pPartPlacement);
	static void ExplodeGridPart(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances); // 如果集合中有阵列，就炸散。

	// 深度克隆“零件放置”集合。
	// 注：
	//  1) 调用这个函数之前需要先把零件克隆好，这里用了新的零件数据。
	//  2) 这个函数不更新缓存。
	static PartPlacementListPtr ClonePartPmtList(const PartPlacementList* pOldPartPlacements, const std::map<LONGLONG, PartPtr>& newParts);

	// 深度克隆“零件放置”对象。
	// 注：
	//   1) 这个函数会更新缓存。
	static PartPlacementPtr ClonePartPmt(const PartPlacement* pOldPartPmt, PartPtr pNewPart);

	// 克隆“零件放置”对象。
	// 注：
	//   1) 除了零件，其他数据都要克隆。
	//   2) 这个函数会更新缓存。
	static PartPlacementPtr ClonePartPmt(const PartPlacement* pPartPlacement);

	// 创建阵列零件。
	/* 参数：
	*  dPartDis: 零件距离必须大于等于0。
	*  iRowNum,iColNum: 至少有一个大于1。
	*/
	static PartPlacementPtr BuildGridPartPmt(const PartPlacement* pBasePartPmt, double dPartDis, int iRowNum, int iColNum, PART_GRID_DIR iGridDir);

	// 从“零件放置”创建“零件实例”。
	static PartInstanceListPtr BuildPartInsts(PartPlacementListPtr pPartPlacements);
	/************************************************************************/


	/************************************************************************/
	// 关于干涉检查。

	// 检查“零件放置”之间的干涉。
	static void CheckInterfere(PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, ParamConfigPtr pParamConfig);

	// 处理“回路实例”的干涉。
	/* 参数：
	*  pAvoidGeoms: 起切点不能在这些基本图形上。
	*  pHostGeoms: 起切点必须在这些基本图形上。
	*  bOnlyScale: 是否只缩短引入引出线长度。
	*/
	// 注：
	// 1) 待完善：当“阵列回路实例”中有内嵌零件时，干涉检查不对。
	static void ProcessLoopIntf(const PartInstanceTopologyItemList* pPartTopItems, const PartInstanceTopologyItem* pPartTopItem, 
		LoopInstancePtr pLoopInstance, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, 
		const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms, const LeadConfigItem* pLeadConfigItem, BOOL bOnlyScale = FALSE);
	// 底图下使用这个函数。
	static void ProcessLoopIntf_2(LoopInstancePtr pLoopInstance, const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms,
		const LeadConfigItem* pLeadConfigItem, BOOL bOnlyScale = FALSE);

	// 处理“回路实例”的干涉。
	static void ProcessLoopsIntf(PartInstanceListPtr pPartInstances, LoopInstanceListPtr pCheckIntfLoops, const LeadConfigItem* pLeadConfigItem, BOOL bOnlyScale = FALSE);
	/************************************************************************/


	/************************************************************************/
	// 关于拓扑结构。

	// 计算出“零件实例”之间的拓扑关系。
	/* 参数：
	*  partRectMap: 零件实例中各个回路实例的外接矩形。
	*  partTopMap: “零件实例”ID和“零件实例拓扑项”的对应，这样查找会比较快。
	*/
	// 注：
	//  1) 待完善：当“阵列回路实例”中有内嵌零件时，拓扑结构算的不对。
	static PartInstanceTopologyItemListPtr CalcPartTopStruct(PartInstanceListPtr pPartInstances, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>>& partRectMap,
		std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap);
	static PartInstanceTopologyItemListPtr CalcPartTopStruct(PartInstanceListPtr pPartInstances, const std::map<LONGLONG, std::pair<Rect2D, std::map<LONGLONG, Rect2D>>>& partRectMap);

	// 得到“零件实例”所在的拓扑层。
	static PartInstanceTopologyItemListPtr GetPartTopLayer(PartInstanceTopologyItemListPtr pPartTopItems, LONGLONG iPartInstID);
	/************************************************************************/


	/************************************************************************/
	// 关于“余料区域”。

	// 板材上除去这些“零件放置”后得到的余料区域。
	/* 参数：
	*  dMinWidth,dMinHeight: 余料区域的矩形外框尺寸要大于这个参数。
	*/
	static RemnantRegionListPtr CalcRemnantRegion(IMaterialSizePtr pMaterialSize, PartInstanceListPtr pPartInstances, double dMinWidth, double dMinHeight);

	// 计算零件实例和“区域边框”的“内靠碰轨迹”。
	/* 参数：
	*  pPartPolys: 零件实例的外轮廓，已经被膨胀了半个“dPartDis”。
	*  refPt: 用来作为计算临界多边形的参考点。
	*/
	// 注：
	//  1) 这些零件实例作为一个整体移动。
	//  2) 如果“区域边框”无法放下这些零件实例，返回NULL。
	static Polygon2DListPtr CalcInnerStickPolys(const RemnantRegion* pRemnantRegion, const Polygon2DList* pPartPolys, const Point2D& refPt, double dCompressVal_of_regionBoundary);

	// 计算零件实例和“余料区域”内孔的“外靠碰轨迹”。
	/* 参数：
	*  pRegionHolePoly: 某一个区域内孔。
	*  pPartPolys: 零件实例的外轮廓，已经被膨胀了半个“dPartDis”。
	*  refPt: 用来作为计算临界多边形的参考点。
	*/
	static Polygon2DPtr CalcOuterStickPolys(const Polygon2D* pRegionHolePoly, const Polygon2DList* pPartPolys, const Point2D& refPt, double dExpandVal_of_regionHole);

	// 得到板材中所有零件实例的外围多边形。
	// 注：
	//   1) 不处理孔洞中的零件实例。
	static Polygon2DListPtr GetPartInstOuterPoly(PartPlacementListPtr pPartPlacements, std::map<__int64, Polygon2DPtr> partOutPolyData);

	// 用光照的方式得到废料区域。
	/* 参数：
	*  pNestPolygonInfos: 该材料上面排的多边形。
    *  bHoleInLayout: 排版布局中间有个“洞”，就是光线n-1照到了零件，光线n没有照到零件，光线n+1照到了零件。
	*/
	// 注：
	//  1) 用台阶近似的表示废料边界。
	static Polygon2DPtr CalcUsedRegion(Polygon2DPtr pMatPoly, NestPolygonInfoListPtr pNestPolygonInfos, const Rect2D& matOuterRect, const Rect2D& partOuterRect,
		RECT_SIDE iRayDir, int iEvalFactor, bool& bHoleInLayout);

	// 用光照的方法得到余料切割线，返回的折线首尾相连成连续的path，不过方向不确定，调用者需要调整。
	static Line2DListPtr CalcRemCutLine(PartPlacementListPtr pPartPlacements, const IMaterialSize* pIMatSize, RECT_SIDE iRayDir, int iEvalFactor, double dOffsetDistance);

	// 用光照的方式得到有用区域，只取面积最大者。
	static Polygon2DPtr CalcUsefulRegion(PartPlacementListPtr pPartPlacements, const IMaterialSize* pMaterialSize, RECT_SIDE iRayDir, int iEvalFactor);
	/************************************************************************/


    /************************************************************************/
    // 关于零件组

    // 从选中的零件放置集合中过滤出零件组，过滤出的零件组和剩下的零件放置作为返回参数返回给调用方。
    // 注：
    // 1）只返回顶层分组，易证明，选择集中若包含一个分组，则该分组的顶层分组必然也在选择集中。
    static void FilterOutPartGroups_fromSelectedPartPlacements(const PartPlacementList* pPartPlacements, const PartGroupList* pAllPartGroups,
        PartGroupListPtr pPartGroups_return, PartPlacementListPtr pPartPlacements_return);

    // 克隆零件放置后，也克隆相关的零件组。
    static PartGroupListPtr ClonePartGroups_whenClonePartPlacements(const PartGroupList* pAllPartGroups, const PartPlacementList* pPartPlacements_old,
        const PartPlacementList* pPartPlacements_new); // 只需克隆顶层零件组
    static PartGroupListPtr ClonePartGroups_whenClonePartPlacements_2(const PartGroupList* pAllPartGroups, const PartPlacementList* pPartPlacements_old,
        const PartPlacementList* pPartPlacements_new); // 递归克隆零件组，保持零件组的拓扑关系。
    /************************************************************************/


	// update cache data of part pmt.
	// 注：
	//  1) 更新“零件放置”中“回路起切特征”对象中的缓存。
	static void UpdateCacheData(PartPlacementListPtr pPartPlacements);

	// 偏移“零件放置/实例”集合，生成新的“零件放置/实例”集合。
	// 注：
	//   1) 新的“零件放置/实例”对象和偏移前的是完全独立的内存。
	//   2) 考虑到后面工序的生成，这些数据需要和偏移前保持一样：
	//      a) 每个图形回路的ID需要和偏移前一样。
	//      b) 每个零件放置的ID需要和偏移前一样。
	//      c) 每个零件实例的ID需要和偏移前一样。
	// 问题：
	//   1) 执行偏移时会偏移CAD数据，假如某个零件在板材上有2个“零件放置”，这2个“零件放置”的内外切不一样，
	//      这时对CAD数据偏移时就不知道外偏还是内偏，目前的做法是按照第一个“零件放置”的内外切来偏移。
    //   2) 该函数返回偏移失败的几何轮廓。
    static std::set<LONGLONG> OffsetPartPmts(const PartPlacementList* pPartPlacements, const PartInstanceList* pPartInstances,
        PartPlacementListPtr pNewPartPlacements, PartInstanceListPtr pNewPartInsts);

	// 得到所有“零件实例”的外围回路的基本图形，用于检查时做比较。
	// 注：
	//  1) 不处理没有外围回路的零件。
	static std::map<LONGLONG, LineArc2DListPtr> CalcOutLoopGeom(PartInstanceListPtr pPartInstances);

	// 通过阵列新的零件间距，计算出行/列间距。
	/* 参数：
	*  dPartDis: 零件距离必须大于等于0。
	*/
	static std::pair<double, double> CalcGridPartSpacing(PartPlacementPtr pGridPartPmt, double dPartDis);

    // 这个零件实例上所有轮廓的刀具是不是都被禁用了，如果是这样，说明不切割这个零件实例。
    static bool AllLoopToolsDisabled(const PartInstance* pPartInstance);

private:
	// 把“零件拓扑项”对象放到拓扑集合中。
	// 注：
	//   1) 根据拓扑关系，把“pPartInstTopItem”插入到“pPartInstanceTopologyItemList”中合适的位置。
    //   2) 当往内孔中的拓扑项中加零件时，需要提供内孔ID。
	static void AddPartInstToTop(PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList, LONGLONG iInnerLoopID, std::map<LONGLONG, PartInstanceTopologyItemPtr>& partTopMap, PartInstanceTopologyItemPtr pPartInstTopItem);


	/************************************************************************/
	// 有关光照求解废料区域。

	// 得到光束宽度
	static double GetRayInfo(const Rect2D& matOuterRect, RECT_SIDE iRayDir, int iEvalFactor);

	// 得到光线对象，总是从上至下或者从左至右。
	/* 参数：
	 * iRayIndex: 第i条光线，从0开始。
	*/
	static Line2DPtr GetRayLine(const Rect2D& partOuterRect, const Rect2D& matOuterRect, double dStep, RECT_SIDE iRayDir, int iRayIndex);

	// 向“初始废料多边形”中增加两个阶梯点。
	static void AddTwoPoint(Point2DListPtr pPoly, Line2DPtr pRayLine1, Line2DPtr pRayLine2, RECT_SIDE iRayDir, const Point2D& pt);

	// 得到被这个光束照到的所有多边形，并做一些过滤。
	// 注：
	//  1) 先按照光线的方向对这些多边形排序。然后，以光线往下照为例，去掉处在最上面多边形下方的多边形，判断标准为外接矩形完全处在下方。
	static NestPolygonInfoListPtr GetPolysWithinBeam(Line2DPtr pRayLine1, Line2DPtr pRayLine2, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);
	// 过滤出被这个光线刺到的多边形。
	static NestPolygonInfoListPtr GetPolysByRay(const Line2D* pRayLine, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);

	// 计算这个光束笼罩下的多边形顶点中最高的点（以光线往下照为例）。
	// 注：
	//  1) 这个光束笼罩下可能没有多边形顶点（多边形是有的），返回NULL。
	static Point2DPtr GetBeamTouchPt(Line2DPtr pRayLine1, Line2DPtr pRayLine2, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);
	// 得到光线和多边形的交点中最高(以光线往下照为例)的点。
	static Point2DPtr GetRayTouchPt(const Line2D* pRayLine, RECT_SIDE iRayDir, NestPolygonInfoListPtr pNestPolygonInfos);

	// 得到光线和材料的交点中最高(以光线往下照为例)的点。
	static Point2DPtr GetRayTouchPt_to_mat(const Line2D* pRayLine, RECT_SIDE iRayDir, Polygon2DPtr pPolyMat);
	/************************************************************************/


	// 计算余料区域。
	/* 参数：
	*  pMatPoly: 板材区域。
	*  bOutRegion: 是不是最外层的区域。
	*  pPartInstanceTopologyItemList: 放在“pMatPoly”中的零件。
	*/
	// 注：
	//  1) 这是个递归函数。
	static void CalcRemnantRegion(Polygon2DPtr pMatPoly, BOOL bOutRegion, PartInstanceTopologyItemListPtr pPartInstanceTopologyItemList, 
		RemnantRegionListPtr availRegion, double dMinWidth, double dMinHeight);


	/************************************************************************/
	// 关于干涉检查。

	// 递归检测“零件实例”的干涉。
	static void CheckPartInstsIntf(const PartInstanceTopologyItemList* pPartTopItems, const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const LeadConfigItem* pLeadConfigItem);

	// 调整引入引出线的长度，使得它们的外围矩形不得大于回路外围矩形的一半。
	static void CheckLeadLength(LoopStartCutDataPtr pLoopStartCutData, const Rect2D& loopInstRect);

	// 检查“回路实例”是否发生干涉。
	// 注：
	//  1) 外围“回路实例”检查规则：
	//	  a) 检查引入引出线和同层次的零件实例是否干涉。
	//    b) 检查引入引出线和包含他的那个内轮廓是否干涉。
	//  2) 内部“回路实例”检查规则：
	//    a) 检查引入引出线和本身回路是否干涉。
	//    b) 检查引入引出线和孩子零件实例是否干涉。
	static BOOL IsLoopInstIntf(const PartInstanceTopologyItemList* pPartInstTopItems, const PartInstanceTopologyItem* pPartInstTopItem, LoopInstancePtr pLoopInstance,
		const std::map<LONGLONG, LineArc2DListPtr>& outerLoopGeom, const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms);

	// 检查“回路实例”是否发生干涉。该函数用于底图下。
	// 注：
	//  1) 需要完善规则。目前没有考虑多重轮廓的情况。
	static BOOL IsLoopInstIntf_2(const LoopInstance* pLoopInstance, const LineArc2DList* pAvoidGeoms, const LineArc2DList* pHostGeoms);
	/************************************************************************/


	// 计算零件的阵列偏移。
	/* 参数：
	*  dPartDis: 零件距离必须大于等于0。
	*/
	// 注：
	//  1) “pPart”和“mat”指定了板材上面零件的位置，把此零件按照指定的间距做阵列。返回XY方向的偏移量绝对值。
	//  2) 零件如果没有封闭的外轮廓，“dPartDis”为外接矩形的距离。
	//  3) 零件如果有封闭的外轮廓，“dPartDis”为真实零件边界的距离，用排版算法计算阵列结果。
	//  4) 待完善，这个计算会导致细小的误差，在小数点后第五位，一旦累加会造成问题。如一个矩形宽度是5.79366，水平方向阵列的间隙是0.1，
	//     这个函数计算出的水平方向阵列偏移是5.79362，产生了误差，使用的地方需要注意。
	static std::pair<double, double> CalcGridData(const Part* pPart, const Matrix2D& mat, double dPartDis);

	// 得到阵列零件的阵列方向。
	static PART_GRID_DIR GetGridPartDir(PartPlacementPtr pGridPartPmt);
};

END_CUTLEADER_NAMESPACE()
