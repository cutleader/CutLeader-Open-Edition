#pragma once

#include "clPartLayoutUICommon.h"
#include "PartPlacementTransformAction.h"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(PartPlacementRotateAction)

BEGIN_CUTLEADER_NAMESPACE()

class PartPlacementRotateAction : public PartPlacementTransformAction
{
public:
	PartPlacementRotateAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		PartPlacementListPtr pSelectedPartPlacements, GlViewPortPtr pViewPort);
	~PartPlacementRotateAction(void);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_ROTATEPARTPMT; }
	virtual int GetActionName() const override { return IDS_COMMAND_ROTATE_PARTPMT; }
	virtual void ConvertKBInput(double& dVal1, double& dVal2) override;

private: // implement IPartPlacementTransformAction interface.
	virtual void SetMatrixValue();
	virtual CString GetActionName();
	virtual int GetMsgID();
};

END_CUTLEADER_NAMESPACE()
