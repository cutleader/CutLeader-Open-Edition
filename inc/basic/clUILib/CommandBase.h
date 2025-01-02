#pragma once

#include "clUILibCommon.h"
#include "ICommand.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for ICommand interface.
class ClUILib_Export CommandBase : virtual public ICommand, public ClData
{
public:
	CommandBase(void);
	CommandBase(const CString& strCommandName, const EditorData& editorData);
	virtual ~CommandBase(void);

public: // implement interface ICommand.
	virtual const CString& GetCommandName() const override { return m_strCommandName; }
	virtual const EditorData& GetEditorData() const override { return m_editorData; }

protected:
	// the name of the command.
	CString m_strCommandName;

	// the view's category info which the command is associated with.
	EditorData m_editorData;
};

END_CUTLEADER_NAMESPACE()
