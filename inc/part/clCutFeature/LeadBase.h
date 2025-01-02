#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"
#include "ILead.h"

BEGIN_CUTLEADER_NAMESPACE()

// the base class for all kinds of lead in/out.
class ClCutFeature_Export LeadBase : public ILead, public ClData
{
public:
	LeadBase(void);
	virtual ~LeadBase(void);

public: // implement the ILead interface.
	virtual void SetLeadInOutType(LeadInOutType emLeadInOut) override { m_iLeadInOutType = emLeadInOut; }
	virtual LeadInOutType GetLeadInOutType() const override { return m_iLeadInOutType; }
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const override { return LineArc2D_Line; }
	virtual void ScaleSize(double dFactor) override {}

protected:
	LeadInOutType m_iLeadInOutType;
};

END_CUTLEADER_NAMESPACE()
