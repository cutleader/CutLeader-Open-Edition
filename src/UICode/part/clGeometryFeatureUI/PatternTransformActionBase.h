#pragma once

#include "Matrix2D.h"
#include "IPatternTransformAction.h"
#include "PatternPreSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// The base class for actions that will transform the pattern, e.g., move/rotate/flip the pattern.
class PatternTransformActionBase : public IPatternTransformAction, public PatternPreSelectAction
{
public:
	PatternTransformActionBase(void);
	~PatternTransformActionBase(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public: // implement IAction interface.
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

protected:
	TRANSFORM_PATTERN_STATE m_actionState;

	// use these data to decide the transform matrix.
	double m_dBasePtX, m_dBasePtY;
	double m_dFirstPtX, m_dFirstPtY;
	double m_dSecondPtX, m_dSecondPtY;

	// 变换矩阵。
	Matrix2D m_modifyPatMat;

	// 克隆了一份几何特征，仅仅用于显示。
	PatternListPtr m_pTmpPatterns;
};

END_CUTLEADER_NAMESPACE()
