#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJoint)

BEGIN_CUTLEADER_NAMESPACE()

// ���Ӷ��΢���ӵ����
class ClPartLayout_Export MicroJointsAddCommand : public CommandBase
{
public:
	MicroJointsAddCommand(EditorData editorData, const std::vector<std::pair<LoopMicroJointPtr, MicroJointPtr>>& microJointDataList);
	virtual ~MicroJointsAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	std::vector<std::pair<LoopMicroJointPtr, MicroJointPtr>> m_microJointDataList;
};

END_CUTLEADER_NAMESPACE()
