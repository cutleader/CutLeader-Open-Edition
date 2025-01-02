#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"


DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)


BEGIN_CUTLEADER_NAMESPACE()

// �������ը������顣
class ClPartLayout_Export DeletePartGroupCommand : public CommandBase
{
public:
    DeletePartGroupCommand(const EditorData& editorData, PartGroupListPtr pPartGroupList, PartGroupPtr pPartGroup_willBeDeleted);
    ~DeletePartGroupCommand();

public:
    virtual void Do();
    virtual void UnDo();

private:
    PartGroupListPtr m_pPartGroupList;
    PartGroupPtr m_pPartGroup_willBeDeleted;
};

END_CUTLEADER_NAMESPACE()
