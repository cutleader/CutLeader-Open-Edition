#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// the command that add a pattern.
class ClGeometryFeature_Export PatternAddCommand : public CommandBase
{
public:
	PatternAddCommand(EditorData editorData, PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, IPatternPtr pPattern);
	~PatternAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pPatList;
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	IPatternPtr m_pPattern;
};

END_CUTLEADER_NAMESPACE()
