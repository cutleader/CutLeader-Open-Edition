#pragma once

#include "PatternBase.h"
#include "boost/enable_shared_from_this.hpp"


DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(IPatternVisitor)

BEGIN_CUTLEADER_NAMESPACE()


// �����pattern
// ע��
//  1) PatternBase���е�base pt��base vector����಻�á�
//  2) һ��polygon pattern�Ϳ����γ�һ���ջ�����������ջ����������Ϊ0��
//  3) polygon pattern�е�line/arcs�ǰ���˳����β�����ġ�
//  4) Ŀǰ�����ֻ���ڵ���ͼ�εĹ����С��Ż�֮ǰ���������Լ���pattern�������Ӷ������Լ�ڴ棬һ���Ż�������Ҫ���������ת��֮ǰ�����ݽṹ��
//  5) Ŀǰ������󲻱��棬Ҳ���ܼӵ�pattern loop��ȥ��
class ClGeometryFeature_Export PolygonPattern : public PatternBase, public boost::enable_shared_from_this<PolygonPattern>
{
public:
	PolygonPattern();
	PolygonPattern(LineArc2DListPtr pConnectedGeom2DList);
	virtual ~PolygonPattern(void);

public: // functions from IData interface.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;
	virtual IDataPtr Clone() const override;

public:  // functions from IPattern interface.
	virtual PatternType GetPatternType() const override { return PATTERN_POLYGON; }
	virtual double DistanceTo(const Point2D& pt) const override;
	virtual void GetSnapPoints(Point2DList& snapPts) const;
	virtual Point2D GetStartPoint() const override;
	virtual void SetStartPoint(const Point2D& pt) override; // adjust the start point to "pt", not change other points.
	virtual Point2D GetEndPoint() const override;
	virtual void SetEndPoint(const Point2D& endPt) override; // adjust the end point to "pt", not change other points.

	virtual void UpdateFrom(IPatternPtr pPattern) override;
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override; // �������ϸ���жϣ��������������Ǿ���Ϊ�ظ���

	virtual void Accept(IPatternVisitorPtr patVisitor) override;

	virtual LineArc2DListPtr Calculate2DLineArcs() const override;

public:
    LineArc2DListPtr GetConnectedGeom2DList() const { return m_pConnectedGeom2DList; } // ��Ҫ���������ֵ�����Ԫ�أ�����ֵ��������ֻ���ĳ���

    void SetIsFromOdDb2dPolyline(bool b) { m_bFromOdDb2dPolyline = b; }
    void SetIsOpenOdDb2dPolyline(bool b) { m_bOpenOdDb2dPolyline = b; }
    void SetStartPt_of_OdDb2dPolyline(const Point2D& pt) { m_startPt_of_OdDb2dPolyline = pt; }

public:
    // ���������ӽڵ㣬��Ҫ��֤������ǰ���˳����β������ͼ�η���һ�µġ�
    // ע��
    //  1) �����������ǣ���˳���һ�ζ���β������ͼ���ͽ�����ͼ�η�����Բ�һ�£����������Ƚ��ң��Ƚ��Ѵ����������������ܳ����Խ������·�����⡣
    void AddGeom2D_2_PolygonPattern(ILineArc2DPtr pGeom2D);
	// ������������溯���߼�һ����pGeom2DList�Ǹ������������������һ��ͼ�ξ͸о�������ĺ���һ���ˡ�
	void AddGeom2DList_2_PolygonPattern(LineArc2DListPtr pGeom2DList);

    // ������������2����β������ֱ�߻�����ȫ�ص�������Բ��������true
    bool IsTwoOverlappedGeom2Ds() const;

	// �н�����Σ�һ�����ڵ���ʱ��֮ǰ���ܶ˵㲻�Ǿ�����ȡ�����û�и��»��档
	void ClampPolygonPattern(double dConnectTol);

	// �Ƿ��ǱպϵĶ����
	bool IsClosedPolygon(double dConnectTol) const;

    // ����Ч�ʵĵط����������
    const Point2D& GetStartPoint_byRef() const;
    const Point2D& GetEndPoint_byRef() const;

private:
    // ��β������line/arc
    LineArc2DListPtr m_pConnectedGeom2DList;

    // ���pattern�ǲ�������OdDb2dPolyline���⼸��������Ϊ���ж�OdDb2dPolylineը����ĵ�һ���ߵ�����ǲ��Ǻ�OdDb2dPolyline�����һ����
    // Ҫ�ǲ�һ���Ļ��Ǿ����յ��OdDb2dPolyline�����һ������ô��һ���ߵķ�����Ҫ��һ�¡�
    bool m_bFromOdDb2dPolyline;
    bool m_bOpenOdDb2dPolyline; // �ǲ��Ǹ����ŵ�OdDb2dPolyline
    Point2D m_startPt_of_OdDb2dPolyline; // OdDb2dPolyline�����
};

END_CUTLEADER_NAMESPACE()
