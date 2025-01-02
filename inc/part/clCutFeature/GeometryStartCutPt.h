#pragma once

#include "clCutFeatureCommon.h"
#include "LoopStartCutPtBase.h"

DECLARE_CUTLEADER_CLASS(GeometryStartCutPt)

BEGIN_CUTLEADER_NAMESPACE()

// this class describe the start/end cut point which at the geom of close path.
class ClCutFeature_Export GeometryStartCutPt : public LoopStartCutPtBase
{
public:
	GeometryStartCutPt();
	GeometryStartCutPt(LONGLONG iStartPatID, const PatternPosition& patternPosition, BOOL bFollowLoopDirection);
	GeometryStartCutPt(LONGLONG iStartPatID, const PatternPosition& patternPosition, double dOverCut, BOOL bFollowLoopDirection);
	virtual ~GeometryStartCutPt();

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public: // implement interface ILoopStartCutPt.
	virtual SECUT_PT_TYPE GetSECutPtType() const override { return SECUT_PT_GEOM; }

public: // get/set functions.
	void SetOverCut(double dOverCut) { m_dOverCut = dOverCut; }
	double GetOverCut() const { return m_dOverCut; }

	void FollowLoopDirection(BOOL bFollowLoopDirection) { m_bFollowLoopDirection = bFollowLoopDirection; }
	BOOL FollowLoopDirection() const { return m_bFollowLoopDirection; }

private:
	// the over cut of the end cut point.
	// notes:
	//   1) the positive value means the over cut direction is follow the cut direction.
	//   2) the over cut cannot exceed the geom base which hold the start cut point.
	double m_dOverCut;

	// whether the cut direction is same with the loop direction.
	// notes:
	//   1) place this in this class because it will decide the end cut point.
	BOOL m_bFollowLoopDirection;
};

END_CUTLEADER_NAMESPACE()
