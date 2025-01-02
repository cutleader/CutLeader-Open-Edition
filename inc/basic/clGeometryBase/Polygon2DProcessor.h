#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Path2D.h"
#include "AlgData.h"
#include <list>

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)

BEGIN_CUTLEADER_NAMESPACE()


// the result of the 2d polygon boolean operation, ��ά����β�������Ľ����
enum PolygonBooleanType
{
	PolygonBoolean_Succeed 			= 0, // succeed
	PolygonBoolean_TryAgain 		= 1, // please try again.
	PolygonBoolean_Error 			= 2, // error
};


// this class will process 2d polygon����������������ά����ζ���
class ClGeometryBase_Export Polygon2DProcessor
{
public:
	/************************************************************************/	
	// optimize the polygon.

	// notes:
	//   1) two adjacent lines in the polygon can be merged.
	//   2) make the polygon has less vertexes.
	// params:
	//   1)  bOuter: whether used for outer boundary.
	static void SleekPolygon(Polygon2DPtr pPolygon2D, double dOptPolyFactor, BOOL bOuter = TRUE);

	// notes:
	//   1) this function will make the polygon more simple(have less vertexes),
	//      so in some case, the result polygon may be anamorphic with the 
	//      original one.
	//   2) this function also call SleekPolygon function.
	static void OptimizePolygon(Polygon2DPtr pPolygon2D, double dOptPolyFactor);
	/************************************************************************/


	/************************************************************************/
	// enlarge or compress the polygon

	// enlarge the polygon with "dVal".
	static void EnlargePolygon(Polygon2DPtr pPolygon2D, double dVal, double dCutArcFactor, double dOptPolyFactor);

	// this function is the reverse function of EnlargePolygon.
	// notes: 
	//   1) returns the polygons after compressing "pPolygon2D". if the returned polygons contains no items, fails.
	static Polygon2DListPtr CompressPolygon(Polygon2DPtr pPolygon2D, double dVal);

	// compress "pPolyList" with "dVal".
	static Polygon2DListPtr CompressPolygon(Polygon2DListPtr pPolyList, double dVal, double dOptPolyFactor);
	/************************************************************************/


	/************************************************************************/
	// merge polygons.
	
	// merge the polygons in "pPolyList".
	static Polygon2DPtr MergePolygon(Polygon2DListPtr pPolyList, double dOptPolyFactor, BOOL bNeedSleek = TRUE);

	// merge "pPolygon2D1" with "pPolygon2D2".
	static Polygon2DPtr MergePolygon(Polygon2DPtr pPolygon2D1, Polygon2DPtr pPolygon2D2);

	// ������Щ����Σ����ཻ�Ĺ������һ��
	// ע��polygonRectsΪ�������Ӿ��λ��棬Ϊ�յĻ��Ͳ���������档
	static std::vector<Polygon2DListPtr> MergePolyEx(Polygon2DListPtr pPolyList, const std::map<LONGLONG, Rect2D>& polygonRects);
	/************************************************************************/


	/************************************************************************/
	// about subtract polygon.

	// subtract "pPolygon2" from "pPolygon1" and return the result polygons.
	// notes:
	//   1) if failed, return empty list.
	static std::vector<Polygon2D> SubtractPolygon(const Polygon2D& polygon1, const Polygon2D& polygon2);

	//
	static PolygonBooleanType SubtractPolygon(std::vector<Polygon2D>& polygonList, const Polygon2D& polygon);

	// �ӡ�pPolygon2D1�����е��͡�pPolygon2D2���ཻ�Ĳ��֡�
	// ע��
	//  1) �ѡ�pPolygon2D2�������һ��ģ�ߣ�����pPolygon2D1������һѹ����pPolygon2D1����ѹ�������Ĳ��ּ�����Ҫ�ġ�
	//     ��Ҫ�á�pPolygon2D1��ѹ��pPolygon2D2��һ�Σ������Ų���©������Ρ�
	//  2) ����и�����ʧ�ܣ����ؿյļ��ϡ�
	static Polygon2DListPtr CutOffPolygon(const Polygon2D* pPolygon2D1, const Polygon2D* pPolygon2D2, const Line2DList* pLineList1, Line2DList* pLineList2);
	/************************************************************************/


	/************************************************************************/
	// about NFP.

	// build the NFP of "pFixedPoly" and "pMovingPoly".
	// notes:
	//   1) "pPolygon2D" should be allocated by the caller.
	static void BuildNFP(const Polygon2D* pFixedPoly, const Polygon2D* pMovingPoly, Polygon2DPtr pPolygon2D, BOOL bCheckResult = TRUE);

	// build the internal NFP of the fixed and moving polygon at the reference point.
	//  ע��
	//   1) �����ơ�����һ���������������õ���һ�����ٽ�������һ����͹���ӣ����������͹̨��ĳ���һ���ߣ�
	//      ��ʱ���͹̨�޷�ͨ������η��أ�ֻ�ܷ���һ�������Σ�����ߵ�͹̨ͨ����retLines�����һ���߷��ء�
	static Polygon2DListPtr BuildInnerNFP(const Polygon2D* pFixedPoly, const Line2DList* pFixPolyEdges, const Polygon2D* pMovingPoly, const Line2DList* pMovePolyEdges, const Point2D& refPt);

	// get a point on NFP, at this point, the area composed by "pFixedPoly" and "pMovingPoly" is min.
	static Point2D GetMinPairAreaPos(Polygon2DPtr pFixedPoly, Polygon2DPtr pMovingPoly, Polygon2DPtr pNFPPoly, double dSpacing);
	/************************************************************************/


	/************************************************************************/
	// ���ڶ���κ�ֱ�ߵ��ཻ��

	// go through the polygon "pPoly" with a ray which define with "startPt" and "dCosA/dSinA".
	static void ThroughPolygon(Polygon2DPtr pPoly, Point2D startPt, double dCosA, double dSinA, std::list<Dist>& distList);

	// �õ�ֱ�ߺͶ���εĽ��㣬û�н��㷵�ؿ�������
	/* ������
	*  basePt, iDir: ֱ�ߵ��������췽��
	*  bSort: �Ƿ�ѽ����������߷�������
	*/
	static Point2DListPtr GetCrossPtsWithLine(const Polygon2D* pPoly, const Point2D& basePt, POSITION_INFO iDir, BOOL bSort = TRUE);
	/************************************************************************/


	// load/save polygon.
	static void SavePolygon(Polygon2DPtr pPoly, CString strPath);
	static Polygon2DPtr LoadPolygon(CString strPath);

	// �Ѷ�����е�ճ�ŵ�ֿ���
	/* ������
	*  dDis: ճ�ŵ㱻�����ľ��롣
	*/
	// ע:
	//   1) ������������������ֻ��һ��ճ�ŵ㡣 
	static void SeperateStickPt(Polygon2D& poly, double dDis);

	// get the edge indexes in "pPoly1" which is common with "pPoly2".
	static std::vector<int> GetCommonEdges(Polygon2DPtr pPoly1, Polygon2DPtr pPoly2);

	// ��ʼ���������Ҫ�õ���һЩ��̬������
	static void Init(BOOL bSleekCtrl, double dSleekRate);

	// ȥ��������еļ�̡���ν��̾���һ����һ��������ߴ̳��������Ρ�����ζ�����С��4ʱ������
	static void RemoveSpike_of_polygon(Polygon2D& poly);

private:
	// ���������ǲ�����Ҫ����˳��
	static BOOL CanSleek(Point2D pt1, Point2D pt2, Point2D pt3, double dLongEdge);

private:
	// ������Щ�߲�����˳������SheetEditParam�е�ע�͡�
	static BOOL m_bSleekCtrl;
	static double m_dSleekRate;
};

END_CUTLEADER_NAMESPACE()
