#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

// �޸ġ����������ͺ�ֵ�������
// ע��
//   1) �޸ĵķ�ʽ���滻���ϵĽ���������
class ClCutFeature_Export CornerModifyCommand : public CommandBase
{
public:
	CornerModifyCommand(EditorData editorData, LoopCornerDataListPtr pLoopCornerList, CornerListPtr pOldCornerList, CornerListPtr pNewCornerList);
	~CornerModifyCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopCornerDataListPtr m_pLoopCornerList;

	// �ϵĽ���������
	CornerListPtr m_pOldCornerList;

	// ��Ӧ�ϡ����������󡱵�ÿ���¡����������󡱡�
	CornerListPtr m_pNewCornerList;

	// ��¼�ˡ���·������������ID�Ķ�Ӧ��
	std::map<LONGLONG, LoopCornerDataPtr> m_loopCornerInfoList;
};

END_CUTLEADER_NAMESPACE()
