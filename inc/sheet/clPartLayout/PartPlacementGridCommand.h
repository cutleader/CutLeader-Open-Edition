#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)
DECLARE_CUTLEADER_CLASS(PartInstance)

BEGIN_CUTLEADER_NAMESPACE()

// �������һ��������С�
// ע:
// 1) ��ԭ�е�������ø���Ϊ���У��������½�һ��������ã���ô����Ŀ����Ϊ�˱���֮ǰ�ļӹ����ա�
class ClPartLayout_Export PartPlacementGridCommand : public CommandBase
{
public:
	PartPlacementGridCommand(const EditorData& editorData, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		PartPlacementPtr pPartPlacement, int iRowCount, int iColCount, double dSpacingX, double dSpacingY);
	~PartPlacementGridCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	PartPlacementListPtr m_pPartPlacements;
	PartInstanceListPtr m_pPartInstances;
	PartPlacementPtr m_pPartPlacement;

	// Ҫ�������������С�
	int	m_iRowCount;
	int	m_iColCount;
	double m_dSpacingX;
	double m_dSpacingY;

	// �������ʵ����undoʱ�����ʵ��Ҫ�ָ���ȥ����������һЩ����Ҳ�ָܻ���ȥ�����繤��
	PartInstancePtr m_pOldPartInstance;
};

END_CUTLEADER_NAMESPACE()
