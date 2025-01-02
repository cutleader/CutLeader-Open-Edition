#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// “图形实例”的缓存数据。
class ClPartLayout_Export PatternInstanceCache
{
public:
	PatternInstanceCache(void);
	~PatternInstanceCache(void);

public: // get/set functions.
	void SetRect(const Rect2D& rect) { m_rect = rect; }
	Rect2D GetRect() const { return m_rect; }

	void SetBaseGeom(LineArc2DListPtr pGeom2DList) { m_pGeom2DList = pGeom2DList; }
	LineArc2DListPtr GetBaseGeom() const { return m_pGeom2DList; }

private:
	// 图形实例的外围矩形。
	Rect2D m_rect;

	// 图形实例的基本图形。
	LineArc2DListPtr m_pGeom2DList;
};

END_CUTLEADER_NAMESPACE()
