#pragma once

#include "clCanvasGeometryUICommon.h"
#include "Rect2D.h"
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(CanvasZone)
DECLARE_CUTLEADER_CLASS(IntegerList)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ����
// ע��
//  1) ��ͼ������һЩ��ͼ�ϵ���ͨ�������Ǳ���ͼ�ϵ�������������ͼ����϶�������������һ��������������Щ����ͼ����֮����ʵ�������˹�ϵ�ġ�ȥ�����ĵ���
class CanvasZone
{
public:
	CanvasZone(void);
	~CanvasZone(void);

public:
	void SetOuterPoly(Polygon2DPtr pOuterPoly) { m_pOuterPoly = pOuterPoly; }
	Polygon2DPtr GetOuterPoly() const { return m_pOuterPoly; }

	void SetPatLoopID4OuterPoly(LONGLONG iPatLoopID4OuterPoly) { m_iPatLoopID4OuterPoly = iPatLoopID4OuterPoly; }
	LONGLONG GetPatLoopID4OuterPoly() const { return m_iPatLoopID4OuterPoly; }

	Polygon2DListPtr GetInnerPolys() const { return m_pInnerPolys; }
	void SetInnerPolys(Polygon2DListPtr pInnerPolys) { m_pInnerPolys = pInnerPolys; }

	IntegerListPtr GetPatLoopIDs4InnerPolys() const { return m_pPatLoopIDs4InnerPolys; }
	void SetPatLoopIDs4InnerPolys(IntegerListPtr pPatLoopIDs4InnerPolys) { m_pPatLoopIDs4InnerPolys = pPatLoopIDs4InnerPolys; }

private:
	// ��ͼ�������������
	// ע��
	// ����������ͼ�������ڵ�ͼû�в��ϱ߿������������һ���ܴ�ܴ�Ķ���Σ���m_iPatternLoopIDΪ-1��
	Polygon2DPtr m_pOuterPoly;
	LONGLONG m_iPatLoopID4OuterPoly; // ��ͼ������������Ӧ�ļ���������

	// ��ͼ�������������
	Polygon2DListPtr m_pInnerPolys;
	IntegerListPtr m_pPatLoopIDs4InnerPolys; // ��ͼ������������Ӧ�ļ���������
};

END_CUTLEADER_NAMESPACE()
