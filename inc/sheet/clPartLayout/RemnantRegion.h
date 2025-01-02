#pragma once

#include "clPartLayoutCommon.h"
#include "clCutFeatureCommon.h"
#include "Rect2D.h"
#include <vector>
#include <tuple>

DECLARE_CUTLEADER_CLASS(Polygon2D)

BEGIN_CUTLEADER_NAMESPACE()

// ��������
// ע��
//  1) ����������һЩ���ϣ����������Щ�����߿����ǽ���������߿򡱣��ڿ����ǽС������ڿס���
//  2) �Ѱ�������ŵ����ȥ����ʣ�µ���һ��顰�������򡱣���Щ����������֮����ʵ�������˹�ϵ�ġ�
class ClPartLayout_Export RemnantRegion
{
public:
	RemnantRegion(void);
	~RemnantRegion(void);

public: // get/set functions.
	void SetOuterPoly(Polygon2DPtr pOuterPoly) { m_pOuterPoly = pOuterPoly; }
	Polygon2DPtr GetOuterPoly() const { return m_pOuterPoly; }

	void SetOuterRect(Rect2D outerRect) { m_outerRect = outerRect; }
	const Rect2D& GetOuterRect() const { return m_outerRect; }

	void SetHoleRegions(std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>> holeRegions) { m_holeRegions = holeRegions; }
	const std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>>& GetHoleRegions() const { return m_holeRegions; }

	void IsOutRegion(BOOL bOutRegion) { m_bOutRegion = bOutRegion; }
	BOOL IsOutRegion() const { return m_bOutRegion; }

private:
	// ������߿򡱡�
	Polygon2DPtr m_pOuterPoly;

	// ������߿򡱵���Ӿ��Ρ�
	// ע��
	//  1) ������Ӿ�����Ϊ�˺����ʹ�á�
	Rect2D m_outerRect;

	// �������ڿס���<�����ڿ׶���Σ������ڿ���Ӿ��Σ��γ�����ڿ׵��������������ĵ��߳ߴ�, �γ�����ڿ׵����������ʹ�õ������з�ʽ>
	std::vector<std::tr1::tuple<Polygon2DPtr, Rect2D, double, PatternLoopCutOffsetType>> m_holeRegions;

	// �ǲ���������ġ��������򡱡�
	// ע��
	//  1) ������������е��ر��������˲��ϵı߿�
	BOOL m_bOutRegion;
};

END_CUTLEADER_NAMESPACE()
