#pragma once

#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(TplEntityList)
DECLARE_CUTLEADER_CLASS(ITplEntity)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export AddEntityCommand : public RptCmdBase
{
public:
	AddEntityCommand(CWnd* pView, TplEntityListPtr pTplEntityList, ITplEntityPtr pNewTplEntity);
	~AddEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TplEntityListPtr m_pTplEntityList;
	ITplEntityPtr m_pNewTplEntity;
};

END_CUTLEADER_NAMESPACE()
