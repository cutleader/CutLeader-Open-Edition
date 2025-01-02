#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// 启用/禁用微连接引入/引出线。
class ClCutFeature_Export MicroJointLeadModifyCommand : public CommandBase
{
public:
	MicroJointLeadModifyCommand(EditorData editorData, MicroJointListPtr pMicroJointList, BOOL bModLeadIn, BOOL bNewVal);
	~MicroJointLeadModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要修改的微连接特征。
	MicroJointListPtr m_pMicroJointList;

	// 修改引入还是引出。
	BOOL m_bModLeadIn;

	// 新的值。
	BOOL m_bNewVal;

	// 老的值。
	std::vector<BOOL> m_bOldValList;
};

END_CUTLEADER_NAMESPACE()
