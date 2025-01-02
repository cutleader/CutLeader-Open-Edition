#pragma once

#include "clGeometryFeatureCommon.h"
#include "PatternEditCommandBase.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// the command will delete patterns.
class ClGeometryFeature_Export PatternDeleteCommand : public PatternEditCommandBase
{
public:
	PatternDeleteCommand(const EditorData& editorData, PartCadDataPtr pPartCadData, PatternListPtr pSelectedPats, const std::vector<LONGLONG>& textStructs_willDelete);
	~PatternDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pSelectedPats;
	std::vector<LONGLONG> m_textStructs_willDelete;
};

END_CUTLEADER_NAMESPACE()