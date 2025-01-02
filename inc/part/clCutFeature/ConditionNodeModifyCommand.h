#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// �޸����������ϵ�ָ��������ֵ��
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

	// Ҫ�޸ĵ�������
	LONGLONG m_iCondTypeID;

	// �ϵ�����ֵ<LoopToolData id, condition value>��
	std::map<LONGLONG, CString> m_oldValList;

	// �µ�����ֵ��
	CString m_strNewVal;
};

END_CUTLEADER_NAMESPACE()
