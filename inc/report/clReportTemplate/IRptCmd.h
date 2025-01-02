#pragma once

#include "clReportTemplateCommon.h"
#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClReportTemplate_Export IRptCmd : virtual public IData
{
public:
	// execute/redo the command.
	virtual void Do() = 0;

	// undo the command.
	virtual void UnDo() = 0;

	// get the command name.
	virtual CString GetCommandName() = 0;

	virtual CWnd* GetView() = 0;
};

END_CUTLEADER_NAMESPACE()
