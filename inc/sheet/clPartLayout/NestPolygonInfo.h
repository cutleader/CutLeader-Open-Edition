#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include "ClData.h"

// declare class.
DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(NestPolygonInfo)
//

BEGIN_CUTLEADER_NAMESPACE()

// ���Ŷ���ε���Ϣ��
class ClPartLayout_Export NestPolygonInfo : public ClData
{
public:
	NestPolygonInfo(Polygon2DPtr pNestPoly, Rect2D polyRect);
	~NestPolygonInfo(void);

public: // get/set functions.
	void SetNestPoly(Polygon2DPtr pNestPoly) { m_pNestPoly = pNestPoly; }
	Polygon2DPtr GetNestPoly() { return m_pNestPoly; }

	void SetPolyRect(Rect2D polyRect) { m_polyRect = polyRect; }
	Rect2D GetPolyRect() { return m_polyRect; }

public:
	// ��ǰ�����ǲ���Ӧ�����ڡ�pNestPolygonInfo��ǰ�档
	// ע��
	//  1) �øö���ε���Ӿ������ж�λ�á�
	//  2) �Ⱥ�˳��Ҫ�ο����ߵ����䷽��������������գ���ôλ�øߵ�����ǰ�档
	static bool BeforeTo4Bottom(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);
	static bool BeforeTo4Top(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);
	static bool BeforeTo4Left(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);
	static bool BeforeTo4Right(NestPolygonInfoPtr pNestPolygonInfo1, NestPolygonInfoPtr pNestPolygonInfo2);

private:
	// ���Ű�λ�õĶ���Ρ�
	Polygon2DPtr m_pNestPoly;

	// �ö���ε���Ӿ��Ρ�
	Rect2D m_polyRect;
};

END_CUTLEADER_NAMESPACE()