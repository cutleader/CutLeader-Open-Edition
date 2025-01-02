#pragma once

#include "PartPlacementEditAction.h"
#include "Matrix2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// 炸散零件阵列的动作。
class PartPlacementExplodeAction : public PartPlacementEditAction
{
public:
	PartPlacementExplodeAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	~PartPlacementExplodeAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_EXPLODE_GRID; }
	virtual int GetActionName() const override { return IDS_COMMAND_EXPLODE_GRID; }

public:
    // 调用者确保传入的零件是阵列零件。
    void ExplodeGridPartPlacement(PartPlacementPtr pPartPlacement);

private:
	// the part placement which will be exploded.
	PartPlacementPtr m_pSelectedPartPlacement;
};

END_CUTLEADER_NAMESPACE()
