#pragma once

#include "CutPartSequenceBase.h"

DECLARE_CUTLEADER_CLASS(LoopSequence)

BEGIN_CUTLEADER_NAMESPACE()

// Loop sequence, »ØÂ·¹¤Ðò¡£
class ClCutSequence_Export LoopSequence : public CutPartSequenceBase
{
public:
	LoopSequence(void);
	LoopSequence(LoopInstancePtr pLoopInstance);
	virtual ~LoopSequence(void);

public: // implement IData interface.
	virtual IDataPtr Clone() const override; // "m_pLoopInstance" should be cloned.

public: // implement interface ICutSequence.
	virtual CutSequenceType GetCutSequenceType() const override { return CutSequence_Loop; }
	virtual Point2D GetFirstCutPt() const override;
	virtual Point2D GetLastCutPt() const override;

public: // implement interface ICutPartSequence.
	virtual BOOL LoopHasBeenCut(const LoopInstance* pLoopInstance) const override;
	virtual LoopInstanceListPtr GetLoopInstanceList() const override;
	virtual LoopInstancePtr GetLoopHasLead() const override { return m_pLoopInstance; }
	virtual PartInstanceListPtr GetPartInstances() const override;

public: // get/set functions.
	LoopInstancePtr GetLoopInstance() const { return m_pLoopInstance; }
	void SetLoopInst(LoopInstancePtr pLoopInstance) { m_pLoopInstance = pLoopInstance; }

	BOOL LeadOutOff() const { return m_bLeadOutOff; }
	void LeadOutOff(BOOL bLeadOutOff) { m_bLeadOutOff = bLeadOutOff; }

protected:
	LoopInstancePtr m_pLoopInstance;

	// whether the lead out is disable for cut avoidance.
	BOOL m_bLeadOutOff;
};

END_CUTLEADER_NAMESPACE()
