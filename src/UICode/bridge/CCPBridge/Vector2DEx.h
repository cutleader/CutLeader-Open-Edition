#pragma once

#include "ccpBridgeCommon.h"
#include "Vector2D.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Vector2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class Vector2DEx
{
public:
	Vector2DEx(void);
	Vector2DEx(double dPtX, double dPtY);

	~Vector2DEx();
	!Vector2DEx();

public:
	double X() { return (*m_ppVect)->X(); }
	void X(double dX) { (*m_ppVect)->X(dX); }
	double Y() { return (*m_ppVect)->Y(); }
	void Y(double dY) { (*m_ppVect)->Y(dY); }

	double Length(void) { return (*m_ppVect)->Length(); }
	double Angle(void) { return (*m_ppVect)->Angle(); }
	void SetAngle(double dVal) { (*m_ppVect)->SetAngle(dVal); }

	bool OrthogonalTo(Vector2DEx^ vect);
	bool ParallelTo(Vector2DEx^ vect);
	double AngleTo(Vector2DEx^ vect);

    void Normalize();
    void MultiplicationAssignment(double dLength);

public:
	Vector2DPtr GetVector2D() { return *m_ppVect; }

private:
	Vector2DPtr* m_ppVect;
};

END_CUTLEADER_NAMESPACE()