#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// 删除微连接的命令。
class ClCutFeature_Export MicroJointDeleteCommand : public CommandBase
{
public:
	MicroJointDeleteCommand(EditorData editorData, LoopMicroJointListPtr pLoopMicroJointList, MicroJointListPtr pDelMJList);
	~MicroJointDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 所有的回路微连接特征。
	LoopMicroJointListPtr m_pLoopMicroJointList;

	// 要删除的微连接特征。
	MicroJointListPtr m_pDelMJList;

	// 记录了“回路微连接特征”和其ID的对应。
	std::map<LONGLONG, LoopMicroJointPtr> m_loopMJInfoList;
};

END_CUTLEADER_NAMESPACE()
