#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(FlyCutSequenceAddAction)

BEGIN_CUTLEADER_NAMESPACE()

// 添加“飞行切割工序”的动作。
class FlyCutSequenceAddAction : public SequenceEditActionBase
{
	enum FLYCUTSEQ_ADD_STATE
	{
		NONE						= 0,
		FLYCUT_SEQ_FIRST_POINT		= 1, // 左键按下
		FLYCUT_SEQ_SECOND_POINT		= 2, // 左键抬起
	};

public:
	FlyCutSequenceAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~FlyCutSequenceAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_Add_FLYCUT; }
	virtual int GetActionName() const override { return IDS_SEQ_AddFlyCut; }
	virtual void UpdateForUndoRedo() override;

private:
	FLYCUTSEQ_ADD_STATE m_emState;

	// the data for selection.
	/*
		LeftTopPointX;
		LeftTopPointY;
		RightBottomPointX;
		RightBottomPointY;
	*/
	double m_data[4];

	// 鼠标移动时的候选loop inst，只有一个有效。
	LoopInstancePtr m_pActiveLoopInstance; // 不框选时的“当前loop inst”。
	LoopInstanceListPtr m_pRectSelectLoopInstances; // 框选时的“当前loop inst”。

	// 缓存所有的“回路实例”。
	LoopInstanceListPtr m_pAllLoopInstances;
};

END_CUTLEADER_NAMESPACE()
