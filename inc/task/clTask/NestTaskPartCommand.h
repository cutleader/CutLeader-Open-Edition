#pragma once

#include "ClTaskCommon.h"
#include "CommandBase.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(SheetList)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)


BEGIN_CUTLEADER_NAMESPACE()

// �����Ű������������õ��Ľ�����������command������ִ���޸ġ�
// ע��
//  1����������undo/redo���ɰ����ͼ������ʾ�����ġ�
//  2����������ִ����������ͼ����ִ�С�
//  3) ���������������������ͼʱ��������ͼ���ٵġ�
class ClTask_Export NestTaskPartCommand : public CommandBase
{
public:
	NestTaskPartCommand(const EditorData& editorData, TaskItemPtr pTaskItem, TaskPartsNestResultPtr pTaskPartsNestResult);
	~NestTaskPartCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
    TaskItemPtr m_pTaskItem;

    // �Ű���
    TaskPartsNestResultPtr m_pTaskPartsNestResult;
};

END_CUTLEADER_NAMESPACE()
