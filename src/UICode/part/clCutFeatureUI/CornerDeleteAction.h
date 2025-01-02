#pragma once

#include "CornerPreSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// “删除角特征”动作。
class ClCutFeatureUI_Export CornerDeleteAction : public CornerPreSelectAction
{
public:
	CornerDeleteAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort);
	~CornerDeleteAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const  { return CORNER_DELETE; }
	virtual int GetActionName() const override { return IDS_COMMAMD_DEL_CORNER; }

public:
	// 选中这些角特征。
	// 注: 
	//   1) 这个函数类似鼠标选中的操作。
	void SelectCornerList(CornerListPtr pCornerList);
};

END_CUTLEADER_NAMESPACE()
