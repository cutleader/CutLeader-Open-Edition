#pragma once

#include "CanvasPatternPreSelectAction.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(AddPartFromCanvasAction)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(SelectPatternOrLoopActionBar)

BEGIN_CUTLEADER_NAMESPACE()

class AddPartFromCanvasAction : public CanvasPatternPreSelectAction
{
public:
	AddPartFromCanvasAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort);
	~AddPartFromCanvasAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return AddCanvasPattern2Task; }
	virtual int GetActionName() const override { return IDS_COMMAND_ADD_PARTPMT; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

public:
	// 发送消息给任务视图，把选中的几何特征定义为零件，存入数据库，并加到任务中。
	void AddSelectedPattern2PartLibrary();
};

END_CUTLEADER_NAMESPACE()
