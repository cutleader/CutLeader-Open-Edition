#pragma once

#include "clCutFeatureCommon.h"
#include "CommandBase.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopToolDataList)

BEGIN_CUTLEADER_NAMESPACE()

// 禁用或启用轮廓上的刀具，也就是加工或不加工轮廓。
class ClCutFeature_Export DisableToolOnLoopCommand : public CommandBase
{
public:
	DisableToolOnLoopCommand(EditorData editorData, LoopToolDataListPtr pLoopToolList, bool bDisable);
	~DisableToolOnLoopCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	LoopToolDataListPtr m_pLoopToolList;
    bool m_bDisable;

	// 老的设置
	std::vector<bool> m_oldData;
};

END_CUTLEADER_NAMESPACE()
