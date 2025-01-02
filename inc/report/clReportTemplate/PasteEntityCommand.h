#pragma once

#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export PasteEntityCommand : public RptCmdBase
{
public:
	PasteEntityCommand(CWnd* pView, TplEntityListPtr pTplEntityList, TplEntityListPtr pNewEntityList);
	~PasteEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TplEntityListPtr m_pTplEntityList;
	TplEntityListPtr m_pNewEntityList;
};

END_CUTLEADER_NAMESPACE()
