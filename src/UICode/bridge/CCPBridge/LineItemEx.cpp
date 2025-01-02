#include "LineItemEx.h"
#include "Matrix2D.h"

BEGIN_CUTLEADER_NAMESPACE()

LineItemEx::LineItemEx(void)
{
	m_ppLineItem = new Line2DPtr();
	m_ppLineItem->reset(new Line2D());
}

LineItemEx::LineItemEx(Point2DEx^ startPt, Point2DEx^ endPt)
{
	m_ppLineItem = new Line2DPtr();

	Point2D sPt(startPt->X(), startPt->Y());
	Point2D ePt(endPt->X(), endPt->Y());
	m_ppLineItem->reset(new Line2D(sPt, ePt));
}

LineItemEx::LineItemEx(Line2DPtr* ppLineItem)
{
	m_ppLineItem = new Line2DPtr;
	*m_ppLineItem = *ppLineItem;
}

LineItemEx::~LineItemEx()
{
	this->!LineItemEx();
}

LineItemEx::!LineItemEx()
{
	if (m_ppLineItem)
		delete m_ppLineItem;
}

GeomItemEx^ LineItemEx::Clone()
{
	ILineArc2DPtr pGeomItem = (*m_ppLineItem)->Clone();
	Line2DPtr pNewLineItem = boost::dynamic_pointer_cast<Line2D>(pGeomItem);
	GeomItemEx^ geomItemEx = gcnew LineItemEx(&pNewLineItem);
	return geomItemEx;
}

Point2DEx^ LineItemEx::GetStartPoint()
{
	const Point2D& tmpPt = (*m_ppLineItem)->GetStartPoint();
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

void LineItemEx::SetStartPoint(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	(*m_ppLineItem)->SetStartPoint(tmpPt);
}

Point2DEx^ LineItemEx::GetEndPoint()
{
	Point2D tmpPt = (*m_ppLineItem)->GetEndPoint();
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

Point2DEx^ LineItemEx::GetPointByPercent(double dPercent)
{
	Point2D tmpPt = (*m_ppLineItem)->GetPointByPercent(dPercent);
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

double LineItemEx::GetPercentByPoint(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	return (*m_ppLineItem)->GetPercentByPoint(tmpPt);
}

void LineItemEx::Transform(Matrix2DEx^ mat)
{
	(*m_ppLineItem)->Transform(*(mat->GetMatrix()));
}

void LineItemEx::Reverse()
{
	(*m_ppLineItem)->Reverse();
}

double LineItemEx::Distance(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	return (*m_ppLineItem)->Distance(tmpPt);
}

Rect2DEx^ LineItemEx::GetRectBox()
{
	Rect2D rect = (*m_ppLineItem)->GetRectBox();
	return gcnew Rect2DEx(rect);
}

Vector2DEx^ LineItemEx::GetBaseVector()
{
	Vector2D vect = (*m_ppLineItem)->GetBaseVector();
	return gcnew Vector2DEx(vect.X(), vect.Y());
}

void LineItemEx::SetEndPoint(Point2DEx^ pt)
{
	Point2D tmpPt(pt->X(), pt->Y());
	(*m_ppLineItem)->SetEndPoint(tmpPt);
}

END_CUTLEADER_NAMESPACE()
