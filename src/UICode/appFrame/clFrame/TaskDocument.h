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
	// 任务对象。
	// 注：
	// 1) 当打开一个任务后，可以认为任务对象就被任务文档管理了生命周期，不过实际上其他地方也存在任务对象的引用，
	//    比如零件管理器和板材管理器中也有该任务对象的引用，所以关闭文档后，所以对任务对象的引用都要解除后，
	//    任务对象的内存才能顺利释放，否则可能造成内存泄露。
    TaskItemPtr m_pTaskItem;
};

END_CUTLEADER_NAMESPACE();
