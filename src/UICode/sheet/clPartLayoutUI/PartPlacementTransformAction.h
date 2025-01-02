#pragma once

#include "IPartPlacementTransformAction.h"
#include "PartPlacementPreSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

class PartPlacementTransformAction : public IPartPlacementTransformAction, public PartPlacementPreSelectAction
{
public:
	PartPlacementTransformAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacementList, PartInstanceListPtr pPartInstanceList,
        PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort);
	virtual ~PartPlacementTransformAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual void UpdateForUndoRedo();

protected:
	TRANSFORM_PARTPMT_STATE m_actionState;

	// use these data to decide the transform matrix.
	double m_dBasePtX, m_dBasePtY;
	double m_dFirstPtX, m_dFirstPtY;
	double m_dSecondPtX, m_dSecondPtY;

	Matrix2D m_modifyMatrix;

	// a new copy of selected patterns.
	// notes:
	//   1) only used to draw the temporary part pmts.
	//   2) when rotating part pmts, we should update draw data of "m_pNewPartPlacements", we do this in "MovePoint()" NOT in "PartPlacementTransformCommand".
	PartPlacementListPtr m_pNewPartPlacements;
};

END_CUTLEADER_NAMESPACE()
