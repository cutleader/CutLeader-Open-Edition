#pragma once

#include "clGeometryBaseCommon.h"
#include "Point2D.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

// �㼯�ϡ�
class ClGeometryBase_Export Point2DList : public std::vector<Point2D>
{
public:
	Point2DList(void);
	~Point2DList(void);

public:
	// whether the point exists in the list.
	BOOL IsExist(const Point2D& pt, double dTol = GEOM_TOLERANCE) const;

	// �ҵ����롰pt������ĵ㡣
	Point2D GetPtClosestTo(const Point2D& pt) const;
	int GetPtIdxClosestTo(const Point2D& pt) const;

	// get the index in point list.
	int GetPtIndex(const Point2D& pt) const;

	// get the point in the left
	int GetLeftPtIndex() const;

	// get the left-lower/higher point.
	int GetLeftLowerPtIdx() const;
	int GetLeftHigherPtIdx() const;

	// get the right-lower/higher point.
	int GetRightLowerPtIdx() const;
	int GetRightHigherPtIdx() const;

	// get the low-lefter/righter point.
	int GetLowLefterPtIdx() const;
	int GetLowRighterPtIdx() const;

	// get the high-lefter/righter point.
	int GetHighLefterPtIdx() const;
	int GetHighRighterPtIdx() const;

	// �õ�һ���㡣��һ���������˱߽�����ڶ�������û�У������ڶ�������Ч�ʸ��ߡ�
	Point2D at(int iPtIndex) const {
		return vector<Point2D>::at(iPtIndex % (int)size());
	}
	const Point2D& at_with_safe_index(int iPtIndex) const {
		return vector<Point2D>::operator [](iPtIndex);
	}

	// fill the vector to "iSize".
	// notes: use the default Point2D object to fill the container.
	void Fill(int iSize);

	// �����е����任��
	void Transform(const Matrix2D& mat);

	// ɾ���㡣
	void DeletePt(const Point2D& pt);
};

END_CUTLEADER_NAMESPACE()
