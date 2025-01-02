#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Rect2D.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(ILineArc2D)

BEGIN_CUTLEADER_NAMESPACE()


// ����ͼ�μ��ϡ�
class ClGeometryBase_Export LineArc2DList : public std::vector<ILineArc2DPtr>
{
public:
	LineArc2DList(void);
	~LineArc2DList(void);

public:
	// get line/arcs those are "close" to pt.
	// notes:
	//  1) within the capture range of the specified point.
	//  2) we may not find the point, return the vector that do not contain items.
	LineArc2DListPtr GetLineArcsCloseTo(const Point2D& pt, double dCaptureRange) const;

	// get the cross point that is "closest" to pt, if fails, return NULL.
	Point2DPtr GetCrossClosestTo(const Point2D& pt) const;

	// �õ���Χ���Ρ�
	Rect2D GetRectBox() const;

	// ���任��
	void Transform(const Matrix2D& mat);

	// clone this geom list��
	LineArc2DListPtr Clone() const;

	// reverse the line/arcs.
	void Reverse();

	// �õ���ͼ�μ��ϡ������������롣
	double GetClosestDis(const Point2D& pt) const;

	// �õ����롰pt������ĵ㡣
	Point2D GetClosestPt(const Point2D& pt);

	// �õ�����ͼ�ε��ܳ���
	double GetTotalLength() const;
};

END_CUTLEADER_NAMESPACE()
