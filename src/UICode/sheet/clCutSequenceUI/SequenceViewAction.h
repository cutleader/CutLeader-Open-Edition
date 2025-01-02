#pragma once

#include "clCutSequenceUICommon.h"
#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(CutNodeSimulator)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)

BEGIN_CUTLEADER_NAMESPACE()

// 工序模拟动作。
class ClCutSequenceUI_Export SequenceViewAction : public SequenceEditActionBase
{
public:
	SequenceViewAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~SequenceViewAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_VIEW; }
	virtual int GetActionName() const override { return IDS_SEQ_VIEWSEQUENCE; }

public: // other functions.
	void Start();

private:
	// get the message by the simulation speed.
	int GetMsgBySpeed(int iSpeed);

private:
	CutNodeSimulator* m_pCutNodeSimulator;

	// “工序模拟形状”集合。
	SequenceSimulateShapeListPtr m_pSequenceSimulateShapeList;
};

END_CUTLEADER_NAMESPACE()
