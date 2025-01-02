#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopCornerData)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(CornerList)

BEGIN_CUTLEADER_NAMESPACE()

// ɾ�������������
class ClCutFeature_Export CornerDeleteCommand : public CommandBase
{
public:
	CornerDeleteCommand(const EditorData& editorData, LoopCornerDataListPtr pLoopCornerList, CornerListPtr pCornerList);
	~CornerDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// ���еĻ�·������
	LoopCornerDataListPtr m_pLoopCornerList;

	// Ҫɾ���Ľ�������
	CornerListPtr m_pDelCornerList;

	// ��¼�ˡ���·������������ID�Ķ�Ӧ��
	std::map<LONGLONG, LoopCornerDataPtr> m_loopCornerInfoList;
};

END_CUTLEADER_NAMESPACE()
