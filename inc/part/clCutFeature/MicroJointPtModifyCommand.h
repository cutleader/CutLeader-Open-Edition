#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(MicroJoint)

BEGIN_CUTLEADER_NAMESPACE()

// �޸ġ�΢���ӵ㡱�����
class ClCutFeature_Export MicroJointPtModifyCommand : public CommandBase
{
public:
	MicroJointPtModifyCommand(EditorData editorData, MicroJointPtr pMJ, LONGLONG iNewPatID, PatternPosition newPatPos);
	~MicroJointPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ�޸ĵ�΢����������
	MicroJointPtr m_pMJ;

	// �µ�΢���ӵ㡣
	LONGLONG m_iNewPatID;
	PatternPosition m_newPatPos;

	// �ϵ�΢���ӵ㡣
	LONGLONG m_iOldPatID;
	PatternPosition m_oldPatPos;
};

END_CUTLEADER_NAMESPACE()
