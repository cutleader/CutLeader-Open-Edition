#pragma once

#include "clGeometryFeatureCommon.h"
#include "baseConst.h"
#include "Point2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(ArrayPatternLoop)
DECLARE_CUTLEADER_CLASS(StdPatternLoop)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(TextStruct)

BEGIN_CUTLEADER_NAMESPACE()

// the connection type of the pattern to an open loop.
enum PAT_CONNECT_TYPE
{
	PAT_CONNECT_NOT						= -1,

	PAT_CONNECT_START2HEAD				= 0,
	PAT_CONNECT_START2TAIL				= 1,
	PAT_CONNECT_END2HEAD				= 2,
	PAT_CONNECT_END2TAIL				= 3,
};

// the connection type between two open loops.
typedef enum tagLoopConnectType
{	
	LOOP_CONNECT_NOT			= -1,

	LOOP_CONNECT_HEAD2HEAD		= 0,
	LOOP_CONNECT_HEAD2TAIL		= 1,
	LOOP_CONNECT_TAIL2HEAD		= 2,
	LOOP_CONNECT_TAIL2TAIL		= 3,
} LOOP_CONNECT_TYPE;

// 用来创建cad数据的选项。这个结构对应到系统参数中的一部分选项，不过数值不一定和系统参数中的对应，有时会根据业务要求动态修改某些参数。
struct BuildPartCadDataOption
{
	BuildPartCadDataOption(DIM_UNIT_TYPE dimUnit, DIM_UNIT_TYPE impUnit, double dImpTol, int iMergeItemCount, double dMergeLineDis, double dLoopFilterSize,
		UINT iOptimizeLimit, BOOL bMoreArc, double dAllowOffset, double dPatFilterSize, BOOL bProcessStd, BOOL bProcessGrid)
		: m_dimUnit(dimUnit), m_impUnit(impUnit), m_dImpTol(dImpTol), m_iMergeItemCount(iMergeItemCount), m_dMergeLineDis(dMergeLineDis), m_dLoopFilterSize(dLoopFilterSize),
		m_iOptimizeLimit(iOptimizeLimit), m_bMoreArc(bMoreArc), m_dAllowOffset(dAllowOffset), m_dPatFilterSize(dPatFilterSize), m_bProcessStd(bProcessStd), m_bProcessGrid(bProcessGrid) {}
	DIM_UNIT_TYPE m_dimUnit;
	DIM_UNIT_TYPE m_impUnit;
	double m_dImpTol;
	int m_iMergeItemCount;
	double m_dMergeLineDis;
	double m_dLoopFilterSize;
	UINT m_iOptimizeLimit;
	BOOL m_bMoreArc;
	double m_dAllowOffset;
	double m_dPatFilterSize;
	BOOL m_bProcessStd;
	BOOL m_bProcessGrid;
};

// this class will generate loops from the pattern list.
class ClGeometryFeature_Export LoopBuilder
{
public:
	/************************************************************************/
	// build loop by patterns.
	
	// 根据一些几何特征来创建几何轮廓。
	// 注：
	// 1) 该函数使用了传入的几何特征的缓存，没有更新返回的几何轮廓的缓存。
	static PatternLoopListPtr BuildPatternLoops(const PatternList* pPatList, double dTol = SYSTEM_TOLERANCE);

	// 根据一些直线/圆弧几何特征来创建多边形轮廓。
	// 注：
	// 1) 该函数使用了传入的几何特征的缓存，没有更新返回的几何轮廓的缓存。
	// 2) 该函数可能会调整集合pLineArcPatterns中元素的顺序，不过不会修改元素数据。
	static PatternLoopListPtr BuildPolygonPatternLoops(PatternListPtr pLineArcPatterns, double dTol = SYSTEM_TOLERANCE);

	// build a polygon loop from the standard pattern.
	// notes:
	//   1) the direction of new polygon loop is CCW, just same as line/arc in "pStdPat".
	//   2) the cache data of pattern/loop should be updated.
	static PolygonPatternLoopPtr BuildPolyLoopByStdPat(const IPattern* pStdPat);
	/************************************************************************/


	/************************************************************************/
	// build polygon loop by other type loop.

	// build polygon loop by array pattern loop.
	// notes:
	//   1) we only convert the first inst of the array pattern to the polygon pattern.
	//   2) the cache data of pattern/loop should be updated.
	static PolygonPatternLoopPtr BuildPolyLoopByArrayLoop(const ArrayPatternLoop* pArrayLoop);

	// build polygon loop by std pattern loop.
	// notes:
	//   1) the cache data of pattern/loop should be updated.
	static PolygonPatternLoopPtr BuildPolyLoopByStdLoop(const StdPatternLoop* pStdLoop);

	// build a new polygon loop by pattern loop.
	// notes:
	//   1) if "pPatternLoop" is polygon pattern loop, only clone it.
	//   2) if "pPatternLoop" is std pattern loop, need to call BuildPolyLoopByStdLoop().
	//   2) if "pPatternLoop" is array pattern loop, need to call BuildPolyLoopByArrayLoop().
	static PolygonPatternLoopPtr BuildPolyLoopByPatternLoop(const IPatternLoop* pPatternLoop);

	// build the polygon loop by polygon.
	// 注:
	//   1) this function just connect patterns in "pPatList" as a loop simply.
	//   2) patterns in "pPatList" are connected head-tail, but the direction may be not same.
	//   3) 如果传入的是2个首尾相连的直线或者完全重叠的两个圆弧，返回空。
	/* params:
	*  pPatList: patterns within it will connect head by tail.
	*/
	static PolygonPatternLoopPtr BuildPolyLoopByPolygon(PatternListPtr pPatList);

	// 从多边形pattern生成多边形回路。这个函数不用也不维护缓存。
	static PolygonPatternLoopPtr BuildPolyLoopByPolygonPat(PolygonPatternPtr pPolygonPat);
	/************************************************************************/


	// 利用从外部导入的图形集合来构建PartCadData对象。
	/* 参数：
	*  bProcessStd: 合并为标准图形。
	*  bProcessGrid: 合并为阵列图形。
	*/
	// 注：
	//   1) 需要根据系统设置执行图形处理，优化等动作。
	//   2) 送进来的pattern的缓存是更新过的，这个函数结束时需要更新缓冲数据。
	//   3) 送进来的pattern的缓存是更新过的，这个函数结束时需要更新缓冲数据。textData中数据的缓存也是更新过的。
	static PartCadDataPtr BuildPartCadData(PatternListPtr pPatList_notPolygon, PatternListPtr pPatList_polygon,
		const std::vector<std::tr1::tuple<TextStructPtr, PatternListPtr, PatternLoopListPtr>>& textData, const BuildPartCadDataOption& option);

private:
	// 把这些开放的多边形pattern连成闭合的多边形pattern，连不了的也要返回。
	// 注:
	// 1) 该函数使用了传入的几何特征的缓存，没有更新返回的几何特征的缓存。
	// 2) 传入的多边形pattern在列表中的顺序可能会被调整，不过其数据没有被改变。
	static std::vector<PolygonPatternPtr> BuildClosedPolygonPats_byOpenPolygonPats(std::vector<PolygonPatternPtr> polygonPatList_open, double dTol = SYSTEM_TOLERANCE);
	// 由于没有地方调用该函数，所以去掉了它的实现，以后如需要可以去svn或其他产品中找。
	static PatternLoopListPtr BuildClosedPolyLoops_byOpenPolyLoops(PatternLoopListPtr pOpenPatternLoopList, double dTol = SYSTEM_TOLERANCE);

	// get the connection info of the pattern to an open loop.
	/* params:
	*  headPt, tailPt: the head/tail point of the open loop.
	*  pPattern: 直线或者圆弧pattern
	*  dTol: use this to check whether two patterns are connected.
	*  dMinDis: return the min distance whether they are connected or not.
	*  pPolygonPatternLoop: 正在生成中的多边形轮廓
	*  bCannotOverlapWithPrePattern: pPattern是不是不能和上一个pattern完全重叠
	*/
	// 注:
	// 1) if start point of "pPattern" is connected to "tailPt", we return PAT_CONNECT_START2TAIL.
	//    if start/end point of "pPattern" are all connected to "headPt", we choose the closer one.
	// 2) 该函数使用了pLineArcPattern的缓存。
	static PAT_CONNECT_TYPE GetPatConnectInfo(const Point2D& headPt, const Point2D& tailPt, const IPattern* pLineArcPattern, double dTol, double& dMinDis,
		const PolygonPatternLoop* pPolygonPatternLoop, bool bCannotOverlapWithPrePattern);

	// get the connection info between two open loops.
	/* params:
	*  headPt, tailPt: the head/tail point of the open loop.
	*  dTol: use this to check whether two patterns are connected.
	*  dMinDis: return the min distance whether they are connected or not.
	*/
	// 注:
	// 1) if start point of "pPolygonPatternLoop" is connected to "tailPt", we return LOOP_CONNECT_HEAD2TAIL.
	//    if start/end point of "pPolygonPatternLoop" are all connected to "headPt", we choose the closer one.
	// 2) 该函数没有使用pPolygonPat的缓存。
	static LOOP_CONNECT_TYPE GetLoopConnectInfo(const Point2D& headPt, const Point2D& tailPt, const PolygonPattern* pPolygonPat, double dTol);
	// 由于没有地方调用该函数，所以去掉了它的实现，以后如需要可以去svn或其他产品中找。
	static LOOP_CONNECT_TYPE GetLoopConnectInfo(const Point2D& headPt, const Point2D& tailPt, const PolygonPatternLoop* pPolygonPatternLoop, double dTol, double& dMinDis);

    // 判断这两个pattern是不是完全重合，用最高精度来判断，这两个pattern是直线或者圆弧。
    static bool TwoPatternsOverlapped(const IPattern* pLineArcPattern1, const IPattern* pLineArcPattern2);

	// 是否需要对图形做优化
	static bool NeedOptimization(const std::vector<PolygonPatternPtr>& polygonPatList, UINT iOptimizeLimit);
};

END_CUTLEADER_NAMESPACE()
