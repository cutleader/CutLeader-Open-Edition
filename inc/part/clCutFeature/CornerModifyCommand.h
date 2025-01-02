#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <map>

DECLARE_CUTLEADER_CLASS(LoopCornerDataList)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

// 修改“角特征类型和值”的命令。
// 注：
//   1) 修改的方式是替换掉老的角特征对象。
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

	// 老的角特征对象。
	CornerListPtr m_pOldCornerList;

	// 对应老“角特征对象”的每个新“角特征对象”。
	CornerListPtr m_pNewCornerList;

	// 记录了“回路角特征”和其ID的对应。
	std::map<LONGLONG, LoopCornerDataPtr> m_loopCornerInfoList;
};

END_CUTLEADER_NAMESPACE()
