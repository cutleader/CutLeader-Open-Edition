#pragma once

#include "clReportTemplateCommon.h"
#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(EntityGroupList)
DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

// this command will cut the entities.
// notes:
//   1) this command will also care the associated groups.
class ClReportTemplate_Export CutEntityCommand : public RptCmdBase
{
public:
	CutEntityCommand(CWnd* pView, TplEntityListPtr pTplEntityList, TplEntityListPtr pCutEntityList,
					 EntityGroupListPtr pEntityGroupList, EntityGroupListPtr pDelEntityGroupList);
	~CutEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TplEntityListPtr m_pTplEntityList;
	TplEntityListPtr m_pCutEntityList;

	EntityGroupListPtr m_pEntityGroupList;
	EntityGroupListPtr m_pDelEntityGroupList;
};

END_CUTLEADER_NAMESPACE()
