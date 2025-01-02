#pragma once

#include "clPostProcessorUICommon.h"
#include "Point2D.h"
#include "ActionBase.h"
#include "WrongToolSizeLoopInstances.h"
#include "clPostProcessorResource.h"

DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(CutNodeList)
DECLARE_CUTLEADER_CLASS(NCSimulateAction)
DECLARE_CUTLEADER_CLASS(IMaterialSize)

BEGIN_CUTLEADER_NAMESPACE()

// this class can display the simulate shape of the NC rows.
class ClPostProcessorUI_Export NCSimulateAction : public ActionBase
{
public:
	NCSimulateAction(IMaterialSizePtr pMaterialSize, GlViewPortPtr pViewPort);
	~NCSimulateAction(void);

public: // implements IInteract interface.
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return MACROUTER_NC_SIMULATE; }
	virtual int GetActionName() const override { return IDS_MACROUTER_NC_CODE; }

public:
	void ViewCutNode(SequenceSimulateShapeListPtr pSeqSimShapeList, CutNodeListPtr pCutNodeList);

    WrongToolSizeLoopInstancesPtr GetWrongToolSizeLoopInstances() const { return m_pWrongToolSizeLoopInstances; }

private:
	// 所有“模拟形状”。
	SequenceSimulateShapeListPtr m_pSequenceSimulateShapeList;

	// the "do-cut" cut nodes.
	// notes:
	//   1) these cut nodes are relative to the selected NC rows.
	CutNodeListPtr m_pCutNodeList;

    // 偏移失败的几何轮廓。
    WrongToolSizeLoopInstancesPtr m_pWrongToolSizeLoopInstances;

	// 当前板材使用的材料尺寸。
	IMaterialSizePtr m_pMaterialSize;
};

END_CUTLEADER_NAMESPACE()
