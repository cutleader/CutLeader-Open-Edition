#pragma once

#include "clGeometryFeatureUICommon.h"
#include "EditorBase.h"
#include "boost/enable_shared_from_this.hpp"
#include "TextParamActionBar.h"
#include "clGeometryFeatureCommon.h"

DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(PartCadData)

BEGIN_CUTLEADER_NAMESPACE()

// 图形编辑器。
class ClGeometryFeatureUI_Export GeometryFeatureEditor : public EditorBase, public boost::enable_shared_from_this<GeometryFeatureEditor>
{
public:
	GeometryFeatureEditor(GlViewPortPtr pViewPort);
	~GeometryFeatureEditor(void);

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

public: // get/set functions.
	void SetPartCadData(PartCadDataPtr pPartCadData) { m_pPartCadData = pPartCadData; }
	void StartPatternSelectAction_andPreSelect(PatternListPtr pPatterns_preSelect, TextStructListPtr pTextStructs_preSelect);

public:
	BOOL SetDrawAction(DRAW_ACTION_TYPE drawType);
	BOOL SetViewAction(VIEW_ACTION_TYPE viewType);
	BOOL SetEditAction(EDIT_ACTION_TYPE editType);

	// 启动action，对齐轮廓。
	void AlignLoop(AlignLoopStyle emAlignLoopStyle);

private:
	// 得到当前选中的几何特征。
	std::pair<PatternListPtr, TextStructListPtr> GetSelectedPatterns();

private:
	PartCadDataPtr m_pPartCadData;

	// 几个action bar，父窗口是零件视图。
	TextParamActionBar* m_pTextParamActionBar; // 文字参数action工具条。
};

END_CUTLEADER_NAMESPACE()
