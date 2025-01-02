#pragma once

#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(EntityGroupList)

BEGIN_CUTLEADER_NAMESPACE()

// this command will unGroup the entities.
class ClReportTemplate_Export UnGroupEntityCommand : public RptCmdBase
{
public:
	UnGroupEntityCommand(CWnd* pView, EntityGroupListPtr pEntityGroupList, EntityGroupListPtr pDelEntityGroupList);
	~UnGroupEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	EntityGroupListPtr m_pEntityGroupList;
	EntityGroupListPtr m_pDelEntityGroupList;
};

END_CUTLEADER_NAMESPACE()
