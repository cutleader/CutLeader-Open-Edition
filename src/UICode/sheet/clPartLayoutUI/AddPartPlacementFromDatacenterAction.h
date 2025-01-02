#pragma once

#include "Matrix2D.h"
#include "PartPlacementEditAction.h"
#include "PartPlacementMoveBase.h"

DECLARE_CUTLEADER_CLASS(AddPartPlacementFromDatacenterAction)

BEGIN_CUTLEADER_NAMESPACE()

// 从数据库添加一个零件到板材。
// 注：
//  1) 可以连续添加。
class AddPartPlacementFromDatacenterAction : public PartPlacementEditAction, public PartPlacementMoveBase
{
public:
	AddPartPlacementFromDatacenterAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		GlViewPortPtr pViewPort, LONGLONG iPartID, LONGLONG iShtParamConfigID, PartDistanceDetectActionBar* pPartDistanceDetectActionBar);
	~AddPartPlacementFromDatacenterAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_ADDPARTPMT; }
	virtual int GetActionName() const override { return IDS_COMMAND_ADD_PARTPMT; }
	virtual void UpdateForUndoRedo() override;
};

END_CUTLEADER_NAMESPACE()
