#pragma once

#include "IData.h"
#include "EditorData.h"

BEGIN_CUTLEADER_NAMESPACE()


// 命令接口。
// 注：
//  1) 其实每个命令关联到一个编辑器。
class ClUILib_Export ICommand : virtual public IData
{
public:
    virtual ~ICommand() {}

public:
	// execute/redo the command.
	virtual void Do() = 0;

	// undo the command.
	virtual void UnDo() = 0;

	// get the command name.
	virtual const CString& GetCommandName() const = 0;

	// 这个命令对应的“编辑器数据”。
	virtual const EditorData& GetEditorData() const = 0;
};

END_CUTLEADER_NAMESPACE()
