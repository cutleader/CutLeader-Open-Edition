#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartGroupList)


BEGIN_CUTLEADER_NAMESPACE()

// ������Ӱ����ɾ��������á�
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

	// Ҫɾ����������á�
	PartPlacementListPtr m_pDelPartPmtList;

	// �������ݣ�undoʱ�õ���
	// ע��
	//  1) ���캯���г�ʼ����
	PartInstanceListPtr m_pOldPartInsts; // Ҫɾ����������ö�Ӧ�����ʵ����
    PartGroupListPtr m_pPartGroups_deleted; // ��ɾ��������飬��ֱ�ӻ��Ӱ�����Щ������õķ��顣
};

END_CUTLEADER_NAMESPACE()
