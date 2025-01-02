#pragma once

#include "clExpertLibUICommon.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(LeadInOutData)

BEGIN_CUTLEADER_NAMESPACE()

// draw Lead in/out in the static control.
class LeadDrawStatic : public CStatic
{
	DECLARE_DYNAMIC(LeadDrawStatic)

public:
	LeadDrawStatic(void);
	~LeadDrawStatic(void);

public:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

public:
	void InitDrawer();

	// init the lead data.
	void InitLead(LeadInOutDataPtr pLeadIn, LeadInOutDataPtr pLeadOut);
	void UpdateLeadIn(LeadInOutDataPtr pLeadIn) { m_pLeadIn = pLeadIn; }
	void UpdateLeadOut(LeadInOutDataPtr pLeadOut) { m_pLeadOut = pLeadOut; }

private:
	void DrawLeadIn();
	void DrawLeadOut();

private:
	GlViewPortPtr m_pViewPort;
	double m_dMaxSize;

	LeadInOutDataPtr m_pLeadIn;
	LeadInOutDataPtr m_pLeadOut;
};

END_CUTLEADER_NAMESPACE()
