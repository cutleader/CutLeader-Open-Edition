#pragma once

#include "Matrix2D.h"
#include "PartPlacementEditAction.h"
#include "PartPlacementMoveBase.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(AddPartPlacementFromDxfdwgAction)

BEGIN_CUTLEADER_NAMESPACE()

// 从dxf添加一个或多个零件到板材。
// 注：
//   1) 当添加一个零件时可以连续操作。
class AddPartPlacementFromDxfdwgAction : public PartPlacementEditAction, public PartPlacementMoveBase
{
public:
	AddPartPlacementFromDxfdwgAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
		GlViewPortPtr pViewPort, PartItemListPtr pPartItemList, const std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart,
		FolderNodePtr pSaveFolder, PartDistanceDetectActionBar* pPartDistanceDetectActionBar);
	~AddPartPlacementFromDxfdwgAction(void);

public: // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return EDIT_ADDPARTPMT_EX; }
	virtual int GetActionName() const override { return IDS_COMMAND_ADD_PARTPMT; }
	virtual void UpdateForUndoRedo() override;

private:
	// 从dxf加载的零件项。
	PartItemListPtr m_pPartItemList;

	// 这些零件项是否被保存。
	BOOL m_bPartItemSaved;

	// 零件项的保存路径。
	FolderNodePtr m_pSaveFolder;
};

END_CUTLEADER_NAMESPACE()
