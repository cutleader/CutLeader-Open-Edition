#pragma once

#include "PatternLoopBase.h"

DECLARE_CUTLEADER_CLASS(PatternLoopNode)
DECLARE_CUTLEADER_CLASS(ArrayPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// the Array pattern loop.
// notes:
//   1) we hold only an Array pattern in this loop, for such loop, functions
//      such as "GetDirection", "IsOpenPath", are applied to the "control" pattern of the array pattern.
class ClGeometryFeature_Export ArrayPatternLoop : public PatternLoopBase
{
public:
	ArrayPatternLoop(PatternLoopNodePtr pPatternLoopNode);
	virtual ~ArrayPatternLoop();

public: // implement IData interface.
	virtual IDataPtr Clone() const override; // do not clone the pattern within the loop node.

public: // implement interface IPatternLoop.
	virtual PatternListPtr GetPatternList() const override; // this function should also return the control pattern.
	virtual void ReverseDirection() override;
	virtual DIRECTION GetDirection() const override;
	virtual Vector2D GetDirectionOnPt(LONGLONG iPatternID, const PatternPosition& patternPosition) const override;
	virtual PatternLoopType GetPatternLoopType() const override { return PatternLoop_Array; }
	virtual BOOL IsOpenPath(double dTol) const override { return FALSE; }
};

END_CUTLEADER_NAMESPACE()
