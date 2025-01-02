#pragma once

#include "ccpBridgeCommon.h"
#include "DataEx.h"
#include "Rect2DEx.h"
#include "Matrix2DEx.h"
#include "LineItemEx.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Polygon2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class Polygon2DEx
{
public:
	Polygon2DEx(void);
	Polygon2DEx(Polygon2DPtr* ppPolygon2D);

	~Polygon2DEx();
	!Polygon2DEx();

public:
    Polygon2DEx^ Clone();
	void AddPoint(Point2DEx^ pt);
	Point2DEx^ GetPtByIndex(int iIndex);
	int GetPtCount();
	Rect2DEx^ GetBoundaryRect();
	void Transfer(Matrix2DEx^ mat);
	bool IncludePolygon(Polygon2DEx^ poly);
    bool IncludePt(Point2DEx^ pt);
	bool IntersectWith(Polygon2DEx^ poly);
	bool IsCCW();
	void AdjustToCCW();

public:
	Polygon2DPtr GetPolygon2D() { return *m_ppPolygon2D; }

private:
	Polygon2DPtr* m_ppPolygon2D;
};

END_CUTLEADER_NAMESPACE()