#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸ġ�΢���ӵ㡱�����
class ClPartLayout_Export MicroJointsPtModifyCommand : public CommandBase
{
public:
	MicroJointsPtModifyCommand(EditorData editorData, MicroJointListPtr pMicroJointList, LONGLONG iNewPatID, const PatternPosition& newPatPos);
	~MicroJointsPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// Ҫ�޸ĵ�΢����������
	MicroJointListPtr m_pMicroJointList;

	// �µ�΢���ӵ㡣
	LONGLONG m_iNewPatID;
	PatternPosition m_newPatPos;

	// �ϵ�΢���ӵ㡣<PatID, PatPos>
	std::vector<std::pair<LONGLONG, PatternPosition>> m_oldMJDataList;
};

END_CUTLEADER_NAMESPACE()
