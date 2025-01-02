#pragma once

#include "LeadBase.h"

DECLARE_CUTLEADER_CLASS(NoneLead)

BEGIN_CUTLEADER_NAMESPACE()

// the "none" lead. actually this class is a dummy class.
class ClCutFeature_Export NoneLead : public LeadBase
{
public:
	NoneLead(LeadInOutType emLeadInOut);
	virtual ~NoneLead(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public:
	virtual LeadType GetLeadType() const override { return Lead_None; }
};

END_CUTLEADER_NAMESPACE()
