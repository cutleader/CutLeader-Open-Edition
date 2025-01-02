#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// ɾ��΢���ӵ����
class ClCutFeature_Export MicroJointDeleteCommand : public CommandBase
{
public:
	MicroJointDeleteCommand(EditorData editorData, LoopMicroJointListPtr pLoopMicroJointList, MicroJointListPtr pDelMJList);
	~MicroJointDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ���еĻ�·΢����������
	LoopMicroJointListPtr m_pLoopMicroJointList;

	// Ҫɾ����΢����������
	MicroJointListPtr m_pDelMJList;

	// ��¼�ˡ���·΢��������������ID�Ķ�Ӧ��
	std::map<LONGLONG, LoopMicroJointPtr> m_loopMJInfoList;
};

END_CUTLEADER_NAMESPACE()
