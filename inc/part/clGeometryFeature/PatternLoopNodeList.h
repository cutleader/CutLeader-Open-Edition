#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(PatternLoopNode)
DECLARE_CUTLEADER_CLASS(PatternLoopNodeList)
DECLARE_CUTLEADER_CLASS(IPattern)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryFeature_Export PatternLoopNodeList : public std::vector<PatternLoopNodePtr>,
										   public ClData
{
public:
	PatternLoopNodeList(void);
	virtual ~PatternLoopNodeList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

	// clone the loop nodes.
	// notes:
	//   1) do not clone the pattern within the loop node.
	virtual IDataPtr Clone() const override;

public:
	void RemovePattern(IPatternPtr pPattern);
};

END_CUTLEADER_NAMESPACE()
