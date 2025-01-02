#include "StdAfx.h"
#include "CanvasPatternGripEditActionBase.h"

#include "clUILibResource.h"
#include "PatternSelectAction.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternGripEditActionBase::CanvasPatternGripEditActionBase(PartPlacementPtr pPartPlacement, GlViewPortPtr pViewPort)
	: CanvasGeometryEditAction(pPartPlacement, pViewPort)
{
	m_pGripEditPat.reset();
}

CanvasPatternGripEditActionBase::~CanvasPatternGripEditActionBase()
{
}

END_CUTLEADER_NAMESPACE()
