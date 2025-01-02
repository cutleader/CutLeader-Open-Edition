#pragma once

#include "LeadBase.h"

DECLARE_CUTLEADER_CLASS(LineArcLead)

BEGIN_CUTLEADER_NAMESPACE()

// the "line arc" type lead.
// notes: 
//   1) this class can be used for both lead in/out.
class ClCutFeature_Export LineArcLead : public LeadBase
{
public:
	LineArcLead(void);
	LineArcLead(LeadInOutType emLeadInOut, double dLength, double dArcAngle);
	virtual ~LineArcLead(void);

public: // implement IData interface
	// notes: we should clone the condition node also.
	virtual IDataPtr Clone() const override;

public: // implement the ILead interface.
	virtual LeadType GetLeadType() const override { return Lead_LineArc; }
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const override;
	virtual void ScaleSize(double dFactor) override { m_dLength *= dFactor; }

public: // get/set functions.
	void SetLength(double dLength) { m_dLength = dLength; }
	double GetLength() const { return m_dLength; }

	void SetArcAngle(double dAngle) { m_dArcAngle = dAngle; }
	double GetArcAngle() const { return m_dArcAngle; }

private:
	double m_dLength; // 刺穿点到起切点的直线距离
	double m_dArcAngle;
};

END_CUTLEADER_NAMESPACE()
