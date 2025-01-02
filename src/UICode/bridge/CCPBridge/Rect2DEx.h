#pragma once

#include "ccpBridgeCommon.h"
#include "Rect2D.h"
#include "Point2DEx.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Rect2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ComVisible(true)]
public enum class RECT_SIDE_EX
{
    RECT_BOTTOM			= 1,
    RECT_RIGHT			= 2,
    RECT_TOP			= 3,
    RECT_LEFT			= 4,
};

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class Rect2DEx
{
public:
	Rect2DEx(void);
	Rect2DEx(double dXMin, double dXMax, double dYMin, double dYMax);
	Rect2DEx(Rect2DPtr* ppRect2D);
	Rect2DEx(const Rect2D& rect2D);

	~Rect2DEx();
	!Rect2DEx();

public: // get/set functions.
	void SetXMin(double dXMin){ (*m_ppRect2D)->SetXMin(dXMin); }
	double GetXMin() { return (*m_ppRect2D)->GetXMin(); }

	void SetXMax(double dXMax) { (*m_ppRect2D)->SetXMax(dXMax); }
	double GetXMax() { return (*m_ppRect2D)->GetXMax(); }

	void SetYMin(double dYMin) { (*m_ppRect2D)->SetYMin(dYMin); }
	double GetYMin() { return (*m_ppRect2D)->GetYMin(); }

	void SetYMax(double dYMax) { (*m_ppRect2D)->SetYMax(dYMax); }
	double GetYMax() { return (*m_ppRect2D)->GetYMax(); }

	double GetWidth() { return GetXMax() - GetXMin(); }
	double GetHeight() { return GetYMax() - GetYMin(); }

public:
    bool ContainPt(Point2DEx^ pt);
    Point2DEx^ GetCenterPt();
	void Unite(Rect2DEx^ rect);
	void Transform(double dX, double dY);

public:
	Rect2DPtr GetRect2D() { return *m_ppRect2D; }

private:
	Rect2DPtr* m_ppRect2D;
};

END_CUTLEADER_NAMESPACE()