#pragma once

#include "ccpBridgeCommon.h"
#include "NcItemListEx.h"
#include "GlViewPortEx.h"

DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(GlViewPort)

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class NcItemDrawer
{
public:
	// �������е�nc�
	static void DrawNcItemList(GlViewPortEx^ glViewPort, NcItemListEx^ ncItemList, Color cutLineColor, Color traceLineColor);

	// ���Ʋ���nc�һ�����ڼӹ�ģ�⡣
	static void DrawNcItemListPartially(GlViewPortEx^ glViewPort, NcItemListEx^ ncItemList, int iEndNcItemIndex, Color drawColor);

private:
	// ����Բ����
	// ע��
	// 1) ����һ��Բ�����10�Σ��������ֻ����10���㣬Բ�����յ��ɺ�����������ʱ�����ϡ�
	// 2) ���ں�С��Բ�����������ٴ�ϵ�������
	static void DrawArc(GlViewPortPtr pGlViewPort, const Arc2D* pArc2D);
};

END_CUTLEADER_NAMESPACE()
