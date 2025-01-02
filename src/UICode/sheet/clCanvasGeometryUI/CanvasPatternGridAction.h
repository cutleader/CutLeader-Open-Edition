#pragma once

#include "CanvasPatternPreSelectAction.h"
#include "Matrix2D.h"
#include "ObserverBase.h"

DECLARE_CUTLEADER_CLASS(CanvasPatternGridAction)
DECLARE_CUTLEADER_CLASS(PatternList)
DECLARE_CUTLEADER_CLASS(SelectPatternOrLoopActionBar)
DECLARE_CUTLEADER_CLASS(TextStructList)
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(CreateGeometryGridActionBar)

BEGIN_CUTLEADER_NAMESPACE()

// 阵列底图上几何特征的action。
// 注：
// 1) 目前只能选择几何轮廓进行阵列。
class CanvasPatternGridAction : public CanvasPatternPreSelectAction, public ObserverBase
{
public:
	CanvasPatternGridAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
		SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, CreateGeometryGridActionBar* pCreateGeometryGridActionBar, GlViewPortPtr pViewPort);
	~CanvasPatternGridAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL LButtonUp(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return GridPatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_COMMAND_GridCanvasLoop; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;
	virtual SnapInputPtr GetSnapInput() const override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

private:
	void GenerateLineArc2Ds4Preview();

protected:
	CreateGeometryGridActionBar* m_pCreateGeometryGridActionBar;

	// 用于预览的二维直线圆弧。
	LineArc2DListPtr m_pLineArc2Ds4Preview;
};

END_CUTLEADER_NAMESPACE()
