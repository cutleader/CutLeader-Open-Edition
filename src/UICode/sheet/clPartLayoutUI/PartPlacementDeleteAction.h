#pragma once

#include "PartPlacementPreSelectAction.h"

DECLARE_CUTLEADER_CLASS(PartPlacementDeleteAction)

BEGIN_CUTLEADER_NAMESPACE()

class PartPlacementDeleteAction : public PartPlacementPreSelectAction
{
public:
	PartPlacementDeleteAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		PartGroupListPtr pPartGroups, PartPlacementListPtr pSelectedPartPlacements, GlViewPortPtr pViewPort);
	~PartPlacementDeleteAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:  // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_DELETEPARTPMT; }
	virtual int GetActionName() const override { return IDS_COMMAND_DEL_PARTPMT; }
	virtual void UpdateForUndoRedo() override;

public:
	// delete selected part pmts.
	void DeleteSelectedPartPlacements();
};

END_CUTLEADER_NAMESPACE()
