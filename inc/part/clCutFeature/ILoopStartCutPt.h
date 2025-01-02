#pragma once

#include "clCutFeatureCommon.h"
#include "IData.h"
#include "PatternPosition.h"

BEGIN_CUTLEADER_NAMESPACE()

// the type of the start/end cut point of the loop.
typedef enum tagSECutPtType
{
	// the start/end cut point are at the geom.
	SECUT_PT_GEOM			= 0,

	// the start/end cut point are at the two ends of the open path.
	SECUT_PT_OPATH			= 1,

	// the start/end cut point are at the corner of the close path.
	SECUT_PT_CORNER			= 2,
} SECUT_PT_TYPE;

// 回路的切割开始和结束点。
class ClCutFeature_Export ILoopStartCutPt : virtual public IData
{
public:
    virtual ~ILoopStartCutPt() {}

public:
	virtual SECUT_PT_TYPE GetSECutPtType() const = 0;

	virtual void SetStartPatID(LONGLONG iStartPatID) = 0;
	virtual LONGLONG GetStartPatID() const = 0;

	virtual void SetPatternPosition(const PatternPosition& patternPosition) = 0;
	virtual PatternPosition GetPatternPosition() const = 0;
};

END_CUTLEADER_NAMESPACE()
