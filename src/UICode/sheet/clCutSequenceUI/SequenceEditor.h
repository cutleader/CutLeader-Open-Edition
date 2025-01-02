#pragma once

#include "clCutSequenceUICommon.h"
#include "Rect2D.h"
#include "EditorBase.h"
#include "ApplyPartActionBar.h"
#include "InsertPositionActionBar.h"
#include "GenerateRemCutLineActionBar.h"

DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(PartLayoutData)
DECLARE_CUTLEADER_CLASS(SequenceData)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// 工序编辑器。
class ClCutSequenceUI_Export SequenceEditor : public EditorBase
{
public:
	SequenceEditor(GlViewPortPtr pViewPort);
	~SequenceEditor(void);

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
	void SetMatSize(IMaterialSizePtr pMaterialSize) { m_pMatSize = pMaterialSize; }
	IMaterialSizePtr GetMatSize() const { return m_pMatSize; }

	void SetPartLayoutData(PartLayoutDataPtr pPartLayoutData) { m_pPartLayoutData = pPartLayoutData; }
	PartLayoutDataPtr GetPartLayoutData() const { return m_pPartLayoutData; }

	void SetSequenceData(SequenceDataPtr pSequenceData) { m_pSequenceData = pSequenceData; }
	SequenceDataPtr GetSequenceData() const { return m_pSequenceData; }

	void SetParamConfig(ParamConfigPtr pParamConfig) { m_pParamConfig = pParamConfig; }
	ParamConfigPtr GetParamConfig() const { return m_pParamConfig; }

public:
	BOOL SetAction(SequenceActionType actionType);

	// 隐藏所有action bar和面板。
	void HideAllActionBars();
	void HideAllPanels();
	
private:
	// the layout info of the sheet.
	PartLayoutDataPtr m_pPartLayoutData;

	// 板材上的工序数据。
	SequenceDataPtr m_pSequenceData;

	// 板材用到的材料。
	IMaterialSizePtr m_pMatSize;

	// 板材用到的参数配置。
	ParamConfigPtr m_pParamConfig;

	// 几个action bar，父窗口是板材视图，每个板材视图都有这么一套。
	ApplyPartActionBar* m_pApplyPartActionBar; // “以零件为单位”action bar。
	InsertPositionActionBar* m_pInsertPositionActionBar; // “指定插入位置”action bar。
	GenerateRemCutLineActionBar* m_pGenerateRemCutLineActionBar; // “生成余料切割线”action bar。
};

END_CUTLEADER_NAMESPACE()
