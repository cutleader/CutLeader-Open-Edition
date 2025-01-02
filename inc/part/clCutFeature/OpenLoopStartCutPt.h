#pragma once

#include "LoopStartCutPtBase.h"

DECLARE_CUTLEADER_CLASS(OpenLoopStartCutPt)

BEGIN_CUTLEADER_NAMESPACE()

// this class describe the start/end cut point which at the two ends of the open path.
class ClCutFeature_Export OpenLoopStartCutPt : public LoopStartCutPtBase
{
public:
	OpenLoopStartCutPt();
	OpenLoopStartCutPt(LONGLONG iStartPatID, const PatternPosition& patternPosition);
	virtual ~OpenLoopStartCutPt();

public: // implement IData interface
	IDataPtr Clone() const override;

public: // implement interface ILoopStartCutPt.
	virtual SECUT_PT_TYPE GetSECutPtType() const override { return SECUT_PT_OPATH; }
};

END_CUTLEADER_NAMESPACE()
