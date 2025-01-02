#pragma once

#include "clGeometryFeatureCommon.h"
#include "Rect2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ�Ρ��Ļ������ݡ�
class ClGeometryFeature_Export PatternCache
{
public:
	PatternCache(void);
	~PatternCache(void);

public: // get/set functions.
	void SetRect(Rect2D patRect) { m_patRect = patRect; }
	const Rect2D& GetRect() const { return m_patRect; }

	void SetBaseGeom(LineArc2DListPtr pGeom2DList) { m_pGeom2DList = pGeom2DList; }
	LineArc2DListPtr GetBaseGeom() const { return m_pGeom2DList; }

public:
	// �任����������ɵ�����Щ�������»��棬���������ɻ���Ҫ��Ч���١�
	void Transfer(const Vector2D& transferVect);
	void Transform(const Matrix2D& transformMatrix);

private:
	// the rect of the pattern.
	Rect2D m_patRect;

	// ���������Ķ�άֱ��Բ����
	// ע��
	// 1) ����ֱ�ߺ�Բ������������ͼ������ȫһ�µġ�
	LineArc2DListPtr m_pGeom2DList;
};

END_CUTLEADER_NAMESPACE()
