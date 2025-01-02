#pragma once

#include "clCutSequenceCommon.h"
#include "PatternPosition.h"

BEGIN_CUTLEADER_NAMESPACE()

// 桥接点。
class BridgePt
{
public:
	BridgePt(void);
	BridgePt(LONGLONG iPatternID, const PatternPosition& patternPosition);
	~BridgePt(void);

public: // get/set functions.
	void SetPatternID(LONGLONG iPatternID) { m_iPatID = iPatternID; }
	LONGLONG GetPatternID() const{ return m_iPatID; }

	PatternPosition GetPatternPosition() const { return m_patPos; }
	void SetPatternPosition(const PatternPosition& patternPosition) { m_patPos = patternPosition; }

public:
	// 桥接点位置。
	LONGLONG m_iPatID;
	PatternPosition m_patPos;
};

END_CUTLEADER_NAMESPACE()
