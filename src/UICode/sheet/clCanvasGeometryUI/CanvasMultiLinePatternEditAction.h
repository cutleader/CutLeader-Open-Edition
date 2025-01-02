#pragma once

#include "CanvasLinePatternEditAction.h"

DECLARE_CUTLEADER_CLASS(CanvasMultiLinePatternEditAction)

BEGIN_CUTLEADER_NAMESPACE()

class CanvasMultiLinePatternEditAction : public CanvasLinePatternEditAction
{
public:
	CanvasMultiLinePatternEditAction(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	~CanvasMultiLinePatternEditAction(void);

public:  // implement IInteract interface.
	virtual BOOL LButtonDown(double& dPtX, double& dPtY);
	virtual BOOL MovePoint(double& dPtX, double& dPtY);

public: // implement IAction interface.
	virtual int GetActionType() const override { return EditMultiLinePatternOnCanvas; }
	virtual int GetActionName() const override { return IDS_GEOMETRY_MULTILINE_TIP; }
};

END_CUTLEADER_NAMESPACE()
