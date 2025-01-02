#pragma once

#include "ccpBridgeCommon.h"
#include "Matrix2D.h"

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(Matrix2D)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class Matrix2DEx
{
public:
	Matrix2DEx(void);
	Matrix2DEx(const Matrix2D& mat);

	~Matrix2DEx();
	!Matrix2DEx();

public:
	// rotate the matrix.
	void Rotate(double dRotangle) { (*m_ppMat)->Rotate(dRotangle); }

	// transfer the matrix.
	void Transfer(double dX, double dY);

    // scale the matrix.
    void SetScale(double dFactor);

public:
	Matrix2DPtr GetMatrix() { return *m_ppMat; }

private:
	Matrix2DPtr* m_ppMat;
};

END_CUTLEADER_NAMESPACE()