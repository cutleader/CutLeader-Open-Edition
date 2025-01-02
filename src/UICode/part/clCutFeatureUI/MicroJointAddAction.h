#pragma once

#include "CutFeatureEditBase.h"

DECLARE_CUTLEADER_CLASS(ParamConfig)

BEGIN_CUTLEADER_NAMESPACE()

// 给回路添加微连。
class ClCutFeatureUI_Export MicroJointAddAction : public CutFeatureEditBase
{
public:
	MicroJointAddAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort, ParamConfigPtr pParamConfig);
	~MicroJointAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return MicroJoint_ADD; }
	virtual int GetActionName() const override { return IDS_COMMAND_ADD_MicroJoint; }
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 当前被捕获的轮廓。
	IPatternLoopPtr m_pActivePatternLoop;

	ParamConfigPtr m_pParamConfig;
};

END_CUTLEADER_NAMESPACE()
