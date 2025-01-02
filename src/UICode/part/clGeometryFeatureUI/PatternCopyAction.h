#pragma once

#include "clGeometryFeatureUICommon.h"
#include "Matrix2D.h"
#include "PatternPreSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// “拷贝图形”动作。
class PatternCopyAction : public PatternPreSelectAction
{
	typedef enum tagModifyState
	{
		COPY_PATTERN_NOTSTART		= 0,
		COPY_PATTERN_BASE_POINT		= 1,
		COPY_PATTERN_DEST_POINT		= 2,
	} COPY_PATTERN_STATE;

public:
	PatternCopyAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelPats, TextStructListPtr pSelectedTextStructs);
	~PatternCopyAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY) override;
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY) override;
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) override;

public:  // implement IAction interface.
	virtual int GetActionType() const override { return EDIT_COPY; }
	virtual int GetActionName() const override { return IDS_COMMAND_COPY_PAT; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const;

private:
	void SetMatrixValue();

private:
	COPY_PATTERN_STATE m_actionState;

	// use these data to decide the transform matrix.
	double m_dBasePtX, m_dBasePtY;
	double m_dFirstPtX, m_dFirstPtY;
	double m_dSecondPtX, m_dSecondPtY;

	// 从原位置到拷贝位置的变换矩阵。
	Matrix2D m_modifyPatMat;

	// 拷贝出来的几何特征，只用于显示, 在鼠标移动时更新。
	PatternListPtr m_pTmpPatterns;
};

END_CUTLEADER_NAMESPACE()
