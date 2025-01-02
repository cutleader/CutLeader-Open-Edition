#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MicroJointList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改微连接宽度命令。
class ClCutFeature_Export MicroJointWidthModifyCommand : public CommandBase
{
public:
	MicroJointWidthModifyCommand(EditorData editorData, MicroJointListPtr pMicroJointList, double dNewWidth);
	~MicroJointWidthModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 要修改的微连接特征。
	MicroJointListPtr m_pMicroJointList;

	// 新的微连接宽度。
	double m_dNewWidth;

	// 老的微连接宽度。
	std::vector<double> m_dOldWidthList;
};

END_CUTLEADER_NAMESPACE()
