#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ��ʵ�����Ļ������ݡ�
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
	// ͼ��ʵ������Χ���Ρ�
	Rect2D m_rect;

	// ͼ��ʵ���Ļ���ͼ�Ρ�
	LineArc2DListPtr m_pGeom2DList;
};

END_CUTLEADER_NAMESPACE()
