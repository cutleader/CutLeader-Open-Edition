#pragma once

#include "clPartLayoutCommon.h"
#include "CanvasPatternEditCommandBase.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartCamData)

BEGIN_CUTLEADER_NAMESPACE()

// 这个命令从底图上删除几何特征。
class CanvasPatternDeleteCommand : public CanvasPatternEditCommandBase
{
public:
	CanvasPatternDeleteCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PartCamDataPtr pPartCamData,
		PatternListPtr pDeletingPatterns, const std::vector<LONGLONG>& textStructs_willDelete);
	~CanvasPatternDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pDeletingPatterns;
	std::vector<LONGLONG> m_textStructs_willDelete;
};

END_CUTLEADER_NAMESPACE()
