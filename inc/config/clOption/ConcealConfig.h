#pragma once

#include "clOptionCommon.h"

DECLARE_CUTLEADER_CLASS(SimulateInfo)
DECLARE_CUTLEADER_CLASS(DebugUIInfo)

BEGIN_CUTLEADER_NAMESPACE()

// in this class, we defined some conceal params.
// notes:
//   1) user will change these params rarely, so hide them.
class ClOption_Export ConcealConfig
{
public:
	ConcealConfig(void);
	~ConcealConfig(void);

public: // get/set functions.
	SimulateInfoPtr GetSimulateInfo() const { return m_pSimulateInfo; }
	DebugUIInfoPtr GetDebugUIInfo() const { return m_pDebugUIInfo; }

private:
	SimulateInfoPtr m_pSimulateInfo;
	DebugUIInfoPtr m_pDebugUIInfo;
};

END_CUTLEADER_NAMESPACE()
