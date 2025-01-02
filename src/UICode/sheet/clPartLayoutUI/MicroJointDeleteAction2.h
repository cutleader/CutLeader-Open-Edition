#pragma once

#include "SpreadSamePartActionBar.h"
#include "MicroJointPreSelectAction2.h"

BEGIN_CUTLEADER_NAMESPACE()

// 在板材上删除“微连接特征”的动作。
class MicroJointDeleteAction2 : public MicroJointPreSelectAction2
{
public:
	MicroJointDeleteAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
		PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar);
	virtual ~MicroJointDeleteAction2(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return PMT_MJ_DELETE; }
	virtual int GetActionName() const override { return IDS_DeleteMicroJoint; }

public:
	// 选中这些微连接特征。
	// 注: 
	//   1) 这个函数类似鼠标选中的操作。
	void SelectMJList(MicroJointListPtr pMicroJointList);

private:
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar;
};

END_CUTLEADER_NAMESPACE()
