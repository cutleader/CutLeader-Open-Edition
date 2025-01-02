#pragma once

#include "PartPlacementEditAction.h"
#include "Point2D.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LineArc2DList)

BEGIN_CUTLEADER_NAMESPACE()

// “测量两点距离”的动作。
class DistanceMeasureAction : public PartPlacementEditAction
{
	typedef enum tagMeasureDisState
	{
		NONE					= 0,
		FIRST_LOOP_SELECTED		= 1,
		FIRST_PT_SELECTED		= 2,
		SECOND_LOOP_SELECTED	= 3,
		SECOND_PT_SELECTED		= 4,
	} MEASURE_DIS_STATE;

public:
	DistanceMeasureAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	virtual ~DistanceMeasureAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return MEASURE_PT_DISTANCE; }
	virtual int GetActionName() const override { return IDS_MEASURE_DISTANCE; }
	virtual SnapInputPtr GetSnapInput() const override;

private:
	// 计算出最近的回路。
	// 注：
	//  1) 板材外边框也算“回路”。
	LineArc2DListPtr CalcClosestLoop(const Point2D& pt);

private:
	MEASURE_DIS_STATE m_iState;

	// 距离光标最近的回路。这个回路如果是轮廓实例的话，m_pCloseLoopInstance就是有效的。
	LineArc2DListPtr m_pCloseLoop;
	LoopInstancePtr m_pCloseLoopInstance;
	PatternListPtr m_pPatternsInLoopInstance;

	// 两个点。
	Point2D m_fstPt;
	Point2D m_sndPt;

	// 缓存所有的外围“回路实例”。
	LoopInstanceListPtr m_pAllLoopInstances;

	// 缓存板材边框的基本图形。
	LineArc2DListPtr m_pSheetBorderGeoms;
};

END_CUTLEADER_NAMESPACE()
