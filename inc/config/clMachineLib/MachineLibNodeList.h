#pragma once

#include "MachineLibNode.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MachineLibNode)

BEGIN_CUTLEADER_NAMESPACE()

class ClMachineLib_Export MachineLibNodeList : public std::vector<MachineLibNodePtr>
{
public:
	MachineLibNodeList(void);
	~MachineLibNodeList(void);
};

END_CUTLEADER_NAMESPACE()
