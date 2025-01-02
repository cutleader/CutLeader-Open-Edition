#pragma once

#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(EntityGroupList)
DECLARE_CUTLEADER_CLASS(EntityGroup)

BEGIN_CUTLEADER_NAMESPACE()

// this command will group the entities.
class ClReportTemplate_Export GroupEntityCommand : public RptCmdBase
{
public:
	GroupEntityCommand(CWnd* pView, EntityGroupListPtr pEntityGroupList, EntityGroupPtr pNewEntityGroup);
	~GroupEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	EntityGroupListPtr m_pEntityGroupList;
	EntityGroupPtr m_pNewEntityGroup;
};

END_CUTLEADER_NAMESPACE()
