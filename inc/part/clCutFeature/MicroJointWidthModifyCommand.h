#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸�΢���ӿ�����
class ClCutFeature_Export MicroJointWidthModifyCommand : public CommandBase
{
public:
	MicroJointWidthModifyCommand(EditorData editorData, MicroJointListPtr pMicroJointList, double dNewWidth);
	~MicroJointWidthModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ�޸ĵ�΢����������
	MicroJointListPtr m_pMicroJointList;

	// �µ�΢���ӿ�ȡ�
	double m_dNewWidth;

	// �ϵ�΢���ӿ�ȡ�
	std::vector<double> m_dOldWidthList;
};

END_CUTLEADER_NAMESPACE()
