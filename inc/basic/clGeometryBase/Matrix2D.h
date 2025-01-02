#pragma once

#include "clGeometryBaseCommon.h"

DECLARE_CUTLEADER_CLASS(Vector2D)
DECLARE_CUTLEADER_CLASS(DoublePair)
DECLARE_CUTLEADER_CLASS(Point2D)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryBase_Export Matrix2D
{
public:
	Matrix2D(void);
	Matrix2D(double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22);
	~Matrix2D(void);

public:
	void Reset();

	void Transfer(const Vector2D& vect);
	void SetTransfer(const Vector2D& vect);

	void Rotate(double dRotangle);
	void SetRotate(double dRotAngle);

	void RotateBy(const Point2D& pt, double dRotAngle);
	void SetRotateBy(const DoublePair& pt, double dRotAngle);

	void SetScale(double dFactor);
	void Scale(double dFactor);
	void SetScaleBy(const Point2D& pt, double dFactor);

	double GetSubDet(short i, short j);
	double GetDeterminant();
	void Inverse();

	void operator*=(const Matrix2D& m2);

	void SetSingleValue(short i, short j, double v ) { m_Mat[i][j] = v; }
	double GetSingleValue(short i, short j) const { return m_Mat[i][j]; }

	bool EqualTo(const Matrix2D& mat) const;

private:
	double m_Mat[3][3];
};

END_CUTLEADER_NAMESPACE()
