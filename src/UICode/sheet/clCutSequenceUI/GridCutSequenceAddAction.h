#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartPlacementList)

BEGIN_CUTLEADER_NAMESPACE()

// 添加“田字工序”的动作。
class GridCutSequenceAddAction : public SequenceEditActionBase
{
	typedef enum tagBdgeSeqAddState
	{
		NONE						= 0,
		GRID_SEQ_FIRST_POINT		= 1, // 左键按下
		GRID_SEQ_SECOND_POINT		= 2, // 左键抬起
	} GRIDSEQ_ADD_STATE;

public:
	GridCutSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	virtual ~GridCutSequenceAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_ADD_GRIDSEQ; }
	virtual int GetActionName() const override { return IDS_SEQ_ADD_GRIDSEQ; }
	virtual void UpdateForUndoRedo() override;

private:
	GRIDSEQ_ADD_STATE m_iState;

	// the data for selection.
	/*
		LeftTopPointX;
		LeftTopPointY;
		RightBottomPointX;
		RightBottomPointY;
	*/
	double m_data[4];

	// 鼠标移动时的候选partpmt，只有一个有效。
	PartPlacementPtr m_pActivePartPlacement; // 不框选时的“当前partpmt”。
	PartPlacementListPtr m_pRectSelectPartPlacements; // 框选时的“当前partpmt”。
};

END_CUTLEADER_NAMESPACE()
