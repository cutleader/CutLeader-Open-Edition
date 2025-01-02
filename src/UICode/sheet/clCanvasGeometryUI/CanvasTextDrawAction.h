#pragma once

#include "CanvasGeometryEditAction.h"
#include "Text2ContourHelper.h"
#include "Vector2D.h"
#include "ObserverBase.h"

DECLARE_CUTLEADER_CLASS(CanvasTextDrawAction)
DECLARE_CUTLEADER_CLASS(TextParamActionBar)
DECLARE_CUTLEADER_CLASS(Polygon2DList);

BEGIN_CUTLEADER_NAMESPACE()

class CanvasTextDrawAction : public CanvasGeometryEditAction, public ObserverBase
{
public:
	CanvasTextDrawAction(PartPlacementPtr pPartPlacement, TextParamActionBar* pTextParamActionBar, GlViewPortPtr pViewPort);
	~CanvasTextDrawAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public: // implement IAction interface.
	virtual int GetActionType() const override { return DrawTextStructOnCanvas; }
	virtual int GetActionName() const override { return IDS_DrawText; }
	virtual BOOL DrawBeforeFinish() override;
	virtual void UpdateForUndoRedo() override;

public: // implement IObserver interface.
	virtual void Update(int iEventID, const CComVariant& varEventContent);

private:
	// 根据文字参数生成用于预览的文字轮廓。
	void GenerateTextContours();

private:
	// 文字参数action工具条。
	TextParamActionBar* m_pTextParamActionBar;

	// 用于预览的文字轮廓。
	Polygon2DListPtr m_pTextContours;
	TextContourGenerationErrorCode m_emErrorCode;
	Vector2D m_transferVect;
};

END_CUTLEADER_NAMESPACE()
