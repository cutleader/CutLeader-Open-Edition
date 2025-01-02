#pragma once

#include "clGeometryBaseCommon.h"
#include "RealPair.h"
#include "Matrix2D.h"
#include "MathEx.h"

BEGIN_CUTLEADER_NAMESPACE()


// this class encapsulates the operations of the vector.
class ClGeometryBase_Export Vector2D : public DoublePair
{
public:
	Vector2D(void)
	{
		m_dX = 1;
		m_dY = 0;
	}

	Vector2D(double dPtX, double dPtY) : DoublePair(dPtX, dPtY)
	{
	}

	Vector2D(const DoublePair& startPt, const DoublePair& endPt)
	{
		m_dX = endPt.X() - startPt.X();
		m_dY = endPt.Y() - startPt.Y();
	}

	~Vector2D(void)
	{
	}

public:
	/************************************************************************/
	// override some operators.

	void operator+=(const Vector2D& v2)
	{
		m_dX += v2.X();
		m_dY += v2.Y();
	}
	void operator*=(double s)
	{
		m_dX *= s;
		m_dY *= s;
	}
	void operator*=(const Matrix2D& mat)
	{
		DoublePair doublePair1;
		double dTmpX = doublePair1.X() * mat.GetSingleValue(0, 0);
		dTmpX += doublePair1.Y() * mat.GetSingleValue(1, 0);
		dTmpX += mat.GetSingleValue(2, 0);
		double dTmpY = doublePair1.X() * mat.GetSingleValue(0, 1);
		dTmpY += doublePair1.Y() * mat.GetSingleValue(1, 1);
		dTmpY += mat.GetSingleValue(2, 1);
		double dTmp = doublePair1.X() * mat.GetSingleValue(0, 2);
		dTmp += doublePair1.Y() * mat.GetSingleValue(1, 2);
		dTmp += mat.GetSingleValue(2, 2);
		doublePair1.Components(dTmpX/dTmp, dTmpY/dTmp);

		DoublePair doublePair2(m_dX, m_dY);
		dTmpX = doublePair2.X() * mat.GetSingleValue(0, 0);
		dTmpX += doublePair2.Y() * mat.GetSingleValue(1, 0);
		dTmpX += mat.GetSingleValue(2, 0);
		dTmpY = doublePair2.X() * mat.GetSingleValue(0, 1);
		dTmpY += doublePair2.Y() * mat.GetSingleValue(1, 1);
		dTmpY += mat.GetSingleValue(2, 1);
		dTmp = doublePair2.X() * mat.GetSingleValue(0, 2);
		dTmp += doublePair2.Y() * mat.GetSingleValue(1, 2);
		dTmp += mat.GetSingleValue(2, 2);
		doublePair2.Components(dTmpX/dTmp, dTmpY/dTmp);

		m_dX = doublePair2.X() - doublePair1.X();
		m_dY = doublePair2.Y() - doublePair1.Y();
	}
	BOOL operator==(const Vector2D& v2)
	{
		return (MathEx::Equal(m_dX, v2.X()) && MathEx::Equal(m_dY, v2.Y()));
	}

	Vector2D operator+(const Vector2D& v2) const
	{
		return Vector2D(m_dX+v2.X(), m_dY+v2.Y());
	}
	Vector2D operator-() const
	{
		return Vector2D(-m_dX, -m_dY);
	}
	Vector2D operator*(double s) const
	{
		return Vector2D(m_dX*s, m_dY*s);
	}
	Vector2D operator/(double s) const
	{
		return Vector2D(m_dX/s, m_dY/s);
	}
	/************************************************************************/


	/************************************************************************/
	// get/set some basic data of the vector.

	// the length of the vector
	double Length(void) const
	{
		return sqrt(m_dX*m_dX + m_dY*m_dY);
	}
	double SqrLength() const
	{
		return m_dX*m_dX + m_dY*m_dY;
	}

	// get the angle of the vector.
	// notes: 
	//   1) range is [-PI, PI].
	double Angle(void) const
	{
		return atan2(m_dY, m_dX);
	}

	// set the angle of the vector
	void SetAngle(double dVal)
	{
		Vector2D v;
		v.Magnitude(Length());

		v.Rotate(dVal);

		*this = v;
	}

	void SetVector(const DoublePair& startPt, const DoublePair& endPt)
	{
		m_dX = endPt.X() - startPt.X();
		m_dY = endPt.Y() - startPt.Y();
	}
	/************************************************************************/


	/************************************************************************/
	// relations with other vector.

	// whether this vector is orthogonal to vect.
	BOOL OrthogonalTo(const Vector2D& vect) const
	{
		return MathEx::Equal(DotProduct(vect), 0.0);
	}
	BOOL OrthogonalToA(const Vector2D& vect) const
	{
		return MathEx::Equal(DotProduct(vect), 0.0, GEOM_TOLERANCE);
	}

	// whether this vector is parallel to vect.
	// ע��
	//   1) �ڶ����������á�GEOM_TOLERANCE�����Ƚϡ�
    //   2) ������Ҫת�ɵ�λ�������жϣ������ת�Ļ������������ֵ��ͷ�����
    //      a) �����һ�������ǳ��̣�һ���Ƚϳ�������������ֵ������0�������ͻᵼ�³��Ƚ���0���������κ�������ƽ�С�
    //      b) �����Ƚϳ���������(0.0,-430.9999942779)��(4.8874999e-006,430.999997)������Ӧ��ƽ�еģ��������ղ���㷨���£�
    //         0.00000000000000000 * 430.99999713889997 - (-430.99999427790004) * 4.8874999265535735e-006 = 0.002����Ϊ���ȳ���ԭ�򣬷Ŵ����������жϳ��˲�ƽ�еĽ����
    //   3) �����ƣ�����㷨���ܻ������ƣ��������������ٽ����
	BOOL ParallelTo(const Vector2D& vect) const
	{
        Vector2D vect1 = *this;
        vect1.Normalize();
        Vector2D vect2 = vect;
        vect2.Normalize();
		return MathEx::Equal(vect1.CrossProduct(vect2), 0.0);
	}
	BOOL ParallelToA(const Vector2D& vect) const
	{
        Vector2D vect1 = *this;
        vect1.Normalize();
        Vector2D vect2 = vect;
        vect2.Normalize();
		return MathEx::Equal(vect1.CrossProduct(vect2), 0.0, GEOM_TOLERANCE);
	}

	// the angle to specified vector.
	// notes: 
	//   1) range is [-PI, PI].
	double AngleTo(const Vector2D& vect) const {
		double dAngle = vect.Angle() - atan2(m_dY, m_dX);
		return MathEx::GetSignedAng(dAngle);
	}
	/************************************************************************/


	/************************************************************************/
	// transform the vector.

	// rotate the vector with the angle
	void Rotate(double rotAngle)
	{
		Matrix2D tmpMatrix;
		tmpMatrix.Rotate(rotAngle);

		*this *= tmpMatrix;
	}

	// mirror this vector based on the specified vector.
	void Mirror(const Vector2D& Vect)
	{
		double len = Vect.Length();
		double a,b,p,q;
		a = Vect.X();
		b = Vect.Y();	
		p = m_dX;
		q = m_dY;
		m_dX = (a*a*p-b*b*p+2*a*b*q)/(a*a+b*b);
		m_dY = (b*b*q-a*a*q+2*a*b*p)/(a*a+b*b);
	}

	// get the orthogonal vector that is, rotate the vector with 90 degree by CCW.
	Vector2D Orthogonal() const
	{
		return(Vector2D(-m_dY, m_dX));
	}

	// change the length of the vector to newMag
	double Magnitude(double newMag)
	{
		double mag = sqrt(m_dX*m_dX + m_dY*m_dY);
		m_dX *= newMag/mag; 
		m_dY *= newMag/mag;

		return newMag;
	}

	// set the vector to unit vector.
	void Normalize(void)
	{
		double length = Length();
		m_dX /= length;
		m_dY /= length;
	}
	/************************************************************************/


	/************************************************************************/
	// operator with other vector.

	double DotProduct(const Vector2D& vect) const
	{
		return (m_dX*vect.X() + m_dY*vect.Y());
	}
	double CrossProduct(const Vector2D& vect) const
	{
		return (m_dX*vect.Y() - m_dY*vect.X());
	}

	// get the angle bisector by this vector and "vect".
	/*
         
						 *           
						 *         **  
						 *        *    
					v1	 *      **     
						 *     *       
						 *   **    v3    
						 * **          
						 **            
						***************
					   *      v2         
					 **                
					*                  
				  **                   
				**    v4                 
			   *                                             

	*/
	// notes:
	//   1) in the illustration, we need v3, not need v4. that is, we will get the 
	//      angle bisector at the side which the included angle is less than PI.
	Vector2D AngleBisector(const Vector2D& vect) const
	{
		Vector2D retVect = *this;

		double dTmpAngle = AngleTo(vect);
		retVect.Rotate(dTmpAngle/2);

		return retVect;
	}
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
