#pragma once

#include "clPartLayoutCommon.h"
#include "CommandBase.h"
#include "PatternPosition.h"

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“微连接点”的命令。
class ClPartLayout_Export MicroJointsPtModifyCommand : public CommandBase
{
public:
	MicroJointsPtModifyCommand(EditorData editorData, MicroJointListPtr pMicroJointList, LONGLONG iNewPatID, const PatternPosition& newPatPos);
	~MicroJointsPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要修改的微连接特征。
	MicroJointListPtr m_pMicroJointList;

	// 新的微连接点。
	LONGLONG m_iNewPatID;
	PatternPosition m_newPatPos;

	// 老的微连接点。<PatID, PatPos>
	std::vector<std::pair<LONGLONG, PatternPosition>> m_oldMJDataList;
};

END_CUTLEADER_NAMESPACE()
