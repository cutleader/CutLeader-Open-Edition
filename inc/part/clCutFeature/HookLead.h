#pragma once

#include "LeadBase.h"

DECLARE_CUTLEADER_CLASS(HookLead)

BEGIN_CUTLEADER_NAMESPACE()

// the "hook" lead.
// notes:
//   1) this class is only used for lead in.
class ClCutFeature_Export HookLead : public LeadBase
{
public:
	HookLead(void);
	HookLead(LeadInOutType emLeadInOut, double dLength, double dRadius);
	virtual ~HookLead(void);

public: // implement IData interface
	// notes: we should clone the condition node also.
	virtual IDataPtr Clone() const override;

public: // implement the ILead interface.
	virtual LeadType GetLeadType() const override { return Lead_Hook; }
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const override;
	virtual void ScaleSize(double dFactor) override;

public: // get/set functions.
	void SetLength(double dLength) { m_dLength = dLength; }
	double GetLength() const { return m_dLength; }

	void SetRadius(double dRadius) { m_dRadius = dRadius; }
	double GetRadius() const { return m_dRadius; }

private:
	double m_dLength;
	double m_dRadius;
};

END_CUTLEADER_NAMESPACE()
