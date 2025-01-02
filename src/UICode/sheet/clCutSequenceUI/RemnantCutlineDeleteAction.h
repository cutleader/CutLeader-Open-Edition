#pragma once

#include "SequenceEditActionBase.h"

DECLARE_CUTLEADER_CLASS(ICutSequence)

BEGIN_CUTLEADER_NAMESPACE()

// 删除选中的快速移动点。
// 注：
//  待完善：目前没有提供undo/redo功能。
class RemnantCutlineDeleteAction : public SequenceEditActionBase
{
public:
	RemnantCutlineDeleteAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~RemnantCutlineDeleteAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_DEL_REM_CUTLINE; }
	virtual int GetActionName() const override { return IDS_SEQ_DEL_REM_CUTLINE_TIP; }

private:
	ICutSequencePtr m_pActiveRemnantLineSeq;
};

END_CUTLEADER_NAMESPACE()
