#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include "ConditionType.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(CondValInfo)
DECLARE_CUTLEADER_CLASS(CondValInfoList)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export CondValInfoList : public std::vector<CondValInfoPtr>,
										 public ClData
{
public:
	CondValInfoList();
	~CondValInfoList();

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // other functions.
	/************************************************************************/
	// about getting CondValInfo.

	// get the CondValInfo of the specified condition type
	// notes: if cannot find this condition type in value list, return NULL.
	CondValInfoPtr GetItemByCondType(LONGLONG iCondTypeId);

	CondValInfoPtr GetItemByID(LONGLONG iId);

	// get CondValInfo list which fit "cateType".
	CondValInfoListPtr GetByCategoryType(COND_CATEGORY_TYPE cateType);
	/************************************************************************/

	void DeleteCondVal(CondValInfoPtr pCondValInfo);

	// replace ConditionType with the new ConditionType.
	// notes:
	//   1) we will call this when clone CondValInfo list, "pNewCondTypeList"
	//      is a clone one of "pOldCondTypeList".
	void ReplaceCondType(ConditionTypeListPtr pOldCondTypeList, ConditionTypeListPtr pNewCondTypeList);
};

END_CUTLEADER_NAMESPACE()
