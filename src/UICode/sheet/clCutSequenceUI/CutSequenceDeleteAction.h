#pragma once

#include "CutSequencePreSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// 删除工序的动作。
class CutSequenceDeleteAction : public CutSequencePreSelectAction
{
	typedef enum tagStartCutEditState
	{
		SEQUENCE_DELETE_NONE				= 0,
		SEQUENCE_DELETE_SELECT_LOOPINST		= 1,
	} SEQUENCE_DELETE_STATE;

public:
	CutSequenceDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData,
		GlViewPortPtr pViewPort, ApplyPartActionBar* pApplyPartActionBar);
	~CutSequenceDeleteAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_REMOVE; }
	virtual int GetActionName() const override { return IDS_SEQ_DELETE_SEQ; }
	virtual void UpdateForUndoRedo() override;

public:
	// 选中这些工序。
	// 注: 
	//   1) 这个函数类似鼠标选中的操作。
	void SelectSeqUnits(CutSequenceListPtr pCutSequences);

private:
	SEQUENCE_DELETE_STATE m_state;
};

END_CUTLEADER_NAMESPACE()
