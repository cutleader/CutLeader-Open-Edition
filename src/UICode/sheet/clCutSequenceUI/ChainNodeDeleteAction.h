#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(IChainNode)

BEGIN_CUTLEADER_NAMESPACE()

// 删除“连割节点”的动作。
class ChainNodeDeleteAction : public SequenceEditActionBase
{
public:
	ChainNodeDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~ChainNodeDeleteAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_DEL_CHAINNODE; }
	virtual int GetActionName() const override { return IDS_SEQ_DEL_CHAIN_NODE; }
	virtual void UpdateForUndoRedo() override;

private:
	// 距离光标最近的节点。
	IChainNodePtr m_pActiveChainNode;
};

END_CUTLEADER_NAMESPACE()
