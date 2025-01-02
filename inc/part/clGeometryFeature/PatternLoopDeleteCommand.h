#pragma once

#include "clGeometryFeatureCommon.h"
#include "CommandBase.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// the command will delete pattern loops.
class ClGeometryFeature_Export PatternLoopDeleteCommand : public CommandBase
{
public:
	PatternLoopDeleteCommand(EditorData editorData, PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, PatternLoopListPtr pSelLoopList);
	~PatternLoopDeleteCommand(void);

public:
	virtual void Do();
	virtual void UnDo();

private:
	PatternListPtr m_pPatList;
	PatternLoopListPtr m_pPatternLoopList;

	// pattern loops that should be deleted.
	PatternLoopListPtr m_pSelLoopList;
};

END_CUTLEADER_NAMESPACE()