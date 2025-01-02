#pragma once

#include "clGeometryFeatureCommon.h"
#include "baseConst.h"
#include "PatternLoopBase.h"

DECLARE_CUTLEADER_CLASS(PolygonPatternLoop)
DECLARE_CUTLEADER_CLASS(PolygonPattern)

BEGIN_CUTLEADER_NAMESPACE()

// the polygon pattern loop.
// ע:
//   1) control point and connect point, these concepts are for this loop.
//      control point is start/end pt of pattern node, and around connect point, there must be two adjacent pattern nodes.
//   2) loop nodes in the loop follows the loop direction, we can figure out the loop direction by the nodes.
//   3) �����������Ҫ��ֻ��һ��polygon pattern��Ҫ������line/arc pattern��
//   4) ���ڶ����������ֻ��һ��polygon patternʱ���ܶ��Ա����û��ʵ�֣�����Ŀǰ�ò�����
class ClGeometryFeature_Export PolygonPatternLoop : public PatternLoopBase
{
public:
	PolygonPatternLoop();
	virtual ~PolygonPatternLoop();

public: // implement IData interface.
	virtual IDataPtr Clone() const override; // do not clone the pattern within the loop node.

public: // implement IPatternLoop interface.
	virtual void ReverseDirection() override;
	virtual DIRECTION GetDirection() const override;
	virtual Vector2D GetDirectionOnPt(LONGLONG iPatternID, const PatternPosition& patternPosition) const override;
	virtual PatternLoopType GetPatternLoopType() const override { return PatternLoop_Polygon; }
	virtual BOOL IsOpenPath(double dTol) const override;

public:
	/************************************************************************/
	// about add/remove pattern to/from the loop.

	// add pattern directly to the begin/end of this loop.
	// notes: 
	//   1) we should indicate whether the pattern has the same direction with the loop.
	//   2) when we load loops from database, we will call this function to add loop node.
	BOOL AppendPattern(const IPatternPtr pPattern, BOOL bLoopDirection);
	BOOL InsertPattern(const IPatternPtr pPattern, BOOL bLoopDirection);

	// add pattern to the open loop.
	// notes:
	//   1) the open loop only includes line/arc patterns, so "pPattern" will be line/arc pattern.
	BOOL AddPattern(IPatternPtr pPattern, double dTol = SYSTEM_TOLERANCE);

	// delete a pattern directly from the loop.
	void RemovePattern(IPatternPtr pPattern);
	/************************************************************************/


	/************************************************************************/
	// the relations between pattern and loop.

	// check whether start or end point of the open path CAN connect with the start or end point of "pPattern".
	/* params:
	*  dTol: use this to check the equation.
	*/
	// notes: 
	//   1) you know, for close path, we should not call this.
	//   2) "pPattern" is not "closed" pattern.
	BOOL CanLinkWithPat(const IPattern* pPattern, double dTol = SYSTEM_TOLERANCE) const;

	// whether "pPattern" stay at two ends of the open loop. 
	// notes: 
	//   1) call this for open path.
	BOOL PatAtOpenLoopEnd(IPatternPtr pPattern);
	/************************************************************************/


	/************************************************************************/
	// about adjacent patterns.

	// get the two patterns will meet at the connection point.
	// notes:
	//   1) if the loop only has one base geom, do not call this.
	//   2) for open path, "iConnectPtIdx" should not be the first or last connection point.
	//   3) "iPatID1" and "iPatID2" follow the loop direction.
	BOOL GetTwoPatternsAtConnectionPt(int iConnectPtIdx, LONGLONG& iPatID1, LONGLONG& iPatID2) const;

	// get two base geoms which meet at the connection point.
	// notes:
	//   1) if the loop only has one base geom, do not call this.
	//   2) for open path, "connectPt" should not be the first or last connection point.
	//   3) two base geoms follow the loop direction.
	LineArc2DListPtr GetTwoGeomAtConPt(Point2D connectPt);

	// get the angle of the two pattern which meet at "iConnectPtIdx".
	// ע:
	// 1) this function is for close path.
	// 2) the angle is "inside" the pattern loop.
	double GetTwoPatternInsideAngle(int iConnectPtIdx) const;
	double GetTwoPatternInsideAngle_2(int iConnectPtIdx, DIRECTION emDirection) const; // �ú������ȼ���������ķ����ڴ������øýӿ�ʱ�ú������������ơ�

	// get the angle of the two pattern which meet at "iConnectPtIdx".
	// notes:
	//   1) this function is for open path.
	//   2) the angle is at the "left" side of the open loop.
	//   3) caller should make sure "iConnectPtIdx" is corner pt, not an end pt of open path.
	double GetTwoPatLeftAng(int iConnectPtIdx);

	// get the cross point of two pattern.
	// notes: 
	//   1) before call this, caller must make sure that these two pattern is adjacent.
	Point2D GetPatCrossPt(LONGLONG iPatID1, LONGLONG iPatID2) const;
	/************************************************************************/


	/************************************************************************/
	// about control points of the loop.

	// �õ�����������������յ㡣
	// ע��
	// 1) ����������û��ʹ�û������ݡ�
	Point2D GetStartPt() const;
	Point2D GetEndPt() const;
	// ����������ʹ���������м��������Ļ��棬Ч�ʸ��ߡ�
	const Point2D& GetStartPt_byRef() const;
	const Point2D& GetEndPt_byRef() const;

	// get ctrl points of the loop.
	// notes: 
	//   1) please refer to "LineArc2DLoop::GetPointList()".
	//   2) if "bConnection" is TRUE, the start/end points of open path will be dropped.
	Point2DListPtr GetCtrlPtList(BOOL bConnection = FALSE) const;
	/************************************************************************/


	// get the next pattern of "pPattern".
	// notes: 
	//   1) for close path, if "pPattern" is last one, the "next" is the first one.
	//   2) for open path, if "pPattern" is last one, the "next" will be NULL.
	IPatternPtr GetNextPat(IPatternPtr pPattern) const;

	// �õ����Ż�·������ͼ�Σ��������ػ�·�ķ��򡣵����������ʱ��ȷ�������·�ǿ��ŵġ�
	LineArc2DListPtr GetPath_of_openLoop() const;

	// �Ѷ��������ת��Ϊ�����pattern��
	// ע��
	// 1) �ú���û�и��»��档
	PolygonPatternPtr Convert2PolygonPattern();

    // �������λ�·����2����β������ֱ�߻�����ȫ�ص�������Բ��������true��
    bool IsTwoOverlappedGeom2Ds() const;

    // �ö���μ��������ǲ���Բ��
    bool IsCircle() const;

    // �ö���μ��������ǲ���������Ρ�
    bool IsRegularPolygon() const;

private:
	void SetLoopNodes(PatternLoopNodeListPtr pPatternLoopNodeList) { m_pLoopNodeList = pPatternLoopNodeList; }

	// ��ֻ�·�ĸ����ڵ㣬����һ���µĻ�·��
	// ע��
	// 1) �ú���û�и��¼�����������������������Ļ��档
	PolygonPatternLoopPtr SplitAllNodes() const;

	// ��ֻ�·�Ļ��ȴ���90�ȵ�Բ���ڵ�Ϊ4�ȷֽڵ㣬����һ���µĻ�·��
	// ע��
	// 1) �ú���û�и��¼�����������������������Ļ��档
	PolygonPatternLoopPtr SplitArcNodes() const;

	// ��֡���㡱��2������ͼ���е�һ��������ط���Ҫȥ���ĵ�
	// ע��
	// 1) �ú���û�и��¼�����������������������Ļ��档
	PolygonPatternLoopPtr SplitOneArcGeom_of_twoAdjacentGeoms(LONGLONG iArcPatID) const;
};

END_CUTLEADER_NAMESPACE()
