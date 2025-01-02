#pragma once

#include "ICanvasPatternGripEditAction.h"
#include "CanvasGeometryEditAction.h"

BEGIN_CUTLEADER_NAMESPACE()

// this interface is used for Grip-editing pattern.
class CanvasPatternGripEditActionBase : virtual public ICanvasPatternGripEditAction, public CanvasGeometryEditAction
{
public:
	CanvasPatternGripEditActionBase(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort);
	virtual ~CanvasPatternGripEditActionBase();

protected:
	// the pattern which will be grip-edit.
	IPatternPtr m_pGripEditPat;
};

END_CUTLEADER_NAMESPACE()
