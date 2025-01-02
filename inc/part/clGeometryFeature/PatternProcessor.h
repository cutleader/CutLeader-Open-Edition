#pragma once

#include "clGeometryFeatureCommon.h"
#include "baseConst.h"
#include <vector>
#include "Vector2D.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(PolygonPattern)

BEGIN_CUTLEADER_NAMESPACE()

// this class will process the pattern list.
// notes:
//   1) this class will delegate some actions to other "processor".
class ClGeometryFeature_Export PatProcessor
{
public:
	// explode pattern which fit the type in "pPatTypeList".
	// notes:
	//   1) explode the proper pattern to line and arc.
	static void Explode(PatternListPtr pPatList, IntegerListPtr pPatTypeList);

	// 合并能够合并的图形。
	/* 参数：
	*  dParallelDis: 重叠的平行线之间的距离小于这个值时，合并之。
	*  delPats: 被删除的图形。
	*/
	// 注: 
	//   1) 如果2个图形完全重合，删除一个。
	//   2) 共线且接触的直线能够合并，重叠的平行线之间的距离小于指定值时也可以合并。
	//   3) 共圆且接触的圆弧能够合并。
	//   4) we do not change the order of other patterns even though we delete some patterns.
	//   5) the function will not use the cache data of the pattern.
	//   6) 待完善：对于多边形pattern，目前只能和多边形pattern做重叠合并，不能和其他pattern做合并，后续应该改进。
	static void MergePattern(PatternListPtr pPatList, double dParallelDis, std::vector<LONGLONG>& delPats);

	// adjust the coordinate origin to the left bottom corner of the pattern list rect boundary.
	// notes:
	//   1) this function will use cache data of pattern.
	static Vector2D AdjustOriginToLB(PatternListPtr pPatList);


	/************************************************************************/
	// 删掉一些满足要求的pattern

	// remove the tiny patterns in open loops.
	// notes:
	//   1) do not update pPatternLoopList.
	static void RemoveTinyPatInOpenLoops(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, double dVal);

	// remove the tiny patterns.
	// notes:
	//   2) the function will use the cache data of the pattern.
	static void RemoveTinyPatterns(PatternListPtr pPatList, double dVal);
	static void RemoveTinyPatterns(std::vector<PolygonPatternPtr>& polygonPatList, double dVal);

    // 删除重复的多边形pattern。两两比较，只要有重复，即删掉其中一个。
    // 注：
    // 1）判断的标准为：
    //    a) 外接矩形重叠，且回路长度和节点数相等。
    //    b) 多边形pattern1中找一个节点，它的起点和终点要和多边形pattern2一个节点的起点和终点重合。原则上要每个节点做严格的重合比较，由于会影响性能，故采用此简化的做法。
    //    c) 即使满足上述条件，这两个多边形pattern也不一定完全重叠。
	static bool TwoPolygonPatOverlapped(const PolygonPattern* pPolygonPat1, const PolygonPattern* pPolygonPat2);
	static void DeleteOverlappedPolygonPats(std::vector<PolygonPatternPtr>& polygonPatList);

    // 删除重复的多边形pattern。和上面函数不同的是，如果一个轮廓和另一个轮廓的一部分重合（在容差内），那么就把前者删掉。
	// 注：
    //  1) dOverlapTolerance参数表示计算重叠的容差。
    //  2) 这里也不是计算绝对重叠，是通过判断一个多边形pattern的顶点（还判断了节点的中点）是不是都在另一个多边形pattern上，少数情况还是漏掉了。
    static bool PolygonPat1_isPartOf_polygonPat2(const PolygonPattern* pPolygonPat1, const PolygonPattern* pPolygonPat2, double dOverlapTolerance); // 多边形pattern1是不是多边形pattern2的一部分
    static void DeleteOverlappedPolygonPatsEx(std::vector<PolygonPatternPtr>& polygonPatList, double dOverlapTolerance);
	/************************************************************************/


	/************************************************************************/
	// about display the debug info.

	// this function print the start/end points of the line/arc pattern.
	static void PrintLineArcPatPt(PatternListPtr pPatList);

	// display the start/end points of the line/arc pattern.
	static void DisplayLineArcPatPt(PatternListPtr pPatList);
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
