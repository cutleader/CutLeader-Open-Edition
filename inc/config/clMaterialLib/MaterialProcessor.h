#pragma once

#include "clMaterialLibCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Polygon2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

class ClMaterialLib_Export MaterialProcessor
{
public:
	// get the area of the material size.
	// notes:
	//   1) for skeleton material, we should deduct the used region.
	static double GetArea(const IMaterialSize* pMaterialSize);

	// �õ����ϵ����
	// ע��
	//  1) ���ص�ͼ��Ԫ����β����һ����·��
	static LineArc2DListPtr GetBorderLineArcs(const IMaterialSize* pMaterialSize);

	// �õ����ϱ߿�Ķ���α�ʾ��
	// ע��
	//  1) �������ʱ�롣
	static Polygon2DPtr GetBorderPoly(const IMaterialSize* pMaterialSize);

	// �õ����ϵ���Ч����
	static LineArc2DListPtr GetUselessRegions(const IMaterialSize* pMaterialSize);
	static std::vector<LineArc2DListPtr> GetUselessRegionsA(const IMaterialSize* pMaterialSize); // ÿ����Ч��·��Ϊһ��Ԫ�ء�

	// �ӡ�����ͼ�λ�·���������ɡ����ϳߴ硱����
	// ע��
	//  1) �������û�����ò��ϳߴ�ĸ���ID��
	static IMaterialSizePtr BuildMatSize(CString strName, const LineArc2DLoopList* pGeomLoopList);

	// ��ʱ����ת90�ȡ�
	static void Rot90Degree(IMaterialSizePtr pMaterialSize);

	// �Ӳ��϶���õ���Χ����Ρ�
	static Polygon2DPtr BuildOuterPolyFromMat(const IMaterialSize* pMaterialSize);
};

END_CUTLEADER_NAMESPACE()
