#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ITplEntity)
DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(EntityGroup)
DECLARE_CUTLEADER_CLASS(EntityGroupList)

BEGIN_CUTLEADER_NAMESPACE()

// the entity group list.
class ClReportTemplate_Export EntityGroupList : public std::vector<EntityGroupPtr>,
										   public ClData
{
public:
	EntityGroupList(void);
	~EntityGroupList(void);

public:
	/************************************************************************/
	// about get the group.

	// get the group by the template entity.
	EntityGroupPtr GetGroupByEntity(ITplEntityPtr pTplEntity);

	// get the group list by the template entity list.
	// notes:
	//   1) no duplicated groups returned.
	EntityGroupListPtr GetGroupByEntity(TplEntityListPtr pTplEntityList);
	/************************************************************************/

	/************************************************************************/
	// about delete the group.

	// delete the group.
	void DeleteGroup(EntityGroupPtr pEntityGroup);

	// delete the group list.
	void DeleteGroup(EntityGroupListPtr pEntityGroupList);
	/************************************************************************/

	// whether "pEntityGroup" is exist.
	BOOL IsGroupExist(EntityGroupPtr pEntityGroup);

	// append the group list.
	// notes:
	//   1) the duplicated groups will not be added.
	void AddGroup(EntityGroupListPtr pEntityGroupList);
};

END_CUTLEADER_NAMESPACE()
