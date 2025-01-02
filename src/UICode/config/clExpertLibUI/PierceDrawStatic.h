#pragma once

#include "clGeometryBaseCommon.h"
#include "PierceLocationInfo.h"

DECLARE_CUTLEADER_CLASS(GplusViewPort)

BEGIN_CUTLEADER_NAMESPACE()

// we can draw Pierce in the static control.
class PierceDrawStatic : public CStatic
{
	DECLARE_DYNAMIC(PierceDrawStatic)

public:
	PierceDrawStatic(void);
	~PierceDrawStatic(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public: // set/get functions.
	void SetDrawer(GplusViewPortPtr pGplusViewPort) { m_pGDIPlusDrawer = pGplusViewPort; }

	void SetShape(PIERCE_SHAPETYPE shape) { m_shape = shape; }
	PIERCE_SHAPETYPE GetShape() { return m_shape; }

	void SetPierceLocAngle(double dAngle) { m_dPierceLocAngle = dAngle; }
	double GetPierceLocAngle() { return m_dPierceLocAngle; }

	void SetDirection(DIRECTION dir) { m_dir = dir; }

private:
	// get the input point angle.
	// notes: the origin is the left-bottom of the cstatic.
	double GetAngleByPt(CPoint point);

private:
	// draw the pierce.
	void DrawPierce();

private:
	GplusViewPortPtr m_pGDIPlusDrawer;

	// pierce location data.
	PIERCE_SHAPETYPE m_shape;
	double m_dPierceLocAngle;
	DIRECTION m_dir;

	double m_dMaxSize;
};

END_CUTLEADER_NAMESPACE()
