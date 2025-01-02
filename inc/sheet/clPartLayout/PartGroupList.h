#pragma once

#include "clPartLayoutCommon.h"
#include "ClData.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(PartGroup)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(PartPlacement)


BEGIN_CUTLEADER_NAMESPACE()

// ������顱����
class ClPartLayout_Export PartGroupList : public std::vector<PartGroupPtr>, public ClData
{
public:
	PartGroupList();
	~PartGroupList(void);

public:
    PartGroupPtr GetPartGroupByID(LONGLONG iPartGroupID) const;
    void RemovePartGroup(const PartGroup* pPartGroup);

    // �õ�ֱ�Ӱ������������ö���ķ��飬�ú�������ÿ������ʱ�����ݹ������
    PartGroupPtr GetPartGroupIncludePartPlacement(const PartPlacement* pPartPlacement) const;

    // �õ��������Ķ�������飬��Ϊ���������л��������ϵ��
    // ע��
    // 1�����û�����������������飬�Ǿͷ����Լ���
    PartGroupPtr GetTopLevelPartGroup(PartGroupPtr pPartGroup) const;

    // �õ�����������������飬��Ϊ���������л��������ϵ��
    PartGroupPtr GetParentPartGroup(const PartGroup* pPartGroup) const;
};

END_CUTLEADER_NAMESPACE()
