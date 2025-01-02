#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(IArrayPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// the command that explode the array pattern.
class ClGeometryFeature_Export ArrayPatternExplodeCommand : public CommandBase
{
public:
	ArrayPatternExplodeCommand(EditorData editorData, PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList,
		LoopTopologyItemListPtr pLoopTopologyItems, IArrayPatternPtr pArrayPattern);
	~ArrayPatternExplodeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pPatList;
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	// about the array pattern.
	IPatternPtr m_ctrlPat;
	IArrayPatternPtr m_pArrayPat;

	// pattern insts of the array pattern including the first inst.
	PatternListPtr m_repeatPatList;
};

END_CUTLEADER_NAMESPACE()
