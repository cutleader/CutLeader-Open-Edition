#pragma once

#include "clCutFeatureCommon.h"
#include "Point2D.h"
#include "CutFeatureEditBase.h"

DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// 调整几何轮廓的切割方向。
class ClCutFeatureUI_Export CutDirectionChangeAction : public CutFeatureEditBase
{
	enum CutDirectionChangeState
	{
		CutDirectionChange_None = 0,
		CutDirectionChange_FirstPt = 1, // 左键按下
		CutDirectionChange_SecondPt = 2, // 左键抬起
	};

public:
	CutDirectionChangeAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
		LoopCutFeatureListPtr pLoopFeatureList, CutDirectionChangeType emCutDirectionChangeType, GlViewPortPtr pViewPort);
	~CutDirectionChangeAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY) override;
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return DIRECTION_CHANGE; }
	virtual int GetActionName() const override { return IDS_CHANGE_CUTDIR; }

private:
	CutDirectionChangeState m_emState;

	// 框选的框框(左上x，左上y，右下x，右下y)
	double m_boxData[4];

	// 鼠标移动时的候选轮廓，只有一个有效。
	IPatternLoopPtr m_pActivePatternLoop; // 不框选时的“当前轮廓”。
	PatternLoopListPtr m_pActivePatternLoops_inRect; // 框选时的“当前轮廓”。

	CutDirectionChangeType m_emCutDirectionChangeType;
};

END_CUTLEADER_NAMESPACE()
