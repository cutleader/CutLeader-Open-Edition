#include "StdAfx.h"
#include "ZoomInAction.h"

#include "math.h"
#include "GlViewPort.h"

BEGIN_CUTLEADER_NAMESPACE()

ZoomInAction::ZoomInAction(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;
}

ZoomInAction::~ZoomInAction(void)
{
}

void ZoomInAction::GetZoomPort(double& dWorldRange, double& dScreenLength, double& dScreenHeight, double& dLeftBottomX, double& dLeftBottomY)
{
	m_pViewPort->GetDrawingArea(dWorldRange, dScreenLength, dScreenHeight, dLeftBottomX, dLeftBottomY);
	if (dScreenLength == 0.0)
		return;

	double lo_Length = dScreenLength;
	m_pViewPort->DToW(lo_Length);

	double ZoomFactor = fabs(m_secondPtX - m_basePtX)/lo_Length ;
	dWorldRange = dWorldRange * ZoomFactor;

	dLeftBottomX = min(m_basePtX, m_secondPtX);
	dLeftBottomY = min(m_basePtY, m_secondPtY);
}

END_CUTLEADER_NAMESPACE()
