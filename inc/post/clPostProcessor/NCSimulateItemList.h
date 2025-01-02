#pragma once

#include "clPostProcessorCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NCSimulateItem)

BEGIN_CUTLEADER_NAMESPACE()

class ClPostProcessor_Export NCSimulateItemList : public std::vector<NCSimulateItemPtr>
{
public:
	NCSimulateItemList(void);
	~NCSimulateItemList(void);

public:
	// get the sim item of "iNCRowID".
	const NCSimulateItem* GetNCSimItem(int iNCRowID) const;
};

END_CUTLEADER_NAMESPACE()
