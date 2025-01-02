#pragma once

#include "PatternBase.h"
#include "boost/enable_shared_from_this.hpp"


DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(PolygonPattern)
DECLARE_CUTLEADER_CLASS(IPatternVisitor)

BEGIN_CUTLEADER_NAMESPACE()


// 多边形pattern
// 注：
//  1) PatternBase类中的base pt和base vector这个类不用。
//  2) 一个polygon pattern就可能形成一个闭环，不过这个闭环的面积可能为0。
//  3) polygon pattern中的line/arcs是按照顺序、首尾相连的。
//  4) 目前这个类只用于导入图形的过程中、优化之前，这样可以减少pattern数量，从而大幅节约内存，一旦优化结束需要把这个数据转到之前的数据结构。
//  5) 目前这个对象不保存，也不能加到pattern loop中去。
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
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const override; // 不是做严格的判断，不过这样子我们就认为重复了

	virtual void Accept(IPatternVisitorPtr patVisitor) override;

	virtual LineArc2DListPtr Calculate2DLineArcs() const override;

public:
    LineArc2DListPtr GetConnectedGeom2DList() const { return m_pConnectedGeom2DList; } // 不要往这个返回值中添加元素，返回值尽量用于只读的场景

    void SetIsFromOdDb2dPolyline(bool b) { m_bFromOdDb2dPolyline = b; }
    void SetIsOpenOdDb2dPolyline(bool b) { m_bOpenOdDb2dPolyline = b; }
    void SetStartPt_of_OdDb2dPolyline(const Point2D& pt) { m_startPt_of_OdDb2dPolyline = pt; }

public:
    // 往多边形添加节点，需要保证多边形是按照顺序、首尾相连、图形方向一致的。
    // 注：
    //  1) 期望的输入是：按顺序把一段段首尾相连的图形送进来（图形方向可以不一致）。如果输入比较乱，比较难处理的完美，比如可能出现自交、多回路等问题。
    void AddGeom2D_2_PolygonPattern(ILineArc2DPtr pGeom2D);
	// 这个函数和上面函数逻辑一样，pGeom2DList是个开放轮廓，把它想成一个图形就感觉和上面的函数一样了。
	void AddGeom2DList_2_PolygonPattern(LineArc2DListPtr pGeom2DList);

    // 如果多边形中是2个首尾相连的直线或者完全重叠的两个圆弧，返回true
    bool IsTwoOverlappedGeom2Ds() const;

	// 夹紧多边形，一般用于导入时，之前可能端点不是绝对相等。这里没有更新缓存。
	void ClampPolygonPattern(double dConnectTol);

	// 是否是闭合的多边形
	bool IsClosedPolygon(double dConnectTol) const;

    // 讲究效率的地方调用这个。
    const Point2D& GetStartPoint_byRef() const;
    const Point2D& GetEndPoint_byRef() const;

private:
    // 首尾相连的line/arc
    LineArc2DListPtr m_pConnectedGeom2DList;

    // 这个pattern是不是来自OdDb2dPolyline，这几个数据是为了判断OdDb2dPolyline炸开后的第一个线的起点是不是和OdDb2dPolyline的起点一样，
    // 要是不一样的话那就是终点和OdDb2dPolyline的起点一样，那么第一个线的方向需要反一下。
    bool m_bFromOdDb2dPolyline;
    bool m_bOpenOdDb2dPolyline; // 是不是个开放的OdDb2dPolyline
    Point2D m_startPt_of_OdDb2dPolyline; // OdDb2dPolyline的起点
};

END_CUTLEADER_NAMESPACE()
