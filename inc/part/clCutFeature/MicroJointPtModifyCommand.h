#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(MicroJoint)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“微连接点”的命令。
class ClCutFeature_Export MicroJointPtModifyCommand : public CommandBase
{
public:
	MicroJointPtModifyCommand(EditorData editorData, MicroJointPtr pMJ, LONGLONG iNewPatID, PatternPosition newPatPos);
	~MicroJointPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要修改的微连接特征。
	MicroJointPtr m_pMJ;

	// 新的微连接点。
	LONGLONG m_iNewPatID;
	PatternPosition m_newPatPos;

	// 老的微连接点。
	LONGLONG m_iOldPatID;
	PatternPosition m_oldPatPos;
};

END_CUTLEADER_NAMESPACE()
