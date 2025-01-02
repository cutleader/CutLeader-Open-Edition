#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改过切值。
// 注：
// 1) 值为正时称为过切，值为负时称为缺口。
class ClCutFeature_Export EndCutPtModifyCommand : public CommandBase
{
public:
	EndCutPtModifyCommand(EditorData editorData, LoopStartCutDataListPtr pLoopStaEndCutList, double dNewOverCut);
	~EndCutPtModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopStartCutDataListPtr m_pLoopStaEndCutList;

	// 新的过切值。
	double m_dNewOverCut;

	// 老的过切值。
	std::vector<double> m_dOldOverCutList;

    // 引线在角上时
    std::map<LONGLONG, double> m_lineLeads_changedToZero; // 记录一些直线引出线，这些引线长度从map中的非0值被改成了0
    std::map<LONGLONG, double> m_arcLeads_changedToZero; // 记录一些圆弧引出线，这些引线角度从map中的非0值被改成了0
};

END_CUTLEADER_NAMESPACE()
