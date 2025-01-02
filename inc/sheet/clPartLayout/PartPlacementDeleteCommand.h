#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartGroupList)


BEGIN_CUTLEADER_NAMESPACE()

// 该命令从板材上删除零件放置。
class ClPartLayout_Export PartPlacementDeleteCommand : public CommandBase
{
public:
	PartPlacementDeleteCommand(EditorData editorData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroupList,
        PartPlacementListPtr pDelPartPmtList);
	virtual ~PartPlacementDeleteCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	PartPlacementListPtr m_pPartPmtList;
	PartInstanceListPtr m_pPartInstList;
    PartGroupListPtr m_pPartGroupList;

	// 要删除的零件放置。
	PartPlacementListPtr m_pDelPartPmtList;

	// 缓存数据，undo时用到。
	// 注：
	//  1) 构造函数中初始化。
	PartInstanceListPtr m_pOldPartInsts; // 要删除的零件放置对应的零件实例。
    PartGroupListPtr m_pPartGroups_deleted; // 被删除的零件组，是直接或间接包含这些零件放置的分组。
};

END_CUTLEADER_NAMESPACE()
