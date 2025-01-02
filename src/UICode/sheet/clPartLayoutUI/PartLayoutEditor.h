#pragma once

#include "clCutFeatureUICommon.h"
#include "clCutFeatureCommon.h"
#include "EditorBase.h"
#include "PartDistanceDetectActionBar.h"
#include "SpreadSamePartActionBar.h"
#include "CreatePartGridActionBar.h"
#include "SelectPartOrLoopActionBar.h"
#include "ChangeLoopToolActionBar.h"
#include "boost/enable_shared_from_this.hpp"
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(PartItemList)
DECLARE_CUTLEADER_CLASS(PartGroupList)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PartPlacementList)
DECLARE_CUTLEADER_CLASS(PartInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 零件布局编辑器。
class ClPartLayoutUI_Export PartLayoutEditor : public EditorBase, public boost::enable_shared_from_this<PartLayoutEditor>
{
public:
	PartLayoutEditor(GlViewPortPtr pViewPort);
	~PartLayoutEditor(void);

public: // implement IInteract interface.
	BOOL LButtonDown(double& dPtX, double& dPtY);
	BOOL LButtonUp(double& dPtX, double& dPtY);
	BOOL LButtonDBClk(double& dPtX, double& dPtY);
	BOOL RButtonUp(double& dPtX, double& dPtY);
	BOOL MovePoint(double& dPtX, double& dPtY);
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IEditor interface.
	virtual CString GetEditorName() const override;

public: // set/get functions
	void SetPartPlacements(PartPlacementListPtr pPartPlacements) { m_pPartPlacementList = pPartPlacements; }
	void SetPartInsts(PartInstanceListPtr pPartInstances) { m_pPartInstanceList = pPartInstances; }
	const PartInstanceList* GetPartInstances() const { return m_pPartInstanceList.get(); }
    void SetPartGroups(PartGroupListPtr pPartGroups) { m_pPartGroups = pPartGroups; }

	void SetMatSize(IMaterialSizePtr pMaterialSize) { m_pMatSize = pMaterialSize; }
	IMaterialSizePtr GetMatSize() const { return m_pMatSize; }

	void SetParamConfig(ParamConfigPtr pParamConfig) { m_pParamConfig = pParamConfig; }
	ParamConfigPtr GetParamConfig() const { return m_pParamConfig; }

public:
	// 启动action，在板材上面添加零件库中的零件。
	void AddPartPmtFromDataCenter(LONGLONG iPartID, LONGLONG iShtParamConfigID);

	// 启动action，在板材上面添加“零件项”集合。
	void AddPartPmtFromDxfdwg(PartItemListPtr pPartItemList, const std::map<LONGLONG, std::vector<std::pair<LONGLONG, Matrix2D>>>& matrixData_of_sameShapePart, FolderNodePtr pSaveFolder);

    // 启动零件放置选择action，并预先选择指定的零件放置。
	// 注：
	// 1) 该函数负责释放pPartPlacementIDs_preSelect。
    void StartPartPlacementSelectAction_andPreSelect(std::vector<LONGLONG>* pPartPlacementIDs_preSelect);

	// 启动轮廓实例选择action，并预先选择指定的轮廓实例。
	void StartLoopInstanceSelectAction_andPreSelect(LoopInstanceListPtr pLoopInstances_preSelect);
	// 注：
	// 1) 该函数负责释放pIdPairs，pIdPairs中是<part instance id, pattern loop id>。
	void StartLoopInstanceSelectAction_andPreSelect(std::vector<std::pair<LONGLONG, LONGLONG>>* pIdPairs);

	BOOL SetEditAction(EDIT_SHEET_ACTION_TYPE actionType);
	BOOL SetViewAction(VIEW_ACTION_TYPE viewType);

    // 启动动作来修改轮廓刀具。
    void ChangeToolOfSelectedLoopInstances(int iToolIndex);

	// 启动动作切换轮廓的内外切。
	void ChangeLoopCutOffset_ByAction(PatternLoopCutOffsetType emCutOffsetType);

	// 启动动作切换轮廓的切割方向。
	void ChangeLoopCutDirection_ByAction(CutDirectionChangeType emCutDirectionChangeType);

	// 启动action来编辑轮廓引线。
	// 注：
	// 1) 如pSelectedLoopInstances中有元素，就编辑它们的引线。
	void EditLoopLeadLine(LoopInstanceListPtr pSelectedLoopInstances);

	// 隐藏所有action bar和面板。
	void HideAllActionBars();
	void HideAllPanels();

	// 得到当前选中的零件，如果当前选的是轮廓，那就返回轮廓所在的零件（需要去重复）。
	PartPlacementListPtr GetSelectedPartPlacements() const;

    // 得到当前选中的轮廓，如果当前选的是零件，那就返回零件的轮廓。
    LoopInstanceListPtr GetSelectedLoopInstances() const;

private:
	PartPlacementListPtr m_pPartPlacementList;
	PartInstanceListPtr m_pPartInstanceList;
    PartGroupListPtr m_pPartGroups;

	// 板材用到的材料。
	IMaterialSizePtr m_pMatSize;

	// 板材用到的参数配置。
	ParamConfigPtr m_pParamConfig;

	// 几个action bar，父窗口是板材视图，每个板材视图都有这么一套。
	PartDistanceDetectActionBar* m_pPartDistanceDetectActionBar; // “智能靠碰参数”对话框。
	SpreadSamePartActionBar* m_pSpreadSamePartActionBar; // “传播相同零件”对话框。
	CreatePartGridActionBar* m_pCreatePartGridActionBar; // “创建零件阵列”的action bar。
    SelectPartOrLoopActionBar* m_pSelectPartOrLoopActionBar; // 选择零件和选择轮廓动作的action工具条。
    ChangeLoopToolActionBar* m_pChangeLoopToolActionBar; // 修改轮廓刀具用到的action工具条。
};

END_CUTLEADER_NAMESPACE()
