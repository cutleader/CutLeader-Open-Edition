#pragma once

#include "clNestJobCommon.h"
#include <map>

DECLARE_CUTLEADER_CLASS(Polygon2D)

BEGIN_CUTLEADER_NAMESPACE()

// ��¼�����������ݡ�
class AreaInfo
{
public:
	AreaInfo() { dAllPartArea = .0; }

	// ������������<part id, part area>��������������ͬ���ľ��ȼ���ġ�
	std::map<__int64, double> partAreaData;

	// �����������߿�<part id, part outer poly>���������߿�����ͬ���ľ��ȼ���ġ�
	std::map<__int64, Polygon2DPtr> partOutPolyData;

	// �������
	double dAllPartArea;
};

END_CUTLEADER_NAMESPACE()