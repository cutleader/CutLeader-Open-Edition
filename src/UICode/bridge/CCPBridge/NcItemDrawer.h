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
	// 绘制所有的nc项。
	static void DrawNcItemList(GlViewPortEx^ glViewPort, NcItemListEx^ ncItemList, Color cutLineColor, Color traceLineColor);

	// 绘制部分nc项，一般用于加工模拟。
	static void DrawNcItemListPartially(GlViewPortEx^ glViewPort, NcItemListEx^ ncItemList, int iEndNcItemIndex, Color drawColor);

private:
	// 绘制圆弧。
	// 注：
	// 1) 比如一个圆弧打成10段，这个函数只绘制10个点，圆弧的终点由函数调用者找时机补上。
	// 2) 对于很小的圆弧，这里会减少打断的数量。
	static void DrawArc(GlViewPortPtr pGlViewPort, const Arc2D* pArc2D);
};

END_CUTLEADER_NAMESPACE()
