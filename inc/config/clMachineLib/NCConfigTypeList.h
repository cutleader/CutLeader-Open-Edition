#pragma once

#include "clMachineLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(NCConfigType)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClMachineLib_Export NCConfigTypeList : public std::vector<NCConfigTypePtr>,
									   public ClData
{
public:
	NCConfigTypeList(void);
	~NCConfigTypeList(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // other functions.
	NCConfigTypePtr GetItemByID(LONGLONG iId);

	int GetIndex(NCConfigTypePtr pNCConfigType);

	void DeleteItemByID(LONGLONG iId);
};

END_CUTLEADER_NAMESPACE()
