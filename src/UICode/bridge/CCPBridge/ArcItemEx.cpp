#include "ArcItemEx.h"

#include "Matrix2D.h"

BEGIN_CUTLEADER_NAMESPACE()

ArcItemEx::ArcItemEx(void)
{
	m_ppArcItem = new Arc2DPtr();
	m_ppArcItem->reset(new Arc2D());
}

ArcItemEx::ArcItemEx(Point2DEx^ centerPt, double dRadius, double dStartAngle, double dMoveAngle)
{
	m_ppArcItem = new Arc2DPtr();
	m_ppArcItem->reset(new Arc2D(Point2D(centerPt->X(), centerPt->Y()), dRadius, dStartAngle, dMoveAngle));
}

ArcItemEx::ArcItemEx(Arc2DPtr* ppArcItem)
{
	m_ppArcItem = new Arc2DPtr;
	*m_ppArcItem = *ppArcItem;
}

ArcItemEx::~ArcItemEx()
{
	this->!ArcItemEx();
}

ArcItemEx::!ArcItemEx()
{
	if (m_ppArcItem)
		delete m_ppArcItem;
}

GeomItemEx^ ArcItemEx::Clone()
{
	ILineArc2DPtr pGeomItem = (*m_ppArcItem)->Clone();
	Arc2DPtr pNewArcItem = boost::dynamic_pointer_cast<Arc2D>(pGeomItem);
	GeomItemEx^ geomItemEx = gcnew ArcItemEx(&pNewArcItem);
	return geomItemEx;
}

Point2DEx^ ArcItemEx::GetStartPoint()
{
	const Point2D& tmpPt = (*m_ppArcItem)->GetStartPoint();
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

void ArcItemEx::SetStartPoint(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	(*m_ppArcItem)->SetStartPoint(tmpPt);
}

Point2DEx^ ArcItemEx::GetEndPoint()
{
	Point2D tmpPt = (*m_ppArcItem)->GetEndPoint();
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

Point2DEx^ ArcItemEx::GetPointByPercent(double dPercent)
{
	Point2D tmpPt = (*m_ppArcItem)->GetPointByPercent(dPercent);
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

double ArcItemEx::GetPercentByPoint(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	return (*m_ppArcItem)->GetPercentByPoint(tmpPt);
}

void ArcItemEx::Transform(Matrix2DEx^ mat)
{
	(*m_ppArcItem)->Transform(*(mat->GetMatrix()));
}

void ArcItemEx::Reverse()
{
	(*m_ppArcItem)->Reverse();
}

double ArcItemEx::Distance(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	return (*m_ppArcItem)->Distance(tmpPt);
}

Rect2DEx^ ArcItemEx::GetRectBox()
{
	Rect2D rect = (*m_ppArcItem)->GetRectBox();
	return gcnew Rect2DEx(rect);
}

Point2DEx^ ArcItemEx::GetCenterPt()
{
	const Point2D& tmpPt = (*m_ppArcItem)->GetBasePoint();
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

void ArcItemEx::SetCenterPt(Point2DEx^ centerPt)
{
	(*m_ppArcItem)->SetBasePoint(Point2D(centerPt->X(), centerPt->Y()));
}

END_CUTLEADER_NAMESPACE()
