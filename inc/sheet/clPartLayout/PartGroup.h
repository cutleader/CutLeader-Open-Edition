#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"


DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartPlacement)


BEGIN_CUTLEADER_NAMESPACE()

// 零件组，是递归数据结构，零件组下来可以包含子零件组和零件放置对象。
class ClPartLayout_Export PartGroup : public ClData
{
public:
	PartGroup(PartGroupListPtr pSubPartGroups, PartPlacementListPtr pPartPlacements);
	~PartGroup(void);

public:
    const PartGroupList* GetSubPartGroups() const { return m_pSubPartGroups.get(); }
    const PartPlacementList* GetPartPlacements() const { return m_pPartPlacements.get(); }

public:
    // 递归得到该零件组中的所有零件放置。
    static PartPlacementListPtr GetAllPartPlacementsInThisGroup(const PartGroup* pPartGroup);

    // 该零件放置是不是在这个零件组中。
    bool IsIncludePartPlacement(const PartPlacement* pPartPlacement) const; // 只检查一层，不是递归。
    bool IsIncludePartPlacement_2(const PartPlacement* pPartPlacement) const; // 是递归函数。

    // 该零件组是不是包含这个零件组中，只检查一层，不是递归。
    bool IsIncludePartGroup(const PartGroup* pPartGroup) const;

    // 递归克隆零件组，零件放置对象是外面克隆好传入的。
    PartGroupPtr ClonePartGroupRecursively(const PartPlacementList* pPartPlacements_old, const PartPlacementList* pPartPlacements_new) const;

    // 递归得到所有子分组。
    PartGroupListPtr GetSubPartGroupsRecursively() const;

private:
    // 子零件组。
    PartGroupListPtr m_pSubPartGroups;

    // 零件组第一层的零件放置对象。
    PartPlacementListPtr m_pPartPlacements;
};

END_CUTLEADER_NAMESPACE()
