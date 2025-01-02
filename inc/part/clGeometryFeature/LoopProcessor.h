#pragma once

#include "baseConst.h"
#include "PatternPosition.h"
#include "Point2D.h"
#include <vector>
#include <set>

DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(LoopTopologyItem)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(PatternLoopNodeList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PolygonPattern)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ�λ�·����������
class ClGeometryFeature_Export LoopProcessor
{
public:
	/************************************************************************/
	// �����޸ļ��������еļ���������

	// adjust "pPattern" to the head of the loop.
	// notes:
	//   1) call this for close polygon loop.
	static void AdjustToHead(IPatternLoopPtr pPatternLoop, IPatternPtr pPattern);

	// add a pattern to "pPatternLoopList".
	/* params:
	*  dTol: when need to re-calculate the loops, need it.
	*/
	// notes:
	//   1) this operation may affect those open paths, so we should collect open loops
	//      which "link" to "pPattern", if we only find one loop, append the pattern to the loop,
	//      BUT if this pattern affects more than one loop, re-calculate these loops.
	//   2) function without "Ex" should not use/update the cache data of the loops.
	//   3) function with "Ex" should update the cache data and the boundary loop of the loops.
	static void AddPatToLoopList(PatternLoopListPtr pPatternLoopList, IPatternPtr pPattern, double dTol = GEOM_TOLERANCE);
	static void AddPatToLoopListEx(PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, IPatternPtr pPattern, double dTol = GEOM_TOLERANCE);

	// delete a pattern from the PatternLoopList, this will affect the loop which contains this pattern.
	/* params:
	*  dTol: when need to re-calculate the loops, need it.
	*/
	// ע:
	// 1) if the loop is a closed "polygon" path, we should move "pPattern" to the head, and then delete "pPattern", the close path become open path.
	// 2) for open "polygon" path, if we remove the pattern at it head or tail, just remove it. BUT if user deleted a pattern not at the head or tail, 
	//    we should delete this loop, and rebuild two loops.
	// 3) function without "Ex" should not use/update the cache data of the loops.
	// 4) function with "Ex" should update the cache data and the boundary loop of the loops.
	static void DelPatFromLoopList(PatternLoopListPtr pPatternLoopList, IPatternPtr pPattern, double dTol = GEOM_TOLERANCE);
	static void DelPatFromLoopListEx(PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, IPatternPtr pPattern, double dTol = GEOM_TOLERANCE);

	// change patterns in loop list.
	/* params:
	*  pPatList: the "after modified" pattern data.
	*/
	// notes:
	// 1) work flow:
	//	  step 1: if we can find a closed loop in "pPatList", we will "protect" that loop.
	//	  step 2: for other case, we can call "RemovePattern" and then call "AddPattern".
	// 2) "pPatList" may be changed in this function, so caller should be aware of this.
	// 3) this function should update the cache data and the boundary loop of the loops.
	static void ChangePatInLoopList(PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, PatternListPtr pPatList);

	// remove the tiny nodes from the loop.
	// notes:
	// 1) currently, this function only care the polygon loop.
	// 2) "dVal" will be compared with the boundary rect of the loop node.
	// 3) the function will use the cache data of the pattern.
	static void RemoveTinyLoopNode(PatternLoopListPtr pPatternLoopList, PatternListPtr pPatList, double dVal);
	static void RemoveTinyLoopNode(PatternLoopListPtr pPatternLoopList, double dVal, std::set<LONGLONG>& polyLoops_deletedNodes);
	/************************************************************************/


	/************************************************************************/
	// ���ڼ��������Ľǡ�

	// whether the point is at the loop corner.
	// notes:
	// 1) this function will check whether "pt" is at the loop corner, if yes, update "pt" with the precise corner point.
	static BOOL PtAtLoopCorner(const IPatternLoop* pPatternLoop, Point2D& pt);

	// get the angle of two adjacent base geom at point(cornerPt).
	// ע:
	// 1) do not call this for open path.
	// 2) do not call this if only one line/arc geom in the loop, e.g. a "round hole" pattern loop.
	// 3) ������Ǵӻ�·�ڲ����ġ�
	static double GetAngAtLoopCorner(const IPatternLoop* pPatternLoop, const Point2D& cornerPt);

	// get PatternPosition data of the adjacent geom.
	// notes:
	// 1) before call this, make sure that "cornerPt" is at the loop corner.
	// 2) refer to the illustration, if we have gotten the PatternPosition at geom1, this function will return the PatternPosition at geom2.
	/*     
						  *        
						  *  geom2      
						  *        
						  *        
						  *        
						  *        
			*************** corner point       
                  geom1
	*/
	static void GetNextCornerGeomPos(const IPatternLoop* pPatternLoop, const Point2D& cornerPt, LONGLONG iFirstPatID, const PatternPosition& firstPatPos,
		LONGLONG& iSecondPatID, PatternPosition& secondPatPos);
	/************************************************************************/


	/************************************************************************/
	// �йؼ����������Ż���

	// merge some loop nodes as line.
	// notes:
	// 1) there are all line nodes in "pPatternLoopList".
	static void OptimizeToLine(PatternLoopListPtr pPatternLoopList, PatternListPtr pPatList, double dDelta);

	// ��һЩֱ�߽ڵ���ϳ�ֱ�ߡ�
	// ע��
	// 1) �����������Ҳ�����»��档
    static void OptimizeToLine_1(PolygonPatternLoopPtr pPolygonPatternLoop, double dDelta);
    static void OptimizeToLine_2(PolygonPatternPtr pPolygonPat, double dDelta); // ���������ж�
    static void OptimizeToLine_3(PolygonPatternPtr pPolygonPat, double dDelta); // ����ʵ��ƫ���ж�

	// merge some loop nodes as arc.
	// notes:
	// 1) there are all line nodes in "pPatternLoopList".
	// 2) �����������Ҳ�����»���
    // 3) �������Ŀǰ�������ˡ�
	static void OptimizeToArc(PatternLoopListPtr pPatternLoopList, PatternListPtr pPatList, double dAngleDelta, double dLengthDelta);

	// merge line nodes which are coline.
	// notes:
	// 1) �����������Ҳ�����»���
	static void MergeCoLineNode_1(PolygonPatternLoopPtr pPolygonPatternLoop);
	static void MergeCoLineNode_2(PolygonPatternPtr pPolygonPat);

	// merge some loop nodes as arc.
	// notes:
	// 1) if the sweep angle is smaller than GEOM_TOLERANCE, use line instead.
    // 2) �����������Ҳ�����»��档
	/* params:
	*  dTolerance: the allowed offset between the old curve and the new curve.
	*/
	static void OptimizeToArc_1(PolygonPatternLoopPtr pPolygonPatternLoop, double dTolerance);
    static void OptimizeToArc_2(PolygonPatternPtr pPolygonPat, double dTolerance);

	// ɾ���ظ��Ļ�·�������Ƚϣ�ֻҪ���ظ�����ɾ������һ�����жϵı�׼Ϊ��Ӿ����ص����һ�·���Ⱥͽڵ�����ȣ���ʵ��ʱ��·��һ����ȫ�ص���
	static bool TwoLoopsOverlapped(const IPatternLoop* pPatternLoop1, const IPatternLoop* pPatternLoop2);
	static void DeleteOverlappedLoops(PatternLoopListPtr pPatternLoopList);
	/************************************************************************/


	/************************************************************************/
	// ���ڵ��Եĺ�����

	// print the start/end points of the polygon loop's node.
	static void PrintPolyLoopPt(IPatternLoopPtr pPatternLoop);

	// print the gap size between each loop node.
	static void PrintPolyLoopGap(IPatternLoopPtr pPatternLoop);

	// display each loop segment.
	static void DisplayLoopSegment(std::vector<PatternLoopNodeListPtr> loopSegmentList);
    static void DisplayLoopSegment(std::vector<LineArc2DListPtr> loopSegmentList);
	/************************************************************************/


	/************************************************************************/
	// �йػ�����¡�

	// update the cache data of pattern loop object.
	// notes:
	//   1) before call this, make sure cache data of patterns are updated.
	static void UpdateCache(PatternLoopListPtr pPatternLoopList);
	static void UpdateCache(IPatternLoopPtr pPatternLoop);

	// update the cache data of pattern and loop object.
	// notes:
	//   1) care control pattern.
	static void UpdateCacheEx(PatternLoopListPtr pPatternLoopList);
	static void UpdateCacheEx(IPatternLoopPtr pPatternLoop);
	/************************************************************************/


	/************************************************************************/
	// �й�ƫ��������

	// ƫ�ƻ�·��
	// ע��
	//   1) ƫ������Ҫ���¡�pPatList����
	//   2) �ú�������¡�ͼ�Ρ��͡���·���Ļ��档
	static BOOL OffsetPolyLoop(PatternListPtr pPatList, IPatternLoopPtr pPatternLoop, BOOL bInside, double dOffsetVal);

	// ƫ�ƻ�·��
	// ע��
	//   1) �ú�������¡�ͼ�Ρ��͡���·���Ļ��档
	static BOOL OffsetPolyLoop(IPatternLoopPtr pPatternLoop, BOOL bInside, double dOffsetVal, const std::vector<std::pair<LONGLONG, LONGLONG>>& corners);
	/************************************************************************/


	/************************************************************************/
	// ���ڼ�������֮������˹�ϵ��

	// ������ͼ�λ�·֮������˹�ϵ��
	// ע��
	//   1) ���Ż�·ֻ�ܱ��պϻ�·�����������ܰ���������·��
	//   2) �ú���Ҫ�ü��������еĻ��档
	//   3) �����ƣ�Ŀǰ�ж����������Ƿ������������ʱ��Щ���⣬����ֻ����һ��ʵ���Ķ���α�����ж��Ƿ��������������Ӧ��������ʵ���Ķ���α�����жϡ�
	static LoopTopologyItemListPtr CalcLoopTopStruct(const PatternLoopList* pPatternLoopList);

	// �ҳ���������������ڵ����˹�ϵ��loopTopologyItemData�е�ÿһ���Ӧһ�������
	static const LoopTopologyItemList* FindLoopTopologyData(const std::vector<LoopTopologyItemListPtr>& loopTopologyItemData, const IPatternLoop* pPatternLoop);
	/************************************************************************/


	/************************************************************************/
	// �й������ϵĵ㡣

	// get first 8-percent point of the loop.
	/* ����:
	*  dPercentAngle: 8-multiple of PI/4.
	*  iIndex: the index of the loop node which hold the cross point.
	*/
	// notes:
	//   1) for array loop, get the point on the first inst.
	//   2) if no cross point, get the loop's closest point, refer the code.
	static void GetFst8PercentPt(const IPatternLoop* pPatternLoop, double dPercentAngle, Point2D& crossPt, int& iIndex);

	// �õ���ͼ�λ�·����16�ȷֵ����Щ�ȷֵ�����ͼ�εĶ˵㡣
	/* ������
	*  pPercentPts, pPercentIndexs: ���صĵȷֵ㼰���ڵĻ�·�ڵ㡣
	*  pCornerPts, pCornerIndexs: ���صĶ˵㼰���ڵĻ�·�ڵ㡣
	*  dFloatAngle: ȡ�ǵ�ʱ����������ͼ�εļнǲ�����180�����������Χ�ڡ�
	*/
	// ע��
	//  1) ������������Ρ��ıߵ���ʱ�뷽��ȷ֣���������ĵȷֵ㱻������
	//  2) �ظ���ͼ�ζ˵㱻ȥ����
	//  3) ����Ҫ180������һ����Χ�ڵĽǵ㡣
	//  4) ����������Ҫ�Ƕȴ���270�ȵĽǵ㣬����������Ҫ�Ƕ�С��90�ȵĽǵ㡣
	static void Get16PercentPt(const IPatternLoop* pPatternLoop, const LoopTopologyItemList* pLoopTopologyItems, Point2DListPtr pPercentPts,
		IntegerListPtr pPercentIndexs, Point2DListPtr pCornerPts, IntegerListPtr pCornerIndexs, double dFloatAngle);

	// �õ�����ʵ��(pPatternLoop, transformMatrix)�ϵ�pt����ĵ㣬�����ص����ڵ������ڵ㡣
	// ע��
	// 1) �ú����ȽϺ���⣬�������м��������е����⣬��Ҫ�ѵõ��������ת����һ��ʵ�����档
	// 2) ��transformMatrix������ʱ�����صĵ㴦�ڰ������ϵ�¡�
	static Point2D GetClosestPt(const IPatternLoop* pPatternLoop, const Matrix2D& transformMatrix, const Point2D& pt, int& iLoopNodeIndex);
	/************************************************************************/


	/************************************************************************/
	// �й����������ıȽϡ�

	// ������·����״�ǲ���һ������״һ������˼�ǣ�һ����·ͨ��ƽ�ƻ���ת���ܺ���һ����·��ȫ����������
	// ע��
	//  1) ĿǰҪ����Ļ�·Ҫ�Ƿ�յġ�
	//  2) Ҫ����״��ͬ����Ҫ���شӻ�·1�任����·2�ľ���ע�⣬����ı任������ܲ�ֹһ����
	//  3) �и����⣬���ǵ�������Բ��ʱ�򣬷��صľ���ֻ��ƽ�Ʊ任����Ϊ��ת�任����������
	static bool TwoCloseLoops_sameShape(const IPatternLoop* pPatternLoop1, const IPatternLoop* pPatternLoop2, std::vector<Matrix2D>& matrixList_1to2, bool& bLoop_is_circle);

	// �жϿ��Ż�·1���վ���任���ǲ��ǺͿ��Ż�·2��ȫ�ص������������Ĭ��ֵ����ô�����ж���������·�Ƿ���ȫ�ص���
	static bool TwoCloseLoops_totallyOverlapped(const IPatternLoop* pPatternLoop1, const IPatternLoop* pPatternLoop2, const Matrix2D& mat_1to2);
	static bool TwoOpenLoops_totallyOverlapped(const IPatternLoop* pPatternLoop1, const IPatternLoop* pPatternLoop2, const Matrix2D& mat_1to2);
	/************************************************************************/


	// clamp polygon loop tightly.
	/* params:
	*  dConnectTol: the connect tolerance which was used to build "pPolygonPatternLoop".
	*/
	// ע:
	// 1) sometimes, line/arc patterns within the loop are not definitely connected, if we use a small tolerance,
	//    the system may think these line/arc patterns cannot compose the loop. so this function use "dConnectTol=0" to clamp the loop tightly.
	// 2) loop node count will greater than 1.
	// 3) when import the external data, need to call this.
	// 4) �������û����Ҳû�и���ͼ�ζ���Ļ��档
	static void ClampPolygonLoop(PolygonPatternLoopPtr pPolygonPatternLoop, double dConnectTol);
	// �������λ�·֮ǰ�Ǳպϵģ�֮ǰ���ܾ����ˡ�ȥ��Сͼ�Ρ����Ӷ����ܲ��պ��ˣ���������������պϡ�
	// ע������������ֻ������Բ����������ֱ�ߣ���һ�����ǵĶ˵���������ᵼ��������ͼ���ص����õ�ʱ����Ҫ֪�����������
	static void ClampClosePolyLoop(PolygonPatternLoopPtr pPolygonPatternLoop);

	// �ҳ����Ż�·�еıպϻ�·��
	// ע:
	//   1) ��������һ����״��6���Ŀ��Ż�·�����ǿ�����ȡ��һ���պϻ�·����ȡ���պϻ�·��ʣ�µĲ��ֽ��������µĻ�·��
	//   2) �������û����Ҳû�и���ͼ�ζ���Ļ��档
	static void FindCloseInOpenLoop(PatternLoopListPtr pPatternLoopList, double dConnectTol);

    // �������˹�ϵ�ж�Ҫ��Ҫ�ֲ�ɶ�������
    static bool NeedToSeperate(const LoopTopologyItemList* pLoopTopItemList);

	// �õ�������ͼ�����ɫ������Ϣ��ȡ���ǵ�һ��ͼ���������Ϣ
	static CString GetLayerName_of_loop(const IPatternLoop* pPatternLoop);
	static int GetColorIndex_of_loop(const IPatternLoop* pPatternLoop);

private:
	// calc the dir factor of two points.
	static void CalcTwoPtDir(Point2D prePt, Point2D postPt, double& dTanX, double& dTanY);

	// separate the loop.
	/* ������
	*  bCutS: �Ƿ��S�͵Ĳ����п���
	*  bLine_has_similarLength: segment�е��ߵĳ����ǲ�����Ҫ����೤����������10������ÿ��Բ���Լ�����һ��segment
	*/
	// ע:
	//   1) one arc will be one "segment".
	//   2) the returned vector will follow the loop order.
	//   3) the geometry like "v" will be separated to 2 segments because the angle of two nodes at vertex
	//      is not close to 180. so the geometry like "u" will not be separated.
	static std::vector<PatternLoopNodeListPtr> SeparateLoop_1(PolygonPatternLoopPtr pPolygonPatternLoop, BOOL bCutS, BOOL bLine_has_similarLength);
    static std::vector<LineArc2DListPtr> SeparateLoop_2(PolygonPatternPtr pPolygonPat, BOOL bCutS, BOOL bLine_has_similarLength);

	// �ѡ���·���������ŵ����˼����С�
	// ע��
	// 1) �������˹�ϵ���ѡ�pLoopTopItem�����뵽��pLoopTopItemList���к��ʵ�λ�á�
	// 2) loopOpenInfo:<��������ID, �ü��������ǿ��ŵ���>
	static void AddLoopToTop(LoopTopologyItemListPtr pLoopTopItemList, LoopTopologyItemPtr pLoopTopItem, const std::map<LONGLONG, BOOL>& loopOpenInfo);
};

END_CUTLEADER_NAMESPACE()
