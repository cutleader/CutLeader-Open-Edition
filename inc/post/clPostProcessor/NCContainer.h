#pragma once

#include "clPostProcessorCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NCRowList)
DECLARE_CUTLEADER_CLASS(NCCan)

BEGIN_CUTLEADER_NAMESPACE()

// this is a container which keeps the NC code cans.
class NCContainer : public std::vector<NCCanPtr>
{
public:
	NCContainer(void);
	~NCContainer(void);

public:
	// init cans in this container.
	void InitContainer(int iCanNum);

	// save NCSection object to the relative can.
	void SaveNCSection(NCSectionPtr pNCSection);

	// get all NC rows in the container.
	NCRowListPtr GetAllNCRow() const;
};

END_CUTLEADER_NAMESPACE()
