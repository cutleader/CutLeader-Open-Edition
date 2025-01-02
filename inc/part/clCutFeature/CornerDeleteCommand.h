#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopCornerData)
DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(CornerList)

BEGIN_CUTLEADER_NAMESPACE()

// 删除角特征的命令。
class ClCutFeature_Export CornerDeleteCommand : public CommandBase
{
public:
	CornerDeleteCommand(const EditorData& editorData, LoopCornerDataListPtr pLoopCornerList, CornerListPtr pCornerList);
	~CornerDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	// 所有的回路特征。
	LoopCornerDataListPtr m_pLoopCornerList;

	// 要删除的角特征。
	CornerListPtr m_pDelCornerList;

	// 记录了“回路角特征”和其ID的对应。
	std::map<LONGLONG, LoopCornerDataPtr> m_loopCornerInfoList;
};

END_CUTLEADER_NAMESPACE()
