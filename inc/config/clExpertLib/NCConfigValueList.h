#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NCConfigValue)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)
DECLARE_CUTLEADER_CLASS(NCConfigValueList)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export NCConfigValueList : public std::vector<NCConfigValuePtr>,
										 public ClData
{
public:
	NCConfigValueList(void);
	~NCConfigValueList(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public:
	NCConfigValuePtr GetNCConfigValByID(LONGLONG iID);
	NCConfigValuePtr GetNCConfigValByUniqueID(LONGLONG iUniqueID);
	void ReplaceNCConfigType(NCConfigTypeListPtr pOldNCConfigTypes, NCConfigTypeListPtr pNewNCConfigTypes);
};

END_CUTLEADER_NAMESPACE()
