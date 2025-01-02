#pragma once

#include "ITplEntity.h"
#include "clReportTemplateUICommon.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplateUI_Export TplEntityMgr
{
public:
	// build the template entity with the default values.
	static ITplEntityPtr BuildDflTplEntity(ENTITY_TYPE entityType);

	// edit the property of the entity.
	static void EditEntityProp(ReportTplPagePtr pReportTplPage, ITplEntityPtr pTplEntity);
};

END_CUTLEADER_NAMESPACE()
