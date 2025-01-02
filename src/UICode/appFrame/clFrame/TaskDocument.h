#pragma once

#include "clFrameCommon.h"
#include "cLeaderDoc.h"


DECLARE_CUTLEADER_CLASS(TaskItem)


BEGIN_CUTLEADER_NAMESPACE()

class CLFRAME_Export TaskDocument : public CLeaderDoc
{
protected:
    DECLARE_DYNCREATE(TaskDocument)

	TaskDocument();

public:
    virtual ~TaskDocument();

public:
    virtual BOOL DoFileSave() { return OnSaveDocument(); }
	virtual BOOL OnNewDocument();
    virtual BOOL OnSaveDocument();
    virtual void SetModifyStatus();

protected:
	DECLARE_MESSAGE_MAP()

public: // get/set functions.
    void SetTaskItem(TaskItemPtr pTaskItem) { m_pTaskItem = pTaskItem; }
    TaskItemPtr GetTaskItem() const { return m_pTaskItem; }

private:
	// �������
	// ע��
	// 1) ����һ������󣬿�����Ϊ�������ͱ������ĵ��������������ڣ�����ʵ���������ط�Ҳ���������������ã�
	//    ��������������Ͱ�Ĺ�������Ҳ�и������������ã����Թر��ĵ������Զ������������ö�Ҫ�����
	//    ���������ڴ����˳���ͷţ������������ڴ�й¶��
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE();
