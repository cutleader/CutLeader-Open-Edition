#pragma once

#include "CanvasPatternPreSelectAction.h"
#include "Matrix2D.h"
#include "LoopPositionDetector.h"

DECLARE_CUTLEADER_CLASS(CanvasPatternMoveAction)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 移动底图上图形的action。
// 注：
// 1) 目前只能选择几何轮廓进行移动。
// 2) 当预选中图形启动该动作时，需要指定移动参考点referencePt，通常是图形中心点，可以想象成钉住这个位置移动，这个变量需要及时更新。如果未选中图形启动该动作，则无需设置该点。
class CanvasPatternMoveAction : public CanvasPatternPreSelectAction, public LoopPositionDetector
{
public:
	CanvasPatternMoveAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs, const Point2D& referencePt,
		SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternMoveAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return MovePatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_COMMAND_MoveCanvasLoop; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;
};

END_CUTLEADER_NAMESPACE()
