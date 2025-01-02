#pragma once

#include "clMachineLibCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(OverTravel)

BEGIN_CUTLEADER_NAMESPACE()

// the over travel of the machine.
class ClMachineLib_Export OverTravel : public ClData
{
public:
	OverTravel();
	OverTravel(double d1, double d2, double d3, double d4);
	~OverTravel(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	double GetMinOTX() { return m_dMinOTX; }
	void SetMinOTX(double dMinOTX) { m_dMinOTX = dMinOTX; }

	double GetMinOTY() { return m_dMinOTY; }
	void SetMinOTY(double dMinOTY) { m_dMinOTY = dMinOTY; }

	double GetMaxOTX() { return m_dMaxOTX; }
	void SetMaxOTX(double dMaxOTX) { m_dMaxOTX = dMaxOTX; }

	double GetMaxOTY() { return m_dMaxOTY; }
	void SetMaxOTY(double dMaxOTY) { m_dMaxOTY = dMaxOTY; }

public: // other functions.
	Rect2D GetRect() { return Rect2D(m_dMinOTX, m_dMaxOTX, m_dMinOTY, m_dMaxOTY); }

private:
	double m_dMinOTX;
	double m_dMinOTY;
	double m_dMaxOTX;
	double m_dMaxOTY;
};

END_CUTLEADER_NAMESPACE()
