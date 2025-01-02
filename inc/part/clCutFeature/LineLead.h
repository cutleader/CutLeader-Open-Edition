#pragma once

#include "LeadBase.h"

DECLARE_CUTLEADER_CLASS(LineLead)

BEGIN_CUTLEADER_NAMESPACE()

// the "one line" lead.
class ClCutFeature_Export LineLead : public LeadBase
{
public:
	LineLead(void);
	LineLead(LeadInOutType emLeadInOut, double dLength, double dAngle);
	virtual ~LineLead(void);

public: // implement IData interface
	// notes: we should clone the condition node also.
	virtual IDataPtr Clone() const override;

public: // implement the ILead interface.
	virtual LeadType GetLeadType() const override { return Lead_Line; }
	virtual LineArc2DType GetFirst2DLineArc(double& dVal) const override;
	virtual void ScaleSize(double dFactor) override { m_dLength *= dFactor; }

public: // set/get functions
	void SetLength(double dLength) { m_dLength = dLength; }
	double GetLength() const { return m_dLength; }

	void SetAngle(double dAngle) { m_dAngle = dAngle; }
	double GetAngle() const { return m_dAngle; }

	void IsAtCorner(BOOL bAtCorner) { m_bAtCorner = bAtCorner; }
	BOOL IsAtCorner() const { return m_bAtCorner; }

private:
	// the length of the line lead.
	double m_dLength;

	// the angle of the line lead.
	double m_dAngle;

	// whether this lead is at corner.
	// notes:
	//   1) if YES, do not need "m_dAngle".
    //   2) 这个变量目前就是个标记，业务方面没有用它。
	BOOL m_bAtCorner;
};

END_CUTLEADER_NAMESPACE()
