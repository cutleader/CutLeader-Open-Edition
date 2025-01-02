#pragma once

#include "PartPlacementEditAction.h"
#include "PartPlacementMoveBase.h"
#include "SelectPartOrLoopActionBar.h"
#include "WrongToolSizeLoopInstances.h"

DECLARE_CUTLEADER_CLASS(PartPlacementSelectAction)

BEGIN_CUTLEADER_NAMESPACE()

// 选择“零件放置”的动作。
class PartPlacementSelectAction : public PartPlacementEditAction, public PartPlacementMoveBase
{
	enum PARTPMT_SELECT_STATE
	{
		PARTPMT_SELECT_NONE          = 0,
		PARTPMT_SELECT_FIRST_POINT   = 1, // 左键按下
		PARTPMT_SELECT_SECOND_POINT  = 2  // 左键抬起
	};

public:
	PartPlacementSelectAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
        GlViewPortPtr pViewPort, PartDistanceDetectActionBar* pPartDistanceDetectActionBar, SelectPartOrLoopActionBar* pSelectPartOrLoopActionBar);
	~PartPlacementSelectAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return EDIT_SELECTPRTPMT; }
	virtual int GetActionName() const override { return IDS_PARTPMT_SELECT; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

public:
	// 清空选择集，选择pPartPlacements。
	void SelectPartPlacements(PartPlacementListPtr pPartPlacements);

    PartPlacementListPtr GetSelectedPrtPmts() const { return m_pSelectedPartPlacements; }

	// 是不是处于快捷编辑模式。
	bool IsUnderQuickEditMode() const { return m_emQuickEditType != NoneQuickEdit; }

    WrongToolSizeLoopInstancesPtr GetWrongToolSizeLoopInstances() const { return m_pWrongToolSizeLoopInstances; }

private:
    // 更新零件放置的外接矩形。
    void UpdateOuterRects_ofAllPartPlacements();


	/************************************************************************/
	// 封装一些修改零件放置的操作，可以被重用。
	// 注：
	// 1) 这些函数的封装程度较高，还更新了界面。

	// 把选中的零件放置旋转一个角度，旋转中心为选中零件放置的中心点。
	// 注:
	// 1) 调用前需确认选中了零件放置。
	void RotateSelectedPartPlacements(double dRotateAngle);

	// delete selected part pmts.
	void DeleteSelectedPartPlacements();

	// 步进移动选中的零件。
	void StepMove(POSITION_INFO emDir);

	// 快速移动选中的零件。
	void RapidMove(POSITION_INFO emDir);

	// 执行快捷拷贝。
	void ExecuteQuickCopy();

	// 执行快捷移动。
	void ExecuteQuickMove();
	/************************************************************************/


private:
	PARTPMT_SELECT_STATE m_state;

	// the data for selection.
	/*
		LeftTopPointX;
		LeftTopPointY;
		RightBottomPointX;
		RightBottomPointY;
	*/
	double m_data[4];

    // 鼠标移动的时候高亮的零件放置对象。
    PartPlacementListPtr m_pActivePartPlacements;

    // 选中的零件放置。
    PartPlacementListPtr m_pSelectedPartPlacements;


	/************************************************************************/	
	// 快捷编辑模式用到的数据。

	// 支持的快捷编辑类型。
	enum QuickEditType
	{
		NoneQuickEdit	= 0, // 无快捷编辑方式。
		Ctrl_C_Ctrl_V	= 1, // 用ctrl+c，ctrl+v的方式拷贝零件。
		Ctrl_Drag		= 2, // 用ctrl+拖拽的方式拷贝零件。
		Drag			= 3, // 用拖拽的方式移动零件。
	};

	// 当前快捷编辑类型。
	QuickEditType m_emQuickEditType;

	// 快捷编辑的基准点。这个变量不会更新。
	Point2D m_quickEditBasePt;

	// 拷贝缓冲区，保存要复制的“零件放置”对象。只有在ctrl+c时设置，ctrl+v时使用。
	PartPlacementListPtr m_pBasePartPlacements;
	/************************************************************************/


	// 缓存零件放置的外围矩形
	std::map<LONGLONG, Rect2D> m_rects_of_partPmt;

    SelectPartOrLoopActionBar* m_pSelectPartOrLoopActionBar;

    // 偏移失败的几何轮廓。
    // 注：
    //  1）不少action完成后都要回到这个action，在回到这个action之前用户需要决定要不要设置该值。
    WrongToolSizeLoopInstancesPtr m_pWrongToolSizeLoopInstances;
};

END_CUTLEADER_NAMESPACE()
