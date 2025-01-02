#pragma once

#include "clGeometryBaseCommon.h"
#include <vector>
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(Point2DList)

BEGIN_CUTLEADER_NAMESPACE()

// the processor for 2d line/arc, ��άֱ��/Բ���Ĵ������ࡣ
class ClGeometryBase_Export LineArc2DProcessor
{
public:
	// expand the line or the "line-like" arc to a similar rectangle if "pGeo2DList" only has one item.
	// notes:
	//   1) after calling this, we can convert this "pGeo2DList" to polygon.
	static void ExpandLineArcToRect(LineArc2DListPtr pGeo2DList);

	// cut lines at the cross points.
	// notes:
	//   1) this function cannot fit all cases, BUT if you input two
	//      crosses lines, this function can get the correct 4 lines.
	static void CutLineByCross(Line2DListPtr pLine2Ds);

	// convert "pArc" to lines.
	// notes:
	//   1) if pArc is very small, return a lineList without items in it.
	static Line2DListPtr DevideArcToLine(const Arc2D* pArc, double dCutArcFactor);

	// adjust the loop to CCW.
	// notes:
	//   1) "pGeo2DList" should connect "head-tail" to a closed loop.
	//   2) if "pGeo2DList" connect "head-tail" as CW, return TRUE.
	static BOOL AdjustLoopToCCW(LineArc2DListPtr pGeom2DList);

	// build points to arc using Least Square Fitting Algorithm.
	/* params:
	*  pPtList: build arc from these points, point count should be greater than 2.
	*  dMaxOffset: the max offset between the "pPtList" to the arc.
	*  bCheckMidPt: ����������ʱ���Ƿ��ǡ���ѡ�㡱���е㡣
	*/
	// ע:
	//   1) if points are on one line, "dMaxOffset" will be a large value.
	//   2) start/end points of the arc will be adjusted to the first/last point in "pPtList".
    //   3) �����ƣ���ϳ�Բ�����е����⡣���У�ͬ����һ��㣬�������ƶ�һ�£���ϳ��Ľ�����ܾͲ�ͬ��
	//      �����İ����������ģ��������������ߣ����ܣ���������ֱ�ߣ����øú���ʱ���ܻ������߽�������
	//      Ҳ���ǻ�����ϳ�һ���������Բ����ʱ�����Բ�����ҵ�ʱ�ܿ��ܴ������߽�������һ���㼯�ƶ��ˣ�
	//      �����ı߽�����Ҳ��ͬ�ˣ����Կ��ܳ��ֲ�ͬ�Ľ����Ŀǰ��ʱû�д���
	static Arc2DPtr BuildPts2Arc(const Point2DList* pSeedPtList, double& dMaxOffset, BOOL bCheckMidPt = FALSE);

    // �õ�����ֱ��/Բ���ϵĵ㣬�����յ�ֱ��/Բ�����ľ�������
    static Point2DListPtr SortPtsOnLine(Line2DPtr pLine2D, Point2DListPtr pPtList);
    static Point2DListPtr SortPtsOnArc(Arc2DPtr pArc2D, Point2DListPtr pPtList);

	// �õ��㵽Բ�������е㡣
	// ע��
	// 1) ���÷���ȷ��pt����Բ���档
	static bool GetTwoTangencyPtsOnCircle(const Point2D& pt, const Point2D& centerPt, double dRadius, Point2D& firstTangencyPt, Point2D& secondTangencyPt);


	/************************************************************************/
	// ���ڻ���ͼ��֮��Ĺ�ϵ��

	// 2������ͼ�μ���֮���ǲ�����ͼ���ཻ����ͼ�μ����е�ͼ���ཻ���㡣
	// ע��
	//  1) �˵�Ӵ�Ҳ����Ϊ�ཻ��
	static BOOL TwoGeomListCross(LineArc2DListPtr pGeo2DList1, LineArc2DListPtr pGeo2DList2);

	// ����ͼ�Ρ�pGeom���Ƿ񱻡�pGeoms���ضϡ�
	// ע��
	//  1) �����pGeom���Ķ˵�Ӵ�����pGeoms��������Ϊ�ǽضϡ�
	static BOOL GeomIsCutByGeoms(ILineArc2DPtr pGeom, LineArc2DListPtr pGeoms);

	// �õ�����ֱ��֮��ľ��롣
	// ע��
	//  1) ����ֱ���ཻ�Ļ�������Ϊ0��
	static double GetTwoLineDis(Line2DPtr pLine1, Line2DPtr pLine2);

	// �õ�ֱ�ߺ�Բ��֮��ľ��롣
	// ע��
	//  1) �����ཻ�Ļ�������Ϊ0��
	static double GetLineArcDis(Line2DPtr pLine, Arc2DPtr pArc);

	// �õ�����Բ��֮��ľ��롣
	// ע��
	//  1) �����ཻ�Ļ�������Ϊ0��
	static double GetTwoArcDis(Arc2DPtr pArc1, Arc2DPtr pArc2);

	// �õ���������ͼ�μ�ľ��롣
	// ע��
	//  1) ��������ͼ���ཻ�Ļ�������Ϊ0��
	static double GetTwoGeomDis(ILineArc2DPtr pGeom1, ILineArc2DPtr pGeom2);

	// 2������ͼ�μ���֮�����̾��롣
	// ע��
	//  1) �õ�pGeoms1�и�������ͼ�ε�pGeoms2�и�������ͼ�εľ��룬������̵ľ���ֵ��
	//  2) ���pGeoms1��ĳЩ����ͼ�κ�pGeoms2��ĳЩ����ͼ���ཻ�������Ϊ0��
	static double GetTwoGeomsDis(LineArc2DListPtr pGeoms1, LineArc2DListPtr pGeoms2);
	/************************************************************************/


	// �㵽�߶εľ���
	static double SquareDistance_ofPoint2Line(const Point2D& pt, const Point2D& startPt, const Point2D& endPt);
	static double SquareDistance_ofPoint2Line_2(double dPtX, double dPtY, double dStartPtX, double dStartPtY, double dEndPtX, double dEndPtY);
};

END_CUTLEADER_NAMESPACE()
