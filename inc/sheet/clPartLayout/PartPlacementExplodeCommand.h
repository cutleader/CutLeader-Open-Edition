#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

class ClPartLayout_Export PartPlacementExplodeCommand : public CommandBase
{
public:
	PartPlacementExplodeCommand(EditorData editorData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartPlacementPtr pGridPmt, PartPlacementListPtr pNewPartPlacements);
	~PartPlacementExplodeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PartPlacementListPtr m_pPartPmtList;
	PartInstanceListPtr m_pPartInstList;

	// Ҫըɢ��������С�
	PartPlacementPtr m_pGridPmt;

	// ըɢ���������á�
	PartPlacementListPtr m_pNewPartPmtList;

	// �������ݡ�
	// ע��
	//  1) ���캯���г�ʼ����
	PartInstanceListPtr m_pOldPartInsts; // Ҫըɢ��������ж�Ӧ�����ʵ����
	PartInstanceListPtr m_pNewPartInsts; // ըɢ���������ö�Ӧ�����ʵ����
};

END_CUTLEADER_NAMESPACE()
