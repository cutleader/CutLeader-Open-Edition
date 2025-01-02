#pragma once

#include "ccpBridgeCommon.h"
#include "DataEx.h"
#include "Matrix2DEx.h"
#include "Rect2DEx.h"
#include "GeomItemEx.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(LineArc2DList)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class GeomItemListEx
{
public:
	GeomItemListEx(void);
	GeomItemListEx(LineArc2DListPtr* ppGeomItemList);

	~GeomItemListEx();
	!GeomItemListEx();

public:
    GeomItemListEx^ Clone();

	void Transform(Matrix2DEx^ mat);
	Rect2DEx^ GetRectBox();

	int Count();
	GeomItemEx^ GetItemByIndex(int iIndex);
	void AddItem(GeomItemEx^ geomItem);
	void AddItemList(GeomItemListEx^ geomItemList);
	void RemoveItem(int iIndex);
	void Clear();

public:
	LineArc2DListPtr GetGeomItemList() { return *m_ppGeomItemList; }

private:
	LineArc2DListPtr* m_ppGeomItemList;
};

END_CUTLEADER_NAMESPACE()