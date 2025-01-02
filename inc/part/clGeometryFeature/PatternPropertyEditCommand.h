#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// the command will change the prop of pattern.
class ClGeometryFeature_Export PatternPropertyEditCommand : public CommandBase
{
public:
	PatternPropertyEditCommand(EditorData editorData, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems,
		IPatternPtr pPattern, DataPropItem* pNewPropItem, DataPropItem* pOldPropItem);
	~PatternPropertyEditCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternLoopListPtr m_pPatternLoopList;
	LoopTopologyItemListPtr m_pLoopTopologyItems;

	// the pattern.
	IPatternPtr m_pPattern;

	DataPropItem* m_pNewPropItem;
	DataPropItem* m_pOldPropItem;
};

END_CUTLEADER_NAMESPACE()
