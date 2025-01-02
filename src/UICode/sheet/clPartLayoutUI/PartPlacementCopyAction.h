#pragma once

#include "PartPlacementPreSelectAction.h"
#include "Rect2D.h"
#include "PartPlacementMoveBase.h"

DECLARE_CUTLEADER_CLASS(PartPlacementCopyAction)

BEGIN_CUTLEADER_NAMESPACE()

// 拷贝“零件放置”的动作。
// 注：
// 1) 当预选中零件启动该动作时，需要指定移动参考点referencePt，通常是零件中心点，零件移动过程中不改变这个值。如果未选中零件启动该动作，则无需设置该点。
class PartPlacementCopyAction : public PartPlacementPreSelectAction, public PartPlacementMoveBase
{
public:
	PartPlacementCopyAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		PartPlacementListPtr pSelectedPartPlacements, const Point2D& referencePt, GlViewPortPtr pViewPort, PartDistanceDetectActionBar* pPartDistanceDetectActionBar);
	~PartPlacementCopyAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_COPYPRTPMT; }
	virtual int GetActionName() const override { return IDS_COMMAND_COPY_PARTPMT; }

private:
	// 移动参考点。
	Point2D m_referencePt;
};

END_CUTLEADER_NAMESPACE()
