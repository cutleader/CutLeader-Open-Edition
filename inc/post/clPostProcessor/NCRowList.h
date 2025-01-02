#pragma once

#include "clPostProcessorCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NCRow)

BEGIN_CUTLEADER_NAMESPACE()

class ClPostProcessor_Export NCRowList : public std::vector<NCRowPtr>
{
public:
	NCRowList(void);
	~NCRowList(void);

public:
	NCRowPtr GetNCRowByID(int iNCRowID) const;
};

END_CUTLEADER_NAMESPACE()
