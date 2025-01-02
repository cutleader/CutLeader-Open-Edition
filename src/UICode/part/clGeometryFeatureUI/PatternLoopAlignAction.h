#pragma once

#include "PatternPreSelectAction.h"
#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(PatternLoopAlignAction)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// this action can align the loops.
class PatternLoopAlignAction : public PatternPreSelectAction
{
public:
	PatternLoopAlignAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelectedPatterns,
		TextStructListPtr pSelectedTextStructs, AlignLoopStyle emAlignLoopStyle);
	~PatternLoopAlignAction(void);

public: // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);

	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public:  // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return AlignPatternLoop; }
	virtual int GetActionName() const override { return IDS_COMMAND_AlignLoop; }
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

public:
	// 对齐选中的轮廓，该函数只执行数据层面的修改。
	// 注：
	// 1) 调用该函数前需确认选择了几何特征。
	bool AlignLoop();

private:
	AlignLoopStyle m_emAlignLoopStyle;
};

END_CUTLEADER_NAMESPACE()
