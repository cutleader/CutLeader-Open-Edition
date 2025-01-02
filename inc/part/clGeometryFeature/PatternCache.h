#pragma once

#include "clGeometryFeatureCommon.h"
#include "Rect2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// “图形”的缓存数据。
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
	// 变换几何特征后可调用这些函数更新缓存，比重新生成缓存要高效不少。
	void Transfer(const Vector2D& transferVect);
	void Transform(const Matrix2D& transformMatrix);

private:
	// the rect of the pattern.
	Rect2D m_patRect;

	// 几何特征的二维直线圆弧表达。
	// 注：
	// 1) 对于直线和圆弧几何特征，图形是完全一致的。
	LineArc2DListPtr m_pGeom2DList;
};

END_CUTLEADER_NAMESPACE()
