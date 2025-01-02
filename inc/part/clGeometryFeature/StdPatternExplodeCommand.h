#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// the command that explode the standard pattern.
class ClGeometryFeature_Export StdPatternExplodeCommand : public CommandBase
{
public:
	StdPatternExplodeCommand(EditorData editorData, PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, IPatternPtr pStdPat);
	~StdPatternExplodeCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pPatList;
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	// the standard pattern.
	IPatternPtr m_pStdPat;

	// line/arc patterns of the standard pattern.
	PatternListPtr m_pLineArcPatList;
};

END_CUTLEADER_NAMESPACE()
