#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"


DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)


BEGIN_CUTLEADER_NAMESPACE()


class ClPartLayout_Export CreatePartGroupCommand : public CommandBase
{
public:
    CreatePartGroupCommand(const EditorData& editorData, PartGroupListPtr pPartGroupList, PartGroupPtr pNewPartGroup);
    ~CreatePartGroupCommand();

public:
    virtual void Do();
    virtual void UnDo();

private:
    PartGroupListPtr m_pPartGroupList;
    PartGroupPtr m_pNewPartGroup;
};

END_CUTLEADER_NAMESPACE()
