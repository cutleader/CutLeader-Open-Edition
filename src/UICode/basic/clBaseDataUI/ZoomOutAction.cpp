#include "StdAfx.h"
#include "ZoomOutAction.h"

#include "math.h"
#include "GlViewPort.h"

BEGIN_CUTLEADER_NAMESPACE()

ZoomOutAction::ZoomOutAction(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
}

ZoomOutAction::~ZoomOutAction(void)
{
}

void ZoomOutAction::GetZoomPort(double& dWorldRange, double& dScreenLength, double& dScreenHeight, double& dLeftBottomX, double& dLeftBottomY)
{
	m_pViewPort->GetDrawingArea(dWorldRange, dScreenLength, dScreenHeight, dLeftBottomX, dLeftBottomY);
	if (dScreenLength == 0.0)
		return;

	double lo_Length = dScreenLength;
	m_pViewPort->DToW(lo_Length);

	double ZoomFactor = lo_Length /fabs(m_secondPtX - m_basePtX);
	dWorldRange = dWorldRange * ZoomFactor;

	dLeftBottomX = (dLeftBottomX - min(m_basePtX,m_secondPtX))*ZoomFactor + dLeftBottomX;
	dLeftBottomY = (dLeftBottomY - min(m_basePtY,m_secondPtY))*ZoomFactor + dLeftBottomY;
}

END_CUTLEADER_NAMESPACE()
