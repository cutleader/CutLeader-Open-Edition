#pragma once

#include "PatternLoopBase.h"

DECLARE_CUTLEADER_CLASS(PatternLoopNode)
DECLARE_CUTLEADER_CLASS(StdPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// the standard pattern loop.
class ClGeometryFeature_Export StdPatternLoop : public PatternLoopBase
{
public:
	StdPatternLoop(PatternLoopNodePtr pPatternLoopNode);
	virtual ~StdPatternLoop();

public: // implement IData interface.
	virtual IDataPtr Clone() const override; // do not clone the pattern within the loop node.

public: // implement interface IPatternLoop.
	virtual void ReverseDirection() override;
	virtual DIRECTION GetDirection() const override;
	virtual Vector2D GetDirectionOnPt(LONGLONG iPatternID, const PatternPosition& patternPosition) const override;
	virtual PatternLoopType GetPatternLoopType() const override { return PatternLoop_Std; }
	virtual BOOL IsOpenPath(double dTol) const override { return FALSE; }
};

END_CUTLEADER_NAMESPACE()
