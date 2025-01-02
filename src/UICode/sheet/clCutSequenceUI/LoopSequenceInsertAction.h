#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 这个动作在选中的工序前面插入新的“回路工序”。
class LoopSequenceInsertAction : public SequenceEditActionBase
{
	typedef enum tagStartCutEditState
	{
		SEQUENCE_INSERT_NONE					= 0,
		SEQUENCE_INSERT_SELECT_BASE_SEQ			= 1,
		SEQUENCE_INSERT_SELECT_NEWLOOPINST		= 2,
	} SEQUENCE_INSERT_STATE;

public:
	LoopSequenceInsertAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~LoopSequenceInsertAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_INSERT_LOOPSEQ; }
	virtual int GetActionName() const override { return IDS_SEQ_INSERT_LOOP_SEQ; }

private:
	SEQUENCE_INSERT_STATE m_state;

	// 新回路工序插在这个工序前面。
	ICutSequencePtr m_pActiveCutSeq;

	// the loop instance which will be sequenced.
	LoopInstancePtr m_pActiveLoopInstance;

	// 缓存所有的“回路实例”
	LoopInstanceListPtr m_pLoopInstanceList;

	// 缓存所有添加了工序的“回路实例”。
	LoopInstanceListPtr m_pAllSequencedLoopInstances;
};

END_CUTLEADER_NAMESPACE()
