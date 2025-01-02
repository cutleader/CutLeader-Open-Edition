#pragma once

#include "SequenceEditActionBase.h"
#include "ApplyPartActionBar.h"

DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 一些动作的基类，这些动作需要选择工序。
// 注：
//  1) 通过选择“回路实例”来选择工序。
//  2) 当“以零件为单位”进行选择时，得到当前工序涉及到的零件上面的所有工序。如果当前工序所在的零件被加了桥，那么桥工序涉及到的所有零件都会被选到。
class ClCutSequenceUI_Export CutSequencePreSelectAction : public SequenceEditActionBase
{
protected:
    // 这个类的派生类可以用这个状态值控制是否启用该类进行工序选择。
	enum PreSelectSequenceActionState
	{
		// the pre-select mode is inactive, mean every input cannot go into this action.
		PreSelectSequenceAction_inactive			= -1,

		// we can begin to select.
		PreSelectSequenceAction_active			    = 0,
	};

public:
	CutSequencePreSelectAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort, ApplyPartActionBar* pApplyPartActionBar);
	~CutSequencePreSelectAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_SELECT; }
	virtual int GetActionName() const override { return IDS_SEQ_SELECTSEQUENCE; }

public: // other functions.
	void SelectedSeqUnit(CutSequenceListPtr pSelCutSeqs) { m_pSelectCutSequences = pSelCutSeqs; }

protected:
	PreSelectSequenceActionState m_iPreSelectSequenceActionState;

	// 当前的工序。
	CutSequenceListPtr m_pCurrentCutSequences;

	// 选中的工序。
	CutSequenceListPtr m_pSelectCutSequences;

	// 缓存所有添加了工序的“回路实例”。
	LoopInstanceListPtr m_pAllSequencedLoopInstances;

	// “以零件为单位”action bar。
	ApplyPartActionBar* m_pApplyPartActionBar;
};

END_CUTLEADER_NAMESPACE()
