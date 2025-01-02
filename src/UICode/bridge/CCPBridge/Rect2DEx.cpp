#include "Rect2DEx.h"

BEGIN_CUTLEADER_NAMESPACE()

Rect2DEx::Rect2DEx(void)
{
	m_ppRect2D = new Rect2DPtr();
	m_ppRect2D->reset(new Rect2D());
}

Rect2DEx::Rect2DEx(double dXMin, double dXMax, double dYMin, double dYMax)
{
	m_ppRect2D = new Rect2DPtr();
	m_ppRect2D->reset(new Rect2D(dXMin, dXMax, dYMin, dYMax));
}

Rect2DEx::Rect2DEx(Rect2DPtr* ppRect2D)
{
	m_ppRect2D = new Rect2DPtr;
	*m_ppRect2D = *ppRect2D;
}

Rect2DEx::Rect2DEx(const Rect2D& rect2D)
{
	m_ppRect2D = new Rect2DPtr();
	m_ppRect2D->reset(new Rect2D(rect2D));
}

Rect2DEx::~Rect2DEx()
{
	this->!Rect2DEx();
}

Rect2DEx::!Rect2DEx()
{
	if (m_ppRect2D)
		delete m_ppRect2D;
}

bool Rect2DEx::ContainPt(Point2DEx^ pt)
{
    return (*m_ppRect2D)->ContainPt(Point2D(pt->X(), pt->Y()));
}

void Rect2DEx::Unite(Rect2DEx^ rect)
{
	(*m_ppRect2D)->Unite(*rect->GetRect2D());
}

Point2DEx^ Rect2DEx::GetCenterPt()
{
    Point2D pt = (*m_ppRect2D)->GetCenterPt();
    return gcnew Point2DEx(pt.X(), pt.Y());
}

void Rect2DEx::Transform(double dX, double dY)
{
	(*m_ppRect2D)->Transform(dX, dY);
}

END_CUTLEADER_NAMESPACE()
