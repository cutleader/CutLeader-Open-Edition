#pragma once

#include "ILoopStartCutPt.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic implementation for interface ILoopStartCutPt.
class ClCutFeature_Export LoopStartCutPtBase : virtual public ILoopStartCutPt, public ClData
{
public:
	LoopStartCutPtBase();
	virtual ~LoopStartCutPtBase();

public: // implement interface ILoopStartCutPt.
	void SetStartPatID(LONGLONG iStartPatID) override { m_iStartPatternID = iStartPatID; }
	LONGLONG GetStartPatID() const override { return m_iStartPatternID; }

	void SetPatternPosition(const PatternPosition& patternPosition) override { m_patternPosition = patternPosition; }
	PatternPosition GetPatternPosition() const override { return m_patternPosition; }

protected:
	// the start pattern id.
	// notes:
	//   1) for PatternLoop_Std and PatternLoop_Array, this is the ID of the "only" pattern.
	LONGLONG m_iStartPatternID;

	// the start point position on the pattern.
	// notes:
	//   1) for PatternLoop_Std and PatternLoop_Array, this is on the "only" pattern.
	PatternPosition m_patternPosition;
};

END_CUTLEADER_NAMESPACE()
