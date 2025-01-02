#pragma once

#include "ccpBridgeCommon.h"
#include "Point2D.h"
#include "Vector2DEx.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Point2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class Point2DEx
{
public:
	Point2DEx(void);
	Point2DEx(double dPtX, double dPtY);

	~Point2DEx();
	!Point2DEx();

public:
	double X() { return (*m_ppPt)->X(); }
	void X(double dX) { (*m_ppPt)->X(dX); }
	double Y() { return (*m_ppPt)->Y(); }
	void Y(double dY) { (*m_ppPt)->Y(dY); }

    void Transfer(Vector2DEx^ vect);
	double DistanceTo(Point2DEx^ pt);

public:
	Point2DPtr GetPoint2D() { return *m_ppPt; }

private:
	Point2DPtr* m_ppPt;
};

END_CUTLEADER_NAMESPACE()