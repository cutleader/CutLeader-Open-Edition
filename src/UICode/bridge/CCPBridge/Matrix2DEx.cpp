#include "Matrix2DEx.h"
#include "Vector2D.h"


BEGIN_CUTLEADER_NAMESPACE()

Matrix2DEx::Matrix2DEx(void)
{
	m_ppMat = new Matrix2DPtr();
	m_ppMat->reset(new Matrix2D());
}

Matrix2DEx::Matrix2DEx(const Matrix2D& mat)
{
	m_ppMat = new Matrix2DPtr();
	m_ppMat->reset(new Matrix2D(mat));
}

Matrix2DEx::~Matrix2DEx()
{
	this->!Matrix2DEx();
}

Matrix2DEx::!Matrix2DEx()
{
	if (m_ppMat)
		delete m_ppMat;
}

void Matrix2DEx::Transfer(double dX, double dY)
{
	Vector2D dblPair(dX, dY);
	(*m_ppMat)->Transfer(dblPair);
}

void Matrix2DEx::SetScale(double dFactor)
{
    (*m_ppMat)->SetScale(dFactor);
}

END_CUTLEADER_NAMESPACE()
