#pragma once

#include "clMachineLibCommon.h"
#include "ClData.h"
#include "ConditionType.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ConditionTypeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClMachineLib_Export ConditionTypeList : public std::vector<ConditionTypePtr>,
								   public ClData
{
public:
	ConditionTypeList();
	~ConditionTypeList();

public: // implements IData interface.
	IDataPtr Clone() const override;

public:
	ConditionTypePtr GetItemByID(LONGLONG iId);

	int GetCondTypeIndex(ConditionTypePtr pCondType);

	void DeleteItemByID(LONGLONG iId);

	// filter the ConditionTypeList using the COND_CATEGORY_TYPE.
	ConditionTypeListPtr GetCondTypeList(COND_CATEGORY_TYPE condCateType);
};

END_CUTLEADER_NAMESPACE()
