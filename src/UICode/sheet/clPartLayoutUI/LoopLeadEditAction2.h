#pragma once

#include "ObserverBase.h"
#include "StartCutPtPreSelectAction2.h"
#include "SpreadSamePartActionBar.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(LoopInstance)

BEGIN_CUTLEADER_NAMESPACE()

// 板材模式下编辑“回路实例”的引入引出线和起切点。
class LoopLeadEditAction2 : public StartCutPtPreSelectAction2, public ObserverBase
{
public:
	LoopLeadEditAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, 
		LeadConfigItemPtr pLeadConfigItem, ConditionTypeListPtr pCondTypeList, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	~LoopLeadEditAction2(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return LEAD_ADJUST; }
	virtual int GetActionName() const override { return IDS_EDIT_LOOP_LEAD; }
	virtual void UpdateForUndoRedo() override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

public:
    void EditLeadsOfSelectedLoopInstances(LoopInstanceListPtr pSelectedLoopInstances);

private:
	void DisplayLoopLead();

private:
	// 引入引出线分配规则。
	LeadConfigItemPtr m_pLeadConfigItem;

	// 所有条件类型。
	ConditionTypeListPtr m_pCondTypeList;

	// “传播相同零件”对话框。
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()
