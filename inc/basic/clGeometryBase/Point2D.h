#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "baseConst.h"
#include "MathEx.h"

BEGIN_CUTLEADER_NAMESPACE()

// point class.
class ClGeometryBase_Export Point2D : public DoublePair
{
public:
	Point2D(void)
	{
		m_dX = 0;
		m_dY = 0;
	}
	Point2D(const Point2D& pt)
	{
		m_dX = pt.X();
		m_dY = pt.Y();
	}
	Point2D(double dPtX, double dPtY) : DoublePair(dPtX, dPtY) {}
	~Point2D(void) {}

public:
	/************************************************************************/
	// over ride some operators.
	
	Point2D operator+(const Vector2D& vect) const
	{ 
		return Point2D(m_dX+vect.X(), m_dY+vect.Y()); 
	}
	void operator+=(const Vector2D& vec)
	{
		m_dX += vec.X();
		m_dY += vec.Y();
	}

	Point2D operator+(const Point2D& p2) const
	{
		return Point2D(m_dX + p2.X(), m_dY + p2.Y());
	}
	void operator+=(const Point2D& pt)
	{ 
		m_dX += pt.X();
		m_dY += pt.Y();
	}

	Point2D operator-(const Point2D& pt) const
	{
		return Point2D(m_dX - pt.X(), m_dY - pt.Y());
	}

	void operator*=(const Matrix2D& mat)
	{
		double dTemp_x = 0;
		double dTemp_y = 0; 
		DOUBLE dTemp   = 1;

		dTemp_x = m_dX * mat.GetSingleValue(0, 0);
		dTemp_x += m_dY * mat.GetSingleValue(1, 0);
		dTemp_x += mat.GetSingleValue(2, 0);

		dTemp_y = m_dX * mat.GetSingleValue(0, 1);
		dTemp_y += m_dY * mat.GetSingleValue(1, 1);
		dTemp_y += mat.GetSingleValue(2, 1);

		dTemp = m_dX * mat.GetSingleValue(0, 2);
		dTemp += m_dY * mat.GetSingleValue(1, 2);
		dTemp += mat.GetSingleValue(2, 2);

		m_dX = dTemp_x/dTemp;
		m_dY = dTemp_y/dTemp;
	}
	void operator*=(double s)
	{ 
		m_dX *= s; 
		m_dY *= s; 
	}

	void operator/=(double s)
	{
		if( s != 0 )
		{
			m_dX /= s;
			m_dY /= s;
		}
	}
	Point2D operator/(double s) const
	{
		if (s != 0)
			return Point2D(m_dX/s, m_dY/s);
		else 
			return Point2D(m_dX, m_dY);
	}

	Point2D operator*(double s) const
	{
		return Point2D(m_dX*s, m_dY*s);
	}

	bool operator <(const Point2D& Pt) const
	{
		if (X() < Pt.X() - GEOM_TOLERANCE)
			return true;
		else if(X() > Pt.X() + GEOM_TOLERANCE)
			return false;
		else
			return Y() < Pt.Y() - GEOM_TOLERANCE;
	}
	bool operator ==(const Point2D& Pt) const
	{
		return (fabs(X() - Pt.X()) < GEOM_TOLERANCE) && (fabs(Y() - Pt.Y()) < GEOM_TOLERANCE);
	}
	bool operator !=(const Point2D& Pt) const
	{ 
		return !((*this) == (Pt)); 
	}
	/************************************************************************/


	/************************************************************************/
	// the relationship with other point.

	// check whether one point is left-lower/higher than the other.
	// notes:
	//   1) the left judge is prior to low/high judge.
	//   2) to be verified: GEOM_TOLERANCE was changed to SYSTEM_TOLERANCE.
	bool LeftLower(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dX, pt.X()))
			return m_dY < pt.Y() - SYSTEM_TOLERANCE;
		else if (m_dX < pt.X() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dX > pt.X() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dY < pt.Y() - SYSTEM_TOLERANCE;
	}
	bool LeftHigher(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dX, pt.X()))
			return m_dY > pt.Y() - SYSTEM_TOLERANCE;
		else if (m_dX < pt.X() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dX > pt.X() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dY > pt.Y() - SYSTEM_TOLERANCE;
	}

	// check whether one point is right-lower/higher than the other.
	// notes:
	//   1) the right judge is prior to low/high judge.
	//   2) to be verified: GEOM_TOLERANCE was changed to SYSTEM_TOLERANCE.
	bool RightLower(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dX, pt.X()))
			return m_dY < pt.Y() - SYSTEM_TOLERANCE;
		else if (m_dX > pt.X() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dX < pt.X() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dY < pt.Y() - SYSTEM_TOLERANCE;
	}
	bool RightHigher(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dX, pt.X()))
			return m_dY > pt.Y() - SYSTEM_TOLERANCE;
		else if (m_dX > pt.X() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dX < pt.X() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dY > pt.Y() - SYSTEM_TOLERANCE;
	}

	// check whether one point is low-lefter/righter than the other.
	// notes:
	//   1) the low judge is prior to left/right judge.
	//   2) to be verified: GEOM_TOLERANCE was changed to SYSTEM_TOLERANCE.
	bool LowLefter(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dY, pt.Y()))
			return m_dX < pt.X() - SYSTEM_TOLERANCE;
		else if (m_dY < pt.Y() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dY > pt.Y() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dX < pt.X() - SYSTEM_TOLERANCE;
	}
	bool LowRighter(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dY, pt.Y()))
			return m_dX > pt.X() - SYSTEM_TOLERANCE;
		else if (m_dY < pt.Y() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dY > pt.Y() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dX > pt.X() - SYSTEM_TOLERANCE;
	}

	// check whether one point is high-lefter/righter than the other.
	// notes:
	//   1) the high judge is prior to left/right judge.
	//   2) to be verified: GEOM_TOLERANCE was changed to SYSTEM_TOLERANCE.
	bool HighLefter(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dY, pt.Y()))
			return m_dX < pt.X() - SYSTEM_TOLERANCE;
		else if (m_dY > pt.Y() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dY < pt.Y() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dX < pt.X() - SYSTEM_TOLERANCE;
	}
	bool HighRighter(const Point2D& pt) const
	{
		if (MathEx::Equal(m_dY, pt.Y()))
			return m_dX > pt.X() - SYSTEM_TOLERANCE;
		else if (m_dY > pt.Y() - SYSTEM_TOLERANCE)
			return true;
		else if (m_dY < pt.Y() + SYSTEM_TOLERANCE)
			return false;
		else
			return m_dX > pt.X() - SYSTEM_TOLERANCE;
	}

	// check whether one point is on left of the other.
	// notes:
	//   1) to be verified: GEOM_TOLERANCE was changed to SYSTEM_TOLERANCE.
	BOOL Lefter(const Point2D& pt) const
	{
		if (m_dX < pt.X() - SYSTEM_TOLERANCE)
			return TRUE;
		else if (m_dX > pt.X() + SYSTEM_TOLERANCE)
			return FALSE;
		else
			return m_dY < pt.Y() - SYSTEM_TOLERANCE;
	}

	// check whether current point is righter that pt.
	bool Righter(const Point2D& pt) const
	{
		return m_dX > pt.X();
	}

	// check whether current point is higher that pt.
	bool Higher(const Point2D& pt) const
	{
		return m_dY > pt.Y();
	}

	// check whether current point is lower that pt.
	bool Lower(const Point2D& pt) const
	{
		return m_dY < pt.Y();
	}

	// return 1 if this point is on the left of the vector 
	// with its start point is startPt and direction is (dX, dY),
	// return -1 if on the right side, else return 0.
	int CheckSide(const Point2D& startPt, double dX, double dY) const
	{
		double dS = dX * (Y() - startPt.Y()) - dY * (X() - startPt.X());

		double len = dX*dX + dY*dY;
		if (len > 1)
		{
			dS /= len;
		}

		if (dS > SYSTEM_TOLERANCE)
		{
			return 1;
		}
		else if(dS < -SYSTEM_TOLERANCE)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	// whether these two points are equal.
	BOOL IsEqual(const Point2D& pt, double dTol = GEOM_TOLERANCE) const
	{
		return fabs(X()-pt.X())+fabs(Y()-pt.Y()) < dTol;
	}
	BOOL IsEqualA(Point2D pt)
	{
		return fabs(X()-pt.X())+fabs(Y()-pt.Y()) < GEOM_TOLERANCE;
	}

	// the distance to other point.
	double DistanceTo(const Point2D& pt) const
	{
		return sqrt((pt.X()-m_dX)*(pt.X()-m_dX)+(pt.Y()-m_dY)*(pt.Y()-m_dY));
	}
	double XDistanceTo(const Point2D& pt)
	{
		return fabs(m_dX-pt.X());
	}
	double YDistanceTo(const Point2D& pt)
	{
		return fabs(m_dY-pt.Y());
	}
	double SquareDistanceTo(const Point2D& pt) const
	{
		return (pt.X()-m_dX)*(pt.X()-m_dX)+(pt.Y()-m_dY)*(pt.Y()-m_dY);
	}
	double SquareDistanceTo_2(double dX, double dY) const
	{
		return (dX - m_dX)*(dX - m_dX) + (dY - m_dY)*(dY - m_dY);
	}
	/************************************************************************/


	/************************************************************************/
	// transform operations for the point.

	void Rotate(double dAngle)
	{
		Matrix2D mat;
		mat.SetRotate(dAngle);

		*this *= mat;
	}
	void RotateBasedPt(const Point2D& pt, double dAngle)
	{
		Matrix2D mat;
		mat.SetRotateBy(pt, dAngle);

		*this *= mat;
	}
	void RotateBasedPt(const Point2D& pt, double dCos, double dSin)
	{
		double dDX = X() - pt.X();
		double dDY = Y() - pt.Y();

		m_dX = pt.X() + dDX * dCos - dDY * dSin;
		m_dY = pt.Y() + dDX * dSin + dDY * dCos;
	}

	void Transfer(const Vector2D& vect)
	{
		Matrix2D mat;
		mat.SetTransfer(vect);

		*this *= mat;
	}

	void Scale(const Point2D& pt, double dFactor)
	{
		Matrix2D mat;
		mat.SetScaleBy(pt, dFactor);
		*this *= mat;
	}

	// 对点做逆向变换。
	Point2D InverseTransform(Matrix2D mat) const
	{
		Point2D newPt = *this;
		mat.Inverse();
		newPt *= mat;

		return newPt;
	}
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
