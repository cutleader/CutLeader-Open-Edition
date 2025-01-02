#pragma once

#include "IData.h"
#include "EditorData.h"

BEGIN_CUTLEADER_NAMESPACE()


// ����ӿڡ�
// ע��
//  1) ��ʵÿ�����������һ���༭����
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

	// ��������Ӧ�ġ��༭�����ݡ���
	virtual const EditorData& GetEditorData() const = 0;
};

END_CUTLEADER_NAMESPACE()
