#pragma once

#include "SequenceEditActionBase.h"
#include "Point2D.h"
#include <map>

BEGIN_CUTLEADER_NAMESPACE()

// 创建余料切割线
// 注：
//  待完善：目前没有提供undo/redo功能。
class RemnantCutlineAddAction : public SequenceEditActionBase
{
public:
	RemnantCutlineAddAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData, GlViewPortPtr pViewPort);
	~RemnantCutlineAddAction(void);

public: // implements IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL RButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual BOOL DrawBeforeFinish() override;
	virtual int GetActionType() const override { return SEQUENCE_ADD_REM_CUTLINE; }
	virtual int GetActionName() const override { return IDS_SEQ_ADD_REM_CUTLINE_TIP; }
	virtual bool IsProcessRightClickEvent() const override { return true; }

private:
	// 切割线的端点。
	Point2DListPtr m_pEndPts_of_remCutLine;

	// 当前光标位置。
	Point2D m_currentPos;
};

END_CUTLEADER_NAMESPACE()
