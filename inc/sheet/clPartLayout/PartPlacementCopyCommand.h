#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"


DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartGroupList)


BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayout_Export PartPlacementCopyCommand : public CommandBase
{
public:
	PartPlacementCopyCommand(EditorData editorData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pAllPartGroupList,
        PartPlacementListPtr pNewPartPlacements, PartGroupListPtr pNewPartGroups);
	virtual ~PartPlacementCopyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartPlacementListPtr m_pPartPmtList;
    PartInstanceListPtr m_pPartInstList;
    PartGroupListPtr m_pAllPartGroupList;

	PartPlacementListPtr m_pNewPartPmtList;

    // 所有要新建的零件分组。
    PartGroupListPtr m_pNewPartGroups;
};

END_CUTLEADER_NAMESPACE()
