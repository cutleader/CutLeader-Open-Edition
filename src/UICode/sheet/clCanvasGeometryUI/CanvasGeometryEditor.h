#pragma once

#include "clCanvasGeometryUICommon.h"
#include "EditorBase.h"
#include "SelectPatternOrLoopActionBar.h"
#include "TextParamActionBar.h"
#include "CreateGeometryGridActionBar.h"
#include "LoopPositionDetectActionBar.h"
#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(PartPlacement)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(PartInstance)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)

BEGIN_CUTLEADER_NAMESPACE()

// 底图几何特征编辑器。
class ClCanvasGeometryUI_Export CanvasGeometryEditor : public EditorBase
{
public:
	CanvasGeometryEditor(GlViewPortPtr pViewPort);
	~CanvasGeometryEditor(void);

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

public:
	void SetPartPlacement(PartPlacementPtr pPartPlacement) { m_pPartPlacement = pPartPlacement; }
	PartPlacementPtr GetPartPlacement() const { return m_pPartPlacement; }
	void SetPartInstance(PartInstancePtr pPartInstance) { m_pPartInstance = pPartInstance; }
	void SetParamConfig(ParamConfigPtr pParamConfig) { m_pParamConfig = pParamConfig; }

public:
	BOOL SetEditAction(EditCanvasGeometryActionType emEditCanvasGeometryActionType);

	// 进入轮廓选择动作，不预选对象。
	void StartLoopSelectAction();

	// 进入轮廓选择动作，并预选中这些轮廓和轮廓中包含的文字。
	// 注：
	// 1) 该函数需要释放pPatternLoopIDs_preSelect和pTextStructIDs_preSelect。
	void StartLoopSelectAction_PreSelectLoopAndText(std::vector<LONGLONG>* pPatternLoopIDs_preSelect, std::vector<LONGLONG>* pTextStructIDs_preSelect);

	// 进入图元选择动作，并预选中一个图元。
	// 注：
	// 1) 该函数需要释放pPatternID_preSelect。
	void StartPatternSelectAction_PreSelectOnePattern(LONGLONG* pPatternID_preSelect);

	// 把从dxf/dwg文件中读取的几何特征放到底图上。
	void AddPatternFromDxf(PatternListPtr pNewPatterns, PatternLoopListPtr pNewPatternLoops, TextStructListPtr pNewTextStructs,
		LoopTopologyItemListPtr pNewLoopTopologyItems);

	// 启动action，对齐轮廓。
	void AlignCanvasLoop(AlignLoopStyle emAlignLoopStyle);

	// 隐藏所有action bar和面板。
	void HideAllActionBars();
	void HideAllPanels();

	// 得到选中的轮廓实例。
	// 注：
	// 1) 当处于选择action下时，返回选中的轮廓实例。
	LoopInstanceListPtr GetSelectedLoopInstances() const;

	// 得到当前选中的几何特征。
	std::pair<PatternListPtr, TextStructListPtr> GetSelectedPatterns() const;

private:
	PartPlacementPtr m_pPartPlacement;
	PartInstancePtr m_pPartInstance;

	// 底图用到的参数配置。
	ParamConfigPtr m_pParamConfig;

	// 几个action bar，父窗口是板材视图。
	SelectPatternOrLoopActionBar* m_pSelectPatternOrLoopActionBar; // 选择几何特征或选择轮廓动作的action工具条。
	TextParamActionBar* m_pTextParamActionBar; // 文字参数action工具条。
	CreateGeometryGridActionBar* m_pCreateGeometryGridActionBar; // 阵列参数action工具条。
	LoopPositionDetectActionBar* m_pLoopPositionDetectActionBar; // 轮廓距离检测action工具条。
};

END_CUTLEADER_NAMESPACE()
