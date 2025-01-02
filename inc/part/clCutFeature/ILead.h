#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "IData.h"
#include "LineArc2DBase.h"

BEGIN_CUTLEADER_NAMESPACE()

// “引入引出线”接口。
// 注:
//   1) we keep the conditions info of the lead in/out.
class ClCutFeature_Export ILead : virtual public IData
{
public:
	virtual ~ILead() {}

public:
	// the type of the lead(lead in or lead out)
	virtual void SetLeadInOutType(LeadInOutType emLeadInOut) = 0;
	virtual LeadInOutType GetLeadInOutType() const = 0;
	virtual LeadType GetLeadType() const = 0;

	// get the info of the first 2d line/arc of the lead, "dVal" will be the length or radius.
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const = 0;

	// scale the size of the lead in/out.
	virtual void ScaleSize(double dFactor) = 0;
};

END_CUTLEADER_NAMESPACE()
