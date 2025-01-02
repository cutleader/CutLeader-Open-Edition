#include "StdAfx.h"
#include "PierceDrawStatic.h"

#include "math.h"
#include "baseConst.h"
#include "GplusViewPort.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(PierceDrawStatic, CStatic)

PierceDrawStatic::PierceDrawStatic(void)
{
	m_dMaxSize = 100;
}

PierceDrawStatic::~PierceDrawStatic(void)
{
}

BEGIN_MESSAGE_MAP(PierceDrawStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void PierceDrawStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	/************************************************************************/
	/*    init the GDI+ drawer.                                             */
	m_pGDIPlusDrawer->Initialize(this->m_hWnd);

	CRect rect;
	GetClientRect(&rect);
	m_pGDIPlusDrawer->SetViewPort(m_dMaxSize, rect.Width(), rect.Height(), 0, 0);
	m_pGDIPlusDrawer->SetPointMap(rect.Width()/2, rect.Height()/2, m_dMaxSize/2, m_dMaxSize/2);

	m_pGDIPlusDrawer->SetSmoothingMode(SMOOTHINGMODE_HIGHQUALITY);

	m_pGDIPlusDrawer->SetBackGroundColor( SkinManager::GetBackgroundColor() );
	m_pGDIPlusDrawer->SetDrawColor( SkinManager::GetTextColor() );
	m_pGDIPlusDrawer->SetLineWidth(1);

	m_pGDIPlusDrawer->ClearScreen();
	/************************************************************************/


	// draw frame。暂时不绘制边框。
	//m_pGDIPlusDrawer->DrawLinePoint2D(0, 0, 100, 0);
	//m_pGDIPlusDrawer->DrawLinePoint2D(100, 0, 100, 100);
	//m_pGDIPlusDrawer->DrawLinePoint2D(100, 100, 0, 100);
	//m_pGDIPlusDrawer->DrawLinePoint2D(0, 0, 0, 100);


	/************************************************************************/
	/* 	draw the geometry using the pierce data.                            */

	// init the environment
	m_pGDIPlusDrawer->SetViewPort(m_dMaxSize*1.3, rect.Width(), rect.Height(), 0, 0);
	m_pGDIPlusDrawer->SetPointMap(rect.Width()/2, rect.Height()/2, m_dMaxSize/2, m_dMaxSize/2);
	m_pGDIPlusDrawer->SetLineWidth(2);

	// init the arrow direction.
	if (m_dir == CCW)
		m_pGDIPlusDrawer->SetLineCap(LINECAP_FLAT, LINECAP_FWARROW);
	else if (m_dir == CW)
		m_pGDIPlusDrawer->SetLineCap(LINECAP_FWARROW, LINECAP_FLAT);

	// draw pierce location.
	DrawPierce();
	/************************************************************************/


	m_pGDIPlusDrawer->SwapBuffers();
}

double PierceDrawStatic::GetAngleByPt(CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	double dx = point.x - rect.Width()/2;
	double dy = point.y - rect.Height()/2;
	double dTmpAngle, dAngle;

	if ((dx <= 0) && (dx >= 0))
	{
		if (point.y < rect.Height()/2)
			dAngle = PI/2;
		else 
			dAngle = PI/2*3;
	}
	else
	{
		dTmpAngle = atan2(dx, dy);

		if (point.y > rect.Height()/2)
			dAngle = dTmpAngle + PI/2*3;
		else if (point.x > rect.Width()/2)
			dAngle = dTmpAngle - PI/2;
		else
			dAngle = dTmpAngle + PI/2*3;
	}

	return dAngle;
}

void PierceDrawStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// get the angle of the cursor.
	double dAngle = GetAngleByPt(point);

	// we should normalize the angle to PI/8,PI/8*3,....
	m_dPierceLocAngle = PierceLocationInfo::NormalizeAngle(m_shape, dAngle);

	Invalidate();

	CStatic::OnLButtonDown(nFlags, point);
}

void PierceDrawStatic::DrawPierce()
{
	double dPiercePointX, dPiercePointY;

	if (m_shape == PIERCE_ROUND)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		m_pGDIPlusDrawer->DrawArc2D(m_dMaxSize/2, m_dMaxSize/2, m_dMaxSize/2, 0, PI/2);
		m_pGDIPlusDrawer->DrawArc2D(m_dMaxSize/2, m_dMaxSize/2, m_dMaxSize/2, PI/2, PI/2);
		m_pGDIPlusDrawer->DrawArc2D(m_dMaxSize/2, m_dMaxSize/2, m_dMaxSize/2, PI, PI/2);
		m_pGDIPlusDrawer->DrawArc2D(m_dMaxSize/2, m_dMaxSize/2, m_dMaxSize/2, 3*(PI/2), PI/2);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		if (m_dPierceLocAngle < 0)
			return;

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		dPiercePointX = m_dMaxSize/2 * cos(m_dPierceLocAngle) + m_dMaxSize/2;
		dPiercePointY = m_dMaxSize/2 * sin(m_dPierceLocAngle) + m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		/************************************************************************/
	}
	else if (m_shape == PIERCE_OBROUND)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize/4, m_dMaxSize/4, m_dMaxSize/2, 0);
		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize/4*3, m_dMaxSize/4*3, m_dMaxSize/2 ,PI);
		m_pGDIPlusDrawer->DrawArc2D(m_dMaxSize/4, m_dMaxSize/2, m_dMaxSize/4, PI/2, PI);
		m_pGDIPlusDrawer->DrawArc2D(m_dMaxSize/4*3, m_dMaxSize/2, m_dMaxSize/4, -PI/2, PI);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		if (m_dPierceLocAngle < 0)
			return;

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if ((m_dPierceLocAngle >= PI/2) && (m_dPierceLocAngle <= PI/2) ||
			(m_dPierceLocAngle >= PI/2*3) && (m_dPierceLocAngle <= PI/2*3))
		{
			dPiercePointX = m_dMaxSize/4 * cos(m_dPierceLocAngle) + m_dMaxSize/2;
			dPiercePointY = m_dMaxSize/4 * sin(m_dPierceLocAngle) + m_dMaxSize/2;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= 0) && (m_dPierceLocAngle <= 0))
		{
			dPiercePointX = m_dMaxSize/4 * cos(m_dPierceLocAngle) + m_dMaxSize/4*3;
			dPiercePointY = m_dMaxSize/4 * sin(m_dPierceLocAngle) + m_dMaxSize/2;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else
		{
			dPiercePointX = m_dMaxSize/4 * cos(m_dPierceLocAngle) + m_dMaxSize/4;
			dPiercePointY = m_dMaxSize/4 * sin(m_dPierceLocAngle) + m_dMaxSize/2;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_RECT)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		m_pGDIPlusDrawer->DrawLine2D(0, 0, m_dMaxSize, 0);
		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize, 0, m_dMaxSize, PI/2);
		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize, m_dMaxSize, m_dMaxSize, PI);
		m_pGDIPlusDrawer->DrawLine2D(0, m_dMaxSize, m_dMaxSize, PI/2*3);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0) 
			return;

		BOOL dCross = false;
		for (int i = 0; i < 4; i++)
		{
			if ((m_dPierceLocAngle >= PI/2*i) && (m_dPierceLocAngle <= PI/2*i))
			{
				dCross = true;
				break;
			}
		}

		if (dCross)
		{
			dPiercePointX = m_dMaxSize/2 * cos(m_dPierceLocAngle) + m_dMaxSize/2;
			dPiercePointY = m_dMaxSize/2 * sin(m_dPierceLocAngle) + m_dMaxSize/2;

			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else
		{
			dPiercePointX = (m_dMaxSize/2/cos(PI/4)) * cos(m_dPierceLocAngle) + m_dMaxSize/2;
			dPiercePointY = (m_dMaxSize/2/cos(PI/4)) * sin(m_dPierceLocAngle) + m_dMaxSize/2;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_RCRECT)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		double dCenter = m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawLine2D(dCenter/5, 0, m_dMaxSize/5*4, 0);
		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize, dCenter/5, m_dMaxSize/5*4, PI/2);
		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize - dCenter/5, m_dMaxSize, m_dMaxSize/5*4, PI);
		m_pGDIPlusDrawer->DrawLine2D(0, m_dMaxSize - dCenter/5, m_dMaxSize/5*4, PI/2*3);

		m_pGDIPlusDrawer->DrawArc2D(dCenter/5, dCenter/5, dCenter/5, PI, PI/2);
		m_pGDIPlusDrawer->DrawArc2D(dCenter/5*9, dCenter/5, dCenter/5, PI/2*3, PI/2);
		m_pGDIPlusDrawer->DrawArc2D(dCenter/5*9, dCenter/5*9, dCenter/5, 0, PI/2 );
		m_pGDIPlusDrawer->DrawArc2D(dCenter/5, dCenter/5*9, dCenter/5, PI/2, PI/2);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0) 
			return;

		BOOL bCross = false;
		for (int i = 0; i < 4; i++)
		{
			if ((m_dPierceLocAngle >= PI/2*i) && (m_dPierceLocAngle <= PI/2*i))
			{
				bCross = true;
				break;
			}
		}

		if (bCross)
		{
			dPiercePointX = dCenter * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = dCenter * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else
		{
			dPiercePointX = (dCenter/5*4/cos(PI/4) + dCenter/5) * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = (dCenter/5*4/cos(PI/4) + dCenter/5) * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_SINGLED)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		double dCenter = m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawLine2D(dCenter - dCenter*cos(PI/4), dCenter - dCenter*sin(PI/4), 2 * dCenter*cos(PI/4), 0);
		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter, PI/4*7, PI/4*3);
		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter, PI/2, PI/4*3);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0)
			return;

		if ((m_dPierceLocAngle >= PI/2*3) && (m_dPierceLocAngle <= PI/2*3))
		{
			dPiercePointX = dCenter;
			dPiercePointY = dCenter - dCenter * sin(PI/4);
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else
		{
			dPiercePointX = dCenter * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = dCenter * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_DOUBLED)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		double dCenter = m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawLine2D(dCenter - dCenter*cos(PI/4), dCenter - dCenter*sin(PI/4), 2 * dCenter*cos(PI/4), 0);
		m_pGDIPlusDrawer->DrawLine2D(dCenter + dCenter*cos(PI/4), dCenter + dCenter*sin(PI/4), 2 * dCenter*cos(PI/4), PI);
		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter, PI/4*3, PI/2);
		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter, PI/4*7, PI/2);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0)
			return;

		if ((m_dPierceLocAngle >= PI/2) && (m_dPierceLocAngle <= PI/2) ||
			(m_dPierceLocAngle >= PI/2*3) && (m_dPierceLocAngle <= PI/2*3))
		{
			dPiercePointX = dCenter * sin(PI/4) * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = dCenter * sin(PI/4) * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else 
		{
			dPiercePointX = dCenter * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = dCenter * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_TRIANGLE)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		double dCenter = m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawLine2D(0, dCenter - dCenter/tan(PI/6)/2, m_dMaxSize, 0);
		m_pGDIPlusDrawer->DrawLine2D(m_dMaxSize, dCenter - dCenter/tan(PI/6)/2, m_dMaxSize, PI/3*2);
		m_pGDIPlusDrawer->DrawLine2D(dCenter, m_dMaxSize, m_dMaxSize, 4*(PI/3));
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0) 
			return;

		BOOL dCross = false;
		for (int i = 0; i < 4; i++)
		{
			if ((m_dPierceLocAngle >= PI/2*i) && (m_dPierceLocAngle <= PI/2*i) && (i != 1))
			{
				dCross = true;
				break;
			}
		}

		if ((m_dPierceLocAngle >= PI/2) && (m_dPierceLocAngle <= PI/2 ) || 
			(m_dPierceLocAngle >= PI/2*3) && (m_dPierceLocAngle <= PI/2*3))
		{
			dPiercePointX = (dCenter/tan(PI/6)/2) * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = (dCenter/tan(PI/6)/2) * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= 0) && (m_dPierceLocAngle <= 0) || 
				 (m_dPierceLocAngle >= PI) && (m_dPierceLocAngle <= PI))
		{
			dPiercePointX = dCenter/2*cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = dCenter/2*sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= PI/4*5) && (m_dPierceLocAngle <= PI/4*5))
		{
			m_pGDIPlusDrawer->DrawRoundPoint2D(0, dCenter - dCenter/tan(PI/6)/2, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= PI/4*7) && (m_dPierceLocAngle <= PI/4*7))
		{
			m_pGDIPlusDrawer->DrawRoundPoint2D(m_dMaxSize, dCenter - dCenter/tan(PI/6)/2, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_RADSLOT)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		double dCenter = m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter, 0, PID2);
		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter, PID2, PID2);

		m_pGDIPlusDrawer->DrawArc2D(dCenter, dCenter, dCenter/5*2, PI, -PI);

		double dx = dCenter - dCenter/10*7;
		double dy = dCenter;
		m_pGDIPlusDrawer->DrawArc2D(dx, dy, dCenter/10*3, PI, PI);

		dx = dCenter + dCenter/10*7;
		dy = dCenter;
		m_pGDIPlusDrawer->DrawArc2D(dx, dy, dCenter/10*3, PI, PI);
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0) 
			return;

		if ((m_dPierceLocAngle >= PID2) && (m_dPierceLocAngle <= PID2)) 
		{
			dPiercePointX = dCenter * cos(PID2) + dCenter;
			dPiercePointY = dCenter * sin(PID2) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= PID2*3) && ( m_dPierceLocAngle <= PID2*3))
		{
			dPiercePointX = dCenter/5*2 * cos(PID2) + dCenter;
			dPiercePointY = dCenter/5*2 * sin(PID2) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= 0) && (m_dPierceLocAngle <= 0))
		{
			dPiercePointX = dCenter + dCenter/10*7 * cos(m_dPierceLocAngle) + 
							dCenter/10*3 * cos(m_dPierceLocAngle + PID2);
			dPiercePointY = dCenter + dCenter/10*7 * sin(m_dPierceLocAngle) -
							dCenter/10*3 * sin(m_dPierceLocAngle + PID2);
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else if ((m_dPierceLocAngle >= PI) && (m_dPierceLocAngle <= PI))
		{
			dPiercePointX = dCenter + dCenter/10*7 * cos(m_dPierceLocAngle) - 
							dCenter/10*3 * cos(m_dPierceLocAngle + PID2);
			dPiercePointY = dCenter + dCenter/10*7 * sin(m_dPierceLocAngle) +
							dCenter/10*3 * sin(m_dPierceLocAngle + PID2);
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
	else if (m_shape == PIERCE_INSIDEPOLYGON || m_shape == PIERCE_EXTERIORPOLYGON)
	{
		/************************************************************************/
		// draw the geometry.

		m_pGDIPlusDrawer->SetDrawColor(RGB(0, 0, 255));

		double dCenter = m_dMaxSize/2;
		m_pGDIPlusDrawer->DrawLinePoint2D(m_dMaxSize, dCenter + dCenter*tan(PI/8), dCenter + dCenter/tan(PI/8*3), m_dMaxSize);
		m_pGDIPlusDrawer->DrawLinePoint2D(dCenter + dCenter/tan(PI/8*3), m_dMaxSize, dCenter + dCenter/tan(PI/8*5), m_dMaxSize);
		m_pGDIPlusDrawer->DrawLinePoint2D(dCenter + dCenter/tan(PI/8*5), m_dMaxSize, 0, dCenter - dCenter*tan(PI/8*7));
		m_pGDIPlusDrawer->DrawLinePoint2D(0, dCenter - dCenter*tan(PI/8*7), 0, dCenter - dCenter*tan(PI/8*9));
		m_pGDIPlusDrawer->DrawLinePoint2D(0, dCenter - dCenter*tan(PI/8*9), dCenter - dCenter/tan(PI/8*11), 0);
		m_pGDIPlusDrawer->DrawLinePoint2D(dCenter - dCenter/tan(PI/8*11), 0, dCenter - dCenter/tan(PI/8*13), 0);
		m_pGDIPlusDrawer->DrawLinePoint2D(dCenter - dCenter/tan(PI/8*13), 0, m_dMaxSize, dCenter + dCenter*tan(PI/8*15));
		m_pGDIPlusDrawer->DrawLinePoint2D(m_dMaxSize, dCenter + dCenter*tan(PI/8*15), m_dMaxSize, dCenter + dCenter*tan(PI/8));
		/************************************************************************/

		/************************************************************************/
		// draw Pierce Location

		m_pGDIPlusDrawer->SetDrawColor(RGB(255, 0, 0));

		if (m_dPierceLocAngle < 0) 
			return;

		BOOL dCorner = false;
		for (int i = 0; i < 8; i++)
		{
			if ((m_dPierceLocAngle >= PI/4*i) && (m_dPierceLocAngle <= PI/4*i))
			{
				dCorner = true;
				break;
			}
		}

		if (dCorner)
		{
			dPiercePointX = dCenter * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = dCenter * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		else
		{
			dPiercePointX = (dCenter/cos(PI/8)) * cos(m_dPierceLocAngle) + dCenter;
			dPiercePointY = (dCenter/cos(PI/8)) * sin(m_dPierceLocAngle) + dCenter;
			m_pGDIPlusDrawer->DrawRoundPoint2D(dPiercePointX, dPiercePointY, 4, TRUE);
		}
		/************************************************************************/
	}
}

END_CUTLEADER_NAMESPACE()
