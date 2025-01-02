#pragma once

#include "clReportTemplateCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(RptCmdList)
DECLARE_CUTLEADER_CLASS(IRptCmd)

BEGIN_CUTLEADER_NAMESPACE()

// the command list.
class ClReportTemplate_Export RptCmdList : public std::vector<IRptCmdPtr>
{
public:
	RptCmdList(void);
	~RptCmdList(void);

public:
	// get the commands from view.
	RptCmdListPtr GetCommandByView(CWnd* pView);

	// remove the commands from the list.
	void RemoveCommand(RptCmdListPtr pCommandList);

	// remove the command from the list.
	// notes:
	//   1) before call this, caller should make sure the list has "iCommandID".
	void RemoveCommand(LONGLONG iCommandID);
};

END_CUTLEADER_NAMESPACE()
