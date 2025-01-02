#pragma once

#include "IRptCmd.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for IRptCmd interface.
class ClReportTemplate_Export RptCmdBase : virtual public IRptCmd,
									  public ClData
{
public:
	RptCmdBase(void);
	~RptCmdBase(void);

public: // implement interface IRptCmd.
	virtual CString GetCommandName() { return m_strCommandName; }
	CWnd* GetView() { return m_pView; }

protected:
	// the name of the command.
	CString m_strCommandName;

	// the view.
	CWnd* m_pView;
};

END_CUTLEADER_NAMESPACE()
