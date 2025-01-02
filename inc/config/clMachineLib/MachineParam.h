#pragma once

#include "clMachineLibCommon.h"
#include "ClData.h"
#include "clGeometryBaseCommon.h"

DECLARE_CUTLEADER_CLASS(MachineTable)
DECLARE_CUTLEADER_CLASS(OverTravel)
DECLARE_CUTLEADER_CLASS(DriverData)
DECLARE_CUTLEADER_CLASS(MachineParam)

BEGIN_CUTLEADER_NAMESPACE()

class MachineParam : public ClData
{
public:
	MachineParam(void);
	virtual ~MachineParam(void);

public: // implements IData interface.
	virtual BOOL IsModified() const override;
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	MachineTablePtr GetMacTable() const { return m_pMacTable; }
	void SetMacTable(MachineTablePtr pMacTable) { m_pMacTable = pMacTable; }

	void SetOverTravel(OverTravelPtr pOverTravel) { m_pOverTravel = pOverTravel; }
	OverTravelPtr GetOverTravel() const { return m_pOverTravel; }

	DriverDataPtr GetDriverData() const { return m_pDriverData; }
	void SetDriverData(DriverDataPtr pDriverData) { m_pDriverData = pDriverData; }

	CString GetNCPostFix() const { return m_strNCPostfix; }
	void SetNCPostFix(CString strPostfix) { m_strNCPostfix = strPostfix; }

private:
	// machine table
	MachineTablePtr m_pMacTable;

	// OverTravel info
	OverTravelPtr m_pOverTravel;

	// the driver.
	DriverDataPtr m_pDriverData;

	// the post fix of NC code.
	CString m_strNCPostfix;
};

END_CUTLEADER_NAMESPACE()
