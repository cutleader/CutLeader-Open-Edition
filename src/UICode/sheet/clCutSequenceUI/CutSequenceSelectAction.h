#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(ICutPartSequence)
DECLARE_CUTLEADER_CLASS(CutSequenceSelectAction)
DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)

BEGIN_CUTLEADER_NAMESPACE()

// 工序选择动作。
// 注：
//  1) 通过选择“回路实例”来选择工序。
//  2) 目前支持按住ctrl多选。
class ClCutSequenceUI_Export CutSequenceSelectAction : public SequenceEditActionBase
{
public:
	CutSequenceSelectAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~CutSequenceSelectAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_SELECT; }
	virtual int GetActionName() const override { return IDS_SEQ_SELECTSEQUENCE; }
	virtual void UpdateForUndoRedo() override;

public: // get/set functions.
	// get sequence units those are selected.
	CutSequenceListPtr GetSelSeqUnits() const { return m_pSelSeqUnitList; }

public: // other functions.
	void SelectedSeqUnit(CutSequenceListPtr pSelSeqUnitList) { m_pSelSeqUnitList = pSelSeqUnitList; }

private:
	// 当前的工序。
	ICutPartSequencePtr m_pCurCutSeq;

	// 选中的工序。
	CutSequenceListPtr m_pSelSeqUnitList;

	// 缓存所有添加了工序的“回路实例”。
	LoopInstanceListPtr m_pAllSequencedLoopInstances;

	// 缓存工序模拟信息
	SequenceSimulateShapeListPtr m_pSequenceSimulateShapeList;
};

END_CUTLEADER_NAMESPACE()
