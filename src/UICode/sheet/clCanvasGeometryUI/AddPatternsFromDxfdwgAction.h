#pragma once

#include "CanvasGeometryEditAction.h"
#include "LoopPositionDetector.h"

DECLARE_CUTLEADER_CLASS(AddPatternsFromDxfdwgAction)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(SelectPatternOrLoopActionBar)

BEGIN_CUTLEADER_NAMESPACE()

class AddPatternsFromDxfdwgAction : public CanvasGeometryEditAction, public LoopPositionDetector
{
public:
	AddPatternsFromDxfdwgAction(PartPlacementPtr pPartPlacement, PatternListPtr pNewPatterns, PatternLoopListPtr pNewPatternLoops, TextStructListPtr pNewTextStructs,
		LoopTopologyItemListPtr pNewLoopTopologyItems, const Point2D& referencePt, SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar,
		LoopPositionDetectActionBar* pLoopPositionDetectActionBar, GlViewPortPtr pViewPort, const ToolAssignConfigItemPtr pToolAssignConfigItem);
	~AddPatternsFromDxfdwgAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return AddPatternFromDxf; }
	virtual int GetActionName() const override { return IDS_COMMAND_ADD_PAT; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

protected:
	// 从dxf加载的几何轮廓、文字数据。
	// 注：
	// 1) 在确定了放置位置后才更新这些数据，然后加到底图上面。
	PatternListPtr m_pNewPatterns;
	PatternLoopListPtr m_pNewPatternLoops;
	TextStructListPtr m_pNewTextStructs;
	LoopTopologyItemListPtr m_pNewLoopTopologyItems;

	const ToolAssignConfigItemPtr m_pToolAssignConfigItem;
	SelectPatternOrLoopActionBar* m_pSelectPatternOrLoopActionBar;
};

END_CUTLEADER_NAMESPACE()
