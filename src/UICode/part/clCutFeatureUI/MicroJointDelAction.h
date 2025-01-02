#pragma once

#include "Point2D.h"
#include "MicroJointPreSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// “删除微连接”动作。
class ClCutFeatureUI_Export MicroJointDelAction : public MicroJointPreSelectAction
{
public:
	MicroJointDelAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort);
	~MicroJointDelAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return MicroJoint_DELETE; }
	virtual int GetActionName() const override { return IDS_COMMAMD_DEL_MicroJoint; }

public:
	// 选中这些微连接特征。
	// 注: 
	//   1) 这个函数类似鼠标选中的操作。
	void SelectMJList(MicroJointListPtr pMicroJointList);
};

END_CUTLEADER_NAMESPACE()
