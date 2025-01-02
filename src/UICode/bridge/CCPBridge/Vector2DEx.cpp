#include "windows.h"
#include "Vector2DEx.h"

BEGIN_CUTLEADER_NAMESPACE()

Vector2DEx::Vector2DEx(void)
{
	m_ppVect = new Vector2DPtr();
	m_ppVect->reset(new Vector2D());
}

Vector2DEx::Vector2DEx(double dPtX, double dPtY)
{
	m_ppVect = new Vector2DPtr();
	m_ppVect->reset(new Vector2D(dPtX, dPtY));
}

Vector2DEx::~Vector2DEx()
{
	this->!Vector2DEx();
}

Vector2DEx::!Vector2DEx()
{
	if (m_ppVect)
		delete m_ppVect;
}

bool Vector2DEx::OrthogonalTo(Vector2DEx^ vect)
{
	return (*m_ppVect)->OrthogonalTo(*(vect->GetVector2D()));
}

bool Vector2DEx::ParallelTo(Vector2DEx^ vect)
{
	return (*m_ppVect)->ParallelTo(*(vect->GetVector2D()));
}

double Vector2DEx::AngleTo(Vector2DEx^ vect)
{
	return (*m_ppVect)->AngleTo(*(vect->GetVector2D()));
}

void Vector2DEx::Normalize()
{
    (*m_ppVect)->Normalize();
}

void Vector2DEx::MultiplicationAssignment(double dLength)
{
    (*m_ppVect)->operator *=(dLength);
}

END_CUTLEADER_NAMESPACE()
