#pragma once

#include "LoopStartCutPtBase.h"

DECLARE_CUTLEADER_CLASS(CornerStartCutPt)

BEGIN_CUTLEADER_NAMESPACE()

// this class describe the start/end cut point which at the corner of the close path.
// notes:
//   1) in this class we record two geoms which meet at the corner point, here "first" and "second" do not mean the cut order.
class ClCutFeature_Export CornerStartCutPt : public LoopStartCutPtBase
{
public:
	CornerStartCutPt();
	CornerStartCutPt(LONGLONG iFirstPatId, const PatternPosition& firstPatPos, LONGLONG iSecondPatId, const PatternPosition& secondPatPos, BOOL bFollowLoopDirection);
	CornerStartCutPt(LONGLONG iFirstPatId, const PatternPosition& firstPatPos, LONGLONG iSecondPatId, const PatternPosition& secondPatPos, double dOverCut, BOOL bFollowLoopDirection);
	virtual ~CornerStartCutPt();

public: // implement IData interface
	IDataPtr Clone() const override;

public: // implement interface ILoopStartCutPt.
	virtual SECUT_PT_TYPE GetSECutPtType() const override { return SECUT_PT_CORNER; }

public: // get/set functions.
	void SetOverCut(double dOverCut) { m_dOverCut = dOverCut; }
	double GetOverCut() const { return m_dOverCut; }

	LONGLONG GetSecondPatternID() const { return m_iSecondPatternID; }
	void SetSecondPatternID(LONGLONG iPatternID) { m_iSecondPatternID = iPatternID; }

	void SetSecondPatternPosition(const PatternPosition& patternPosition) { m_secondPatternPosition = patternPosition; }
	PatternPosition GetSecondPatternPosition() const { return m_secondPatternPosition; }

	void FollowLoopDirection(BOOL bFollowLoopDirection) { m_bFollowLoopDirection = bFollowLoopDirection; }
	BOOL FollowLoopDirection() const { return m_bFollowLoopDirection; }

private:
	// 过切值
	// 注：
	//   1) 过切值为正时，沿着切割方向进行过切；反之同理。 
	double m_dOverCut;

	// the second pattern which meet with pattern "m_iStartPatternID" at the corner point.
	// notes:
	//   1) for standard and array pattern loop, these are for the "only" pattern.
	LONGLONG m_iSecondPatternID;
	PatternPosition m_secondPatternPosition;

	// whether the cut direction is same with the loop direction.
	BOOL m_bFollowLoopDirection;
};

END_CUTLEADER_NAMESPACE()
