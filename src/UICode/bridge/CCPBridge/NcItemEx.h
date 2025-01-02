#pragma once

#include "ccpBridgeCommon.h"
#include "NcItem.h"
#include "GeomItemEx.h"
#include "Point2DEx.h"

BEGIN_CUTLEADER_NAMESPACE()

[ComVisible(true)]
public enum class NcItemTypeEx
{
	NcItem_CutLine_Ex				= 0,
	NcItem_TraceLine_Ex				= 1,
	NcItem_CutSegment_OrderNum_Ex	= 2,
};

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class NcItemEx
{
public:
	NcItemEx(void);
	NcItemEx(NcItemPtr* ppNcItem);
	~NcItemEx();
	!NcItemEx();

public:
	NcItemTypeEx GetNcItemType();
	void SetNcItemType(NcItemTypeEx emNcItemTypeEx);

	GeomItemEx^ GetLineArc2D();
	void SetLineArc2D(GeomItemEx^ geomItem);

	Point2DEx^ GetCurrentCutPt();
	void SetCurrentCutPt(Point2DEx^ currentCutPt);

	int GetOrderNum();
	void SetOrderNum(int iOrderNum);

public:
	NcItemPtr GetNcItem() { return *m_ppNcItem; }

private:
	NcItemPtr* m_ppNcItem;
};

END_CUTLEADER_NAMESPACE()