#include "Polygon2DEx.h"

#include "Polygon2D.h"

BEGIN_CUTLEADER_NAMESPACE()

Polygon2DEx::Polygon2DEx(void)
{
	m_ppPolygon2D = new Polygon2DPtr();
	m_ppPolygon2D->reset(new Polygon2D());
}

Polygon2DEx::Polygon2DEx(Polygon2DPtr* ppPolygon2D)
{
	m_ppPolygon2D = new Polygon2DPtr;
	*m_ppPolygon2D = *ppPolygon2D;
}

Polygon2DEx::~Polygon2DEx()
{
	this->!Polygon2DEx();
}

Polygon2DEx::!Polygon2DEx()
{
	if (m_ppPolygon2D)
		delete m_ppPolygon2D;
}

Polygon2DEx^ Polygon2DEx::Clone()
{
	Polygon2DPtr pNewPoly = (*m_ppPolygon2D)->Clone();
	Polygon2DEx^ newPolygonEx = gcnew Polygon2DEx(&pNewPoly);
	return newPolygonEx;
}

void Polygon2DEx::AddPoint(Point2DEx^ pt)
{
	(*m_ppPolygon2D)->push_back(Point2D(pt->X(), pt->Y()));
}

Point2DEx^ Polygon2DEx::GetPtByIndex(int iIndex)
{
	Point2D pt = (*m_ppPolygon2D)->operator[](iIndex);
	return gcnew Point2DEx(pt.X(), pt.Y());
}

int Polygon2DEx::GetPtCount()
{ 
	return (*m_ppPolygon2D)->size(); 
}

Rect2DEx^ Polygon2DEx::GetBoundaryRect()
{
	Rect2D rect = (*m_ppPolygon2D)->GetBoundaryRect();
	return gcnew Rect2DEx(rect);
}

void Polygon2DEx::Transfer(Matrix2DEx^ mat)
{
	Matrix2DPtr pMatrix = mat->GetMatrix();
	(*m_ppPolygon2D)->Transfer(*pMatrix);
}

bool Polygon2DEx::IncludePolygon(Polygon2DEx^ poly)
{
	Polygon2DPtr pPoly = poly->GetPolygon2D();
	return (*m_ppPolygon2D)->IncludePolygon(*pPoly);
}

bool Polygon2DEx::IncludePt(Point2DEx^ pt)
{
    if ((*m_ppPolygon2D)->GetPointPos(Point2D(pt->X(), pt->Y())) == OUTSIDE_POLY)
    {
        return false;
    }

    return true;
}

bool Polygon2DEx::IntersectWith(Polygon2DEx^ poly)
{
	Polygon2DPtr pPoly = poly->GetPolygon2D();
	return (*m_ppPolygon2D)->IntersectWith(*pPoly);
}

bool Polygon2DEx::IsCCW()
{
	return (*m_ppPolygon2D)->IsCCW();
}

void Polygon2DEx::AdjustToCCW()
{
	(*m_ppPolygon2D)->AdjustToCCW();
}

END_CUTLEADER_NAMESPACE()
