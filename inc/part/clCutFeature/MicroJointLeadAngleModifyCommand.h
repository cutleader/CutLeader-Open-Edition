#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“微连接引入/引出线角度”的命令。
class ClCutFeature_Export MicroJointLeadAngleModifyCommand : public CommandBase
{
public:
	MicroJointLeadAngleModifyCommand(const EditorData& editorData, MicroJointListPtr pMicroJointList, BOOL bModLeadIn, double dNewAng);
	~MicroJointLeadAngleModifyCommand(void);

public:
	virtual void Do() override;
	virtual void UnDo() override;

private:
	// 要修改的微连接特征。
	MicroJointListPtr m_pMicroJointList;

	// 修改引入还是引出。
	BOOL m_bModLeadIn;

	// 新的角度。
	double m_dNewAng;

	// 老的角度。
	std::vector<double> m_dOldAngList;
};

END_CUTLEADER_NAMESPACE()
