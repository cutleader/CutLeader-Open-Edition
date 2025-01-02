#pragma once

#include "CanvasPatternPreSelectAction.h"
#include "Matrix2D.h"
#include "LoopPositionDetector.h"

DECLARE_CUTLEADER_CLASS(CanvasPatternCopyAction)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(SelectPatternOrLoopActionBar)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 拷贝底图上图形的action。
// 注：
// 1) 该action是拷贝pSelectedPatterns所在的几何轮廓，也就是pAffectedPatternLoops。如果选择了某轮廓的一部分节点，pAffectedPatternLoops中也会有该轮廓。
//    pLoopTopologyItems是要拷贝的轮廓的拓扑结构，有时调用方已经计算好了该数据。
// 2) 当预选中图形启动该动作时，需要指定移动参考点referencePt，通常是图形中心点，可以想象成钉住这个位置移动，这个变量需要及时更新。如果未选中图形启动该动作，则无需设置该点。
class CanvasPatternCopyAction : public CanvasPatternPreSelectAction, public LoopPositionDetector
{
public:
	CanvasPatternCopyAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
		PatternLoopListPtr pAffectedPatternLoops, LoopTopologyItemListPtr pLoopTopologyItems, const Point2D& referencePt,
		SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternCopyAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return CopyPatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_COMMAND_CopyCanvasLoop; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;
};

END_CUTLEADER_NAMESPACE()
