#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 调整“桥接工序”的起始轮廓。
class StartBridgeNodeChangeAction : public SequenceEditActionBase
{
public:
	StartBridgeNodeChangeAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~StartBridgeNodeChangeAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_BDGE_START_CHANGE; }
	virtual int GetActionName() const override { return IDS_SEQ_CHANGE_BDGE_START; }
	virtual void UpdateForUndoRedo() override;

private:
	// 当前的“回路实例”。
	LoopInstancePtr m_pActiveLoopInstance;

	// 缓存所有“桥接工序”中的“回路实例”。
	LoopInstanceListPtr m_pBdgeLoopInsts;
};

END_CUTLEADER_NAMESPACE()
