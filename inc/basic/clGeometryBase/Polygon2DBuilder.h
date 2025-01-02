#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Path2D.h"
#include <vector>

// declare classes.
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)
DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
//

BEGIN_CUTLEADER_NAMESPACE()

// ��Բ�����ɶ���εĲ���
enum BuildPolyFromArc_Policy
{
	BuildPolyFromArc_diffPolicy4DiffDirArc	= 0, // ˳�����满�ò�ͬ����ɢ�����������ʽ�ƺ���Ҫ�ģ��Ű�����п��ܲ�������Σ��������ص���˳�����满����ʱ���ɵĶ�����Ҫ������ȫ�غϡ�
	BuildPolyFromArc_samePolicy4DiffDirArc	= 1, // ˳�����满����ͬ����ɢ����
};

// this class will build 2d polygon����������������ά����ζ���
// notes:
//   1) be clear of the difference between the polygon and the hull.
class ClGeometryBase_Export Polygon2DBuilder
{
public:
	/************************************************************************/
	// build from line/arcs.
	
	// build the polygon hull using line/arcs.
	// notes:
	//   1) we will return a hull.
	//   2) if "pGeo2DList" only have one item which is a line or "line-like" arc, before call this, caller should use LineArc2DProcessor::ExpandLineArcToRect to expand it.
	//   3) this function guarantee that the returned polygon is CCW.
	static Polygon2DPtr BuildHullFromLineArc(const LineArc2DList* pGeo2DList, double dCutArcFactor, BuildPolyFromArc_Policy iPolicy = BuildPolyFromArc_diffPolicy4DiffDirArc);

	// build the polygon using line/arcs.
	/* ������
	*  bNeedCCWPoly: �Ƿ񷵻���ʱ�뷽��Ķ���Σ���Ȼ���ֵ��FALSEʱҲ��������ʱ�뷽��Ķ���Ρ�
	*/
	// notes:
	//   1) "pGeo2DList" should connected "head-tail" as a loop.
	static Polygon2DPtr BuildPolyFromLineArc(LineArc2DListPtr pGeo2DList, BuildPolyFromArc_Policy iPolicy, double dCutArcFactor, BOOL bNeedCCWPoly = TRUE);

	// build the polygons using line/arcs.
	// notes:
	//   1) we may find several polygons from "pGeo2DList".
	//   2) "pGeo2DList" may not follow an order.
	static Polygon2DListPtr BuildPolyListFromLineArc(const LineArc2DList* pGeo2DList, double dCutArcFactor, BuildPolyFromArc_Policy iPolicy = BuildPolyFromArc_diffPolicy4DiffDirArc);
	/************************************************************************/

	/************************************************************************/
	// build from line.

	// build the polygon using lines.
	// notes:
	//   1) we need the boundary polygon, NOT the hull.
	//   2) caller should make sure that "pLineList" should have 3 items at least.
	//   3) "pPolygon2D" should be allocated by caller.
	//   4) if "pLineList" are connected as a closed loop, "pLineList" should have same direction, return TRUE. otherwise return FALSE.
	//   5) this function do not guarantee that the returned polygon is CCW.
	static BOOL BuildPolyFromLine(Line2DListPtr pLineList, Polygon2DPtr pPolygon2D);

	// build polygon list using lines.
	//  ע��
	//   1) ����һ���������������õ���һ���������һ����͹���ӣ����������͹̨��ĳ���һ���ߣ�
	//      ��ʱ���͹̨�޷�ͨ������η��أ�ֻ�ܷ���һ�������Σ�����ߵ�͹̨ͨ����retLines�����һ���߷��ء�
	static Polygon2DListPtr BuildPolyListFromLine(const Line2DList* pLineList, std::vector<Line2DPtr>& retLines, BOOL bShowDebug = FALSE);

	// find a boundary from "pLineList".
	// notes:
	//   1) "pPolygon2D" should be allocated by caller.
	//   2) illustrate:  run from "startPt", along the "most" left path.
	/*                                                
		   +-------------------------+            +-------------------------+  
		   |                         |            |                         |  
		   |                    +----------+      |                    +-----  
		   |                    |    |     |      |                    |       
		   |                    |    |     |      |                    |       
		   |                    +----------+      |                    +-----  
		   |                         |            |                         |  
		   |                         |            |                         |  
		   |                +--------------+      |                +---------  
		   |                |        |     |      |                |           
		   +-----*----------|--------+     |      +----------------|           
			 start point    |              |              CCW                  
							+--------------+                                   
	*/
	static BOOL FindBoundaryPoly(Line2DListPtr pLineList, const Point2D& startPt, Polygon2DPtr pPolygon2D);

	// �õ�2������εĽ�������Σ�pLineList����2���ཻ�Ķ���Ρ�
	static Polygon2DPtr FindInnerBoundary(Line2DListPtr pLineList, BOOL bShowDebug = FALSE);
	/************************************************************************/

	// build polygon from arc, we convert the arc to a polygon.
	// notes:
	//   1) if the sweep angle is "very" small, we can convert it to one line.
	//   2) if we only convert an arc and it is "very" small, before call this, caller should use LineArc2DProcessor::ExpandLineArcToRect to expand it.
	/* params:
	*   bNeedEndPt: whether add the end point of "pArc" to polygon.
	*   bCCW: the returned polygon will set to CCW if the point count in Polygon is greater than 2.
	*/
	static Polygon2DPtr BuildPolyFromArc(const Arc2D* pArc, BuildPolyFromArc_Policy iPolicy, double dCutArcFactor, BOOL bNeedEndPt = FALSE, BOOL bCCW = TRUE);

	// build polygon from point list.
	// notes:
	//   1) caller should make sure that the count of the points is greater than 2.
	//   2) return the hull of all these points. so maybe some points will be filtered out.
	//   3) this function guarantee that the returned polygon is CCW.
	static Polygon2DPtr BuildHullFromPtList(const Point2DList* pPtList);
};

END_CUTLEADER_NAMESPACE()
