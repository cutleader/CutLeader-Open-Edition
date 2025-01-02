#pragma once

#include "clReportTemplateCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(ITplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// the entity group.
// notes:
//   1) for report template, entities in one group can be selected/moved/deleted together.
//   2) for report page, if a table spread, entities in one group can be moved together.
class ClReportTemplate_Export EntityGroup : public ClData
{
public:
	EntityGroup(void);
	~EntityGroup(void);

public: // get/set functions.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() { return m_strName; }

	void SetTplEntityList(TplEntityListPtr pTplEntityList) { m_pTplEntityList = pTplEntityList; }
	TplEntityListPtr GetTplEntityList() { return m_pTplEntityList; }

public:
	// whether this group include "pTplEntity".
	BOOL IncludeTplEntity(ITplEntityPtr pTplEntity);

	// get the outer rect of the group.
	CRect GetGroupRect();

private:
	CString m_strName;

	// template entities in the group.
	TplEntityListPtr m_pTplEntityList;
};

END_CUTLEADER_NAMESPACE()
