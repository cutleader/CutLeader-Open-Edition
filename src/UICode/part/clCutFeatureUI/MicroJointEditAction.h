#pragma once

#include "Point2D.h"
#include "MicroJointPreSelectAction.h"

DECLARE_CUTLEADER_CLASS(ConditionTypeList)

BEGIN_CUTLEADER_NAMESPACE()

// “编辑微连接”动作。
class ClCutFeatureUI_Export MicroJointEditAction : public MicroJointPreSelectAction
{
public:
	MicroJointEditAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort);
	~MicroJointEditAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return MicroJoint_EDIT; }
	virtual int GetActionName() const override { return IDS_EditMicroJoint; }
	virtual void UpdateForUndoRedo() override;

private:
	// 新的微连接位置。
	Point2D m_newPt;
	int m_iPatIndex;
};

END_CUTLEADER_NAMESPACE()
