#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// ����/����΢��������/�����ߡ�
class ClCutFeature_Export MicroJointLeadModifyCommand : public CommandBase
{
public:
	MicroJointLeadModifyCommand(EditorData editorData, MicroJointListPtr pMicroJointList, BOOL bModLeadIn, BOOL bNewVal);
	~MicroJointLeadModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ�޸ĵ�΢����������
	MicroJointListPtr m_pMicroJointList;

	// �޸����뻹��������
	BOOL m_bModLeadIn;

	// �µ�ֵ��
	BOOL m_bNewVal;

	// �ϵ�ֵ��
	std::vector<BOOL> m_bOldValList;
};

END_CUTLEADER_NAMESPACE()
