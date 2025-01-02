#pragma once

#include "LeadBase.h"

DECLARE_CUTLEADER_CLASS(ArcLead)

BEGIN_CUTLEADER_NAMESPACE()

// the "arc" type lead.
// notes:
//   1) when start cut point is at geom, this lead type only can be used for lead out.
class ClCutFeature_Export ArcLead : public LeadBase
{
public:
	ArcLead(void);
	ArcLead(LeadInOutType emLeadInOut, double dRadius, double dMoveAngle);
	virtual ~ArcLead(void);

public: // implement IData interface
	// notes: we should clone the condition node also.
	virtual IDataPtr Clone() const override;

public: // implement the ILead interface.
	virtual LeadType GetLeadType() const override { return Lead_Arc; }
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const override;
	virtual void ScaleSize(double dFactor) override;

public: // get/set functions.
	void SetRadius(double dRadius) { m_dRadius = dRadius; }
	double GetRadius() const { return m_dRadius; }

	void SetSweepAngle(double dAngle) { m_dMoveAngle = dAngle; }
	double GetSweepAngle() const { return m_dMoveAngle; }

	void IsAtCorner(BOOL bAtCorner) { m_bAtCorner = bAtCorner; }
	BOOL IsAtCorner() const { return m_bAtCorner; }

private:
	double m_dRadius;
	double m_dMoveAngle;

	// whether this lead is at corner.
	// notes:
	//   1) if YES, do not need "m_dRadius".
    //   2) 这个变量目前就是个标记，业务方面没有用它。
	BOOL m_bAtCorner;
};

END_CUTLEADER_NAMESPACE()
