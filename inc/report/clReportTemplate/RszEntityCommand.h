#pragma once

#include "clReportTemplateCommon.h"
#include "RptCmdBase.h"

DECLARE_CUTLEADER_CLASS(ITplEntity)
DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

// this command will resize the entities.
// notes:
//   1) this command do not change the entity ID.
class ClReportTemplate_Export RszEntityCommand : public RptCmdBase
{
public:
	RszEntityCommand(CWnd* pView, TplEntityListPtr pTplEntityList, ITplEntityPtr pOldEntity, ITplEntityPtr pNewEntity);
	~RszEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TplEntityListPtr m_pTplEntityList;

	// the selected entity.
	ITplEntityPtr m_pSelEntity;

	// the border rect before/after resizing.
	CRect m_borderRect;
};

END_CUTLEADER_NAMESPACE()
