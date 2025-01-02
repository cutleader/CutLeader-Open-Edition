#pragma once

#include "LeadBase.h"

DECLARE_CUTLEADER_CLASS(TwoLineLead)

BEGIN_CUTLEADER_NAMESPACE()

// the "two line" lead.
// notes:
//   1) this class only use for lead in.
class ClCutFeature_Export TwoLineLead : public LeadBase
{
public:
	TwoLineLead(void);
	TwoLineLead(LeadInOutType emLeadInOut, double dLength, double dAngle, double dSplitPercent);
	virtual ~TwoLineLead(void);

public: // implement IData interface
	// notes: we should clone the condition node also.
	virtual IDataPtr Clone() const override;

public: // implement the ILead interface.
	virtual LeadType GetLeadType() const override { return Lead_TwoLines; }
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const override;
	virtual void ScaleSize(double dFactor) override { m_dLength *= dFactor; }

public: // get/set functions.
	void SetLength(double dLength) { m_dLength = dLength; }
	double GetLength() const { return m_dLength; }

	void SetAngle(double dAngle) { m_dAngle = dAngle; }
	double GetAngle() const { return m_dAngle; }

	void SetSplit(double dSplit) { m_dSplitPercent = dSplit; }
	double GetSplit() const { return m_dSplitPercent; }

private:
	// 刺穿点到起切点的直线距离
	double m_dLength;

	// the angle of the line which is closer to the start point.
	double m_dAngle;

	double m_dSplitPercent;
};

END_CUTLEADER_NAMESPACE()
