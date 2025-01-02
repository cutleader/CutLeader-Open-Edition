#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(LoopStaEndCutList)
DECLARE_CUTLEADER_CLASS(CondNode)

BEGIN_CUTLEADER_NAMESPACE()

// �ӡ���·�������г��㡱��ɾ���и����������
class DelLoopPierceCond : public CommandBase
{
public:
	DelLoopPierceCond(EditorData editorData, LoopStaEndCutListPtr pLoopStaEndCutList, BOOL bModLeadIn, LONGLONG iDelCondTypeID);
	~DelLoopPierceCond(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopStaEndCutListPtr m_pLoopStaEndCutList;

	// �޸������ߵ��������������ߵġ�
	BOOL m_bModLeadIn;

	// Ҫɾ���������ڵ㡣
	LONGLONG m_iDelCondTypeID;

	// ��ɾ���������ڵ㡣
	// ע��
	//   1) �е�Ϊ�ա�
	std::vector<CondNodePtr> m_delConNodeList;
};

END_CUTLEADER_NAMESPACE()
