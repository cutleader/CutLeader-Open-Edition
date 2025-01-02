#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令往底图上添加几何轮廓。
class CanvasLoopAddCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasLoopAddCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		PatternListPtr pNewPatterns, PatternLoopListPtr pNewPatternLoops, TextStructListPtr pNewTextStructs);
	~CanvasLoopAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pNewPatterns;
	PatternLoopListPtr m_pNewPatternLoops;
	TextStructListPtr m_pNewTextStructs;
};

END_CUTLEADER_NAMESPACE()
