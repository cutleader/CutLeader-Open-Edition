#pragma once

#include "clReportTemplateCommon.h"
#include "RptCmdBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(TplEntityList)

BEGIN_CUTLEADER_NAMESPACE()

// this command will move the entities.
// notes:
//   1) this command do not change the entity ID.
class ClReportTemplate_Export MoveEntityCommand : public RptCmdBase
{
public:
	MoveEntityCommand(CWnd* pView, TplEntityListPtr pTplEntityList, TplEntityListPtr pOldEntityList, TplEntityListPtr pNewEntityList);
	~MoveEntityCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	TplEntityListPtr m_pTplEntityList;

	// the selected entity list.
	TplEntityListPtr m_pSelEntityList;

	// the border rect before/after moving.
	std::vector<CRect> m_borderRectList;
};

END_CUTLEADER_NAMESPACE()
