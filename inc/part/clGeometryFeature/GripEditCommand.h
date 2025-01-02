#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// this command can grip-edit the patterns.
class ClGeometryFeature_Export GripEditCommand : public CommandBase
{
public:
	GripEditCommand(EditorData editorData, IPatternPtr pNewPat, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, IPatternPtr pOldPat);
	~GripEditCommand(void);

public:
	//   1) after call this:
	//      a) m_pNewPat: the transformed patterns and "original".
	//      b) m_pOldPat: the not transformed patterns and tmp.
	virtual void Do();

	//   1) after call this:
	//      a) m_pNewPat: the transformed patterns and tmp.
	//      b) m_pOldPat: the not transformed patterns and "original".
	virtual void UnDo();

private:
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	IPatternPtr m_pNewPat;
	IPatternPtr m_pOldPat;
};

END_CUTLEADER_NAMESPACE()
