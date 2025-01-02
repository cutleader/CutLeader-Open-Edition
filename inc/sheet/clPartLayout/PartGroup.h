#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"


DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartPlacement)


BEGIN_CUTLEADER_NAMESPACE()

// ����飬�ǵݹ����ݽṹ��������������԰�����������������ö���
class ClPartLayout_Export PartGroup : public ClData
{
public:
	PartGroup(PartGroupListPtr pSubPartGroups, PartPlacementListPtr pPartPlacements);
	~PartGroup(void);

public:
    const PartGroupList* GetSubPartGroups() const { return m_pSubPartGroups.get(); }
    const PartPlacementList* GetPartPlacements() const { return m_pPartPlacements.get(); }

public:
    // �ݹ�õ���������е�����������á�
    static PartPlacementListPtr GetAllPartPlacementsInThisGroup(const PartGroup* pPartGroup);

    // ����������ǲ��������������С�
    bool IsIncludePartPlacement(const PartPlacement* pPartPlacement) const; // ֻ���һ�㣬���ǵݹ顣
    bool IsIncludePartPlacement_2(const PartPlacement* pPartPlacement) const; // �ǵݹ麯����

    // ��������ǲ��ǰ������������У�ֻ���һ�㣬���ǵݹ顣
    bool IsIncludePartGroup(const PartGroup* pPartGroup) const;

    // �ݹ��¡����飬������ö����������¡�ô���ġ�
    PartGroupPtr ClonePartGroupRecursively(const PartPlacementList* pPartPlacements_old, const PartPlacementList* pPartPlacements_new) const;

    // �ݹ�õ������ӷ��顣
    PartGroupListPtr GetSubPartGroupsRecursively() const;

private:
    // ������顣
    PartGroupListPtr m_pSubPartGroups;

    // ������һ���������ö���
    PartPlacementListPtr m_pPartPlacements;
};

END_CUTLEADER_NAMESPACE()
