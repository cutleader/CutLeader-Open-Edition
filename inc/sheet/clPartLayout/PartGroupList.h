#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartPlacement)


BEGIN_CUTLEADER_NAMESPACE()

// “零件组”集合
class ClPartLayout_Export PartGroupList : public std::vector<PartGroupPtr>, public ClData
{
public:
	PartGroupList();
	~PartGroupList(void);

public:
    PartGroupPtr GetPartGroupByID(LONGLONG iPartGroupID) const;
    void RemovePartGroup(const PartGroup* pPartGroup);

    // 得到直接包含这个零件放置对象的分组，该函数遍历每个分组时，不递归遍历。
    PartGroupPtr GetPartGroupIncludePartPlacement(const PartPlacement* pPartPlacement) const;

    // 得到该零件组的顶层零件组，因为零件组可以有互相包含关系。
    // 注：
    // 1）如果没有零件组包含这个零件组，那就返回自己。
    PartGroupPtr GetTopLevelPartGroup(PartGroupPtr pPartGroup) const;

    // 得到包含该零件组的零件组，因为零件组可以有互相包含关系。
    PartGroupPtr GetParentPartGroup(const PartGroup* pPartGroup) const;
};

END_CUTLEADER_NAMESPACE()
