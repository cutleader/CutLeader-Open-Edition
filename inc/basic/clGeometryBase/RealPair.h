#pragma once

#include "clGeometryBaseCommon.h"
#include "baseConst.h"
#include "math.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class keeps two double value.
class ClGeometryBase_Export DoublePair
{
public:
	DoublePair(void)
	{
		m_dX = 0;
		m_dY = 0;
	}

	DoublePair(double dX, double dY)
	{
		m_dX = dX;
		m_dY = dY;
	}

	~DoublePair(void)
	{

	}

public:
	double X() const { return m_dX; }
	void X(double dX) { m_dX = dX; }

	double Y() const { return m_dY; }
	void Y(double dY) { m_dY = dY; }

	void Components(double dX, double dY)
	{
		m_dX = dX;
		m_dY = dY;
	}

	// compare.
	BOOL SmallerThan(DoublePair& dbPair)
	{
		return m_dX < dbPair.X() && m_dY < dbPair.Y();
	}
	bool Equal(DoublePair& dbPair)
	{
		return fabs(m_dX-dbPair.X()) < GEOM_TOLERANCE && fabs(m_dY-dbPair.Y()) < GEOM_TOLERANCE;
	}

protected:
	double m_dX;
	double m_dY;
};

END_CUTLEADER_NAMESPACE()
