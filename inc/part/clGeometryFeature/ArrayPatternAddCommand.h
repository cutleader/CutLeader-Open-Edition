#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(ArrayPatternBase)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// the command that add an array pattern.
class ClGeometryFeature_Export ArrayPatternAddCommand : public CommandBase
{
public:
	// notes:
	//   1) when call this function, "pCtrlPat" is not a real "control" pattern. and "pArrayPattern" is also not real "be-control" pattern.
	ArrayPatternAddCommand(EditorData editorData, PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems,
		ArrayPatternBasePtr pArrayPattern, IPatternPtr pCtrlPat);
	~ArrayPatternAddCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pPatList;
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	ArrayPatternBasePtr m_pArrayPat;
	IPatternPtr m_pCtrlPat;
};

END_CUTLEADER_NAMESPACE()
