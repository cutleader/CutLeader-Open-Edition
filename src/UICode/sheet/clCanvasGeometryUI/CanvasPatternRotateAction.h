#pragma once

#include "CanvasPatternPreSelectAction.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(CanvasPatternRotateAction)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 旋转底图上图形的action。
// 注：
// 1) 目前只能选择几何轮廓进行旋转。
class CanvasPatternRotateAction : public CanvasPatternPreSelectAction
{
	enum RotatePatternState
	{
		RotatePattern_NOTSTART				= 0,
		RotatePattern_BASE_PT_SELECTED		= 1,
		RotatePattern_SECOND_PT_SELECTED	= 2,
	};

public:
	CanvasPatternRotateAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
		SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternRotateAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return RotatePatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_COMMAND_RotateCanvasLoop; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

private:
	void CalcMatrixValue();

protected:
	RotatePatternState m_actionState;

	// use these data to decide the transform matrix.
	double m_dBasePtX, m_dBasePtY;
	double m_dFirstPtX, m_dFirstPtY;
	double m_dSecondPtX, m_dSecondPtY;

	Matrix2D m_modifyMatrix;

	// 拷贝出来的几何特征，只用于显示，在鼠标移动时更新。
	PatternListPtr m_pTmpPatterns;
};

END_CUTLEADER_NAMESPACE()
