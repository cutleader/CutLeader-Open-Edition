#include "StdAfx.h"
#include "CanvasZone.h"

#include "Polygon2DList.h"
#include "IntegerList.h"

BEGIN_CUTLEADER_NAMESPACE()

CanvasZone::CanvasZone(void)
{
	m_pInnerPolys.reset(new Polygon2DList);
	m_pPatLoopIDs4InnerPolys.reset(new IntegerList);
}

CanvasZone::~CanvasZone(void)
{
}

END_CUTLEADER_NAMESPACE()
