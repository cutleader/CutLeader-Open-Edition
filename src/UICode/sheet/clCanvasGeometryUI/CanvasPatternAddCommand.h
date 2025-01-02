#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令往底图上添加一个几何特征。
class CanvasPatternAddCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasPatternAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData, IPatternPtr pNewPattern);
	~CanvasPatternAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	IPatternPtr m_pNewPattern;
};

END_CUTLEADER_NAMESPACE()
