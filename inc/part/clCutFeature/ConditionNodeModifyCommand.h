#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 修改轮廓刀具上的指定条件的值。
class ClCutFeature_Export ConditionNodeModifyCommand : public CommandBase
{
public:
	ConditionNodeModifyCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, LONGLONG iCondTypeID, CString strNewVal);
	~ConditionNodeModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;

	// 要修改的条件。
	LONGLONG m_iCondTypeID;

	// 老的条件值<LoopToolData id, condition value>。
	std::map<LONGLONG, CString> m_oldValList;

	// 新的条件值。
	CString m_strNewVal;
};

END_CUTLEADER_NAMESPACE()
