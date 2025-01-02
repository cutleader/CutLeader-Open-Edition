#pragma once

#include "clMachineLibCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(MachineTable)

BEGIN_CUTLEADER_NAMESPACE()

// the table info of the machine
class ClMachineLib_Export MachineTable : public ClData
{
public:
	MachineTable();
	MachineTable(double dTableSizeX, double dTableSizeY);
	~MachineTable();

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetTableSizeX(double dTableSizeX) { m_dTableSizeX = dTableSizeX; }
	double GetTableSizeX() { return m_dTableSizeX; }

	void SetTableSizeY(double dTableSizeY) { m_dTableSizeY = dTableSizeY; }
	double GetTableSizeY() { return m_dTableSizeY; }

public: // other functions.
	Rect2D GetRect() { return Rect2D(0, m_dTableSizeX, 0, m_dTableSizeY); }

private:
	// the table size.
	double m_dTableSizeX;
	double m_dTableSizeY;
};

END_CUTLEADER_NAMESPACE()
