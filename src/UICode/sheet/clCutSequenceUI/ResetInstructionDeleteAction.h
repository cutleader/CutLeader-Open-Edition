#pragma once

#include "SequenceEditActionBase.h"
#include "Point2D.h"
#include <map>

DECLARE_CUTLEADER_CLASS(ICutNode)

BEGIN_CUTLEADER_NAMESPACE()

// 删除复位点。
// 注：
//  待完善：目前没有提供undo/redo功能。
class ResetInstructionDeleteAction : public SequenceEditActionBase
{
public:
	ResetInstructionDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~ResetInstructionDeleteAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_DEL_RESET_INSTRUCT; }
	virtual int GetActionName() const override { return IDS_SEQ_DEL_CHAIN_RESET; }

private:
    // 更新连割工序的切割路径和复位点。
    void Update_cutPath_and_resetPt();

    // 计算切割路径上距离鼠标最近的复位点
    void Calc_closestResetPt_onCutPath(const Point2D& pt);

private:
    // 所有的连割工序
    CutSequenceListPtr m_pAllChainSeqs;

    // 所有连割工序的切割路径，此路径是和模拟时一样的路径，可能考虑了偏移的。
    CutNodeListPtr m_pCutNode_4_chainSeq;

    // 板材上所有的复位点，这里用来显示，需要实时维护
    Point2DListPtr m_pResetPtList;

    // 鼠标移动时，切割路径上距离鼠标最近的复位点，要是没有复位点了就为空。
    Point2DPtr m_pClosestResetPt_onCutPath;
};

END_CUTLEADER_NAMESPACE()
