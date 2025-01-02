#pragma once

#include "EditorData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(CommandList)
DECLARE_CUTLEADER_CLASS(ICommand)

BEGIN_CUTLEADER_NAMESPACE()


// the command list.
class ClUILib_Export CommandList : public std::vector<ICommandPtr>
{
public:
	CommandList(void);
	~CommandList(void);

public:
	// 根据编辑器得到命令集合。
	CommandListPtr GetCmdByEditor(const EditorData& editorData);

	// 根据视图得到命令集合。
	CommandListPtr GetCommandByView(const CWnd* pView);

	// remove the commands from the list.
	void RemoveCommand(CommandListPtr pCommandList);

	// remove the command from the list.
	// notes:
	//   1) before call this, caller should make sure the list has "iCommandID".
	void RemoveCommand(LONGLONG iCommandID);

	ICommandPtr GetCommandByID(LONGLONG iCommandID) const;
};

END_CUTLEADER_NAMESPACE()
