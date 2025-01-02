#pragma once

#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(EntityGroupList)

BEGIN_CUTLEADER_NAMESPACE()

// this command will delete the entities.
// notes:
//   1) this command will also care the associated groups.
class ClReportTemplate_Export DelEntityCommand : public RptCmdBase
{
public:
	DelEntityCommand(CWnd* pView, TplEntityListPtr pTplEntityList, TplEntityListPtr pDelEntityList, EntityGroupListPtr pEntityGroupList, EntityGroupListPtr pDelEntityGroupList);
	~DelEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TplEntityListPtr m_pTplEntityList;
	TplEntityListPtr m_pDelEntityList;

	EntityGroupListPtr m_pEntityGroupList;
	EntityGroupListPtr m_pDelEntityGroupList;
};

END_CUTLEADER_NAMESPACE()
