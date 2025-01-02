#include "Point2DEx.h"

BEGIN_CUTLEADER_NAMESPACE()

Point2DEx::Point2DEx(void)
{
	m_ppPt = new Point2DPtr();
	m_ppPt->reset(new Point2D());
}

Point2DEx::Point2DEx(double dPtX, double dPtY)
{
	m_ppPt = new Point2DPtr();
	m_ppPt->reset(new Point2D(dPtX, dPtY));
}

Point2DEx::~Point2DEx()
{
	this->!Point2DEx();
}

Point2DEx::!Point2DEx()
{
	if (m_ppPt)
		delete m_ppPt;
}

void Point2DEx::Transfer(Vector2DEx^ vect)
{
    (*m_ppPt)->operator +=(*(vect->GetVector2D()));
}

double Point2DEx::DistanceTo(Point2DEx^ pt)
{
	return (*m_ppPt)->DistanceTo(*(pt->GetPoint2D()));
}

END_CUTLEADER_NAMESPACE()
