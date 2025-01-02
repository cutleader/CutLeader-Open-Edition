#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(BridgeNode)

BEGIN_CUTLEADER_NAMESPACE()

// 断开桥的动作。
class BridgeSequenceBreakAction : public SequenceEditActionBase
{
public:
	BridgeSequenceBreakAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~BridgeSequenceBreakAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_BREAK_BDGESEQ; }
	virtual int GetActionName() const override { return IDS_SEQ_BREAK_BDGE_SEQ; }
	virtual void UpdateForUndoRedo() override;

private:
	// 计算距离光标最近的“入口桥”基本图形及对应的“桥接节点”。
	void CalcCurBdgeGeomInfo(const Point2D& pt);

private:
	// 距离光标最近的“入口桥”基本图形及对应的“桥接节点”。
	LineArc2DListPtr m_pCurBdgeGeom;
	BridgeNodePtr m_pCurBdgeNode;
};

END_CUTLEADER_NAMESPACE()
