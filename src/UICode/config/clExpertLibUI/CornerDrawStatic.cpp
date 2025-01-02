#include "StdAfx.h"
#include "CornerDrawStatic.h"

#include "Point2D.h"
#include "GlViewPort.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(CornerDrawStatic, CStatic)

CornerDrawStatic::CornerDrawStatic(void)
{
	m_dMaxSize = 100;

	m_pViewPort.reset(new GlViewPort);
}

CornerDrawStatic::~CornerDrawStatic(void)
{
}

BEGIN_MESSAGE_MAP(CornerDrawStatic, CStatic)
	ON_WM_PAINT()

END_MESSAGE_MAP()

void CornerDrawStatic::InitDrawer()
{
	m_pViewPort->InitEnv(m_hWnd, 0.00001, 10000);
	m_pViewPort->SetBackgroundColor( SkinManager::GetBackgroundColor() );
	m_pViewPort->SetDrawColor(RGB(255, 0, 0));

	CRect rect;
	GetClientRect(&rect);
	m_pViewPort->SetDrawingArea(m_dMaxSize, rect.Width(), rect.Height(), 0, 0);
}

void CornerDrawStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	m_pViewPort->BindRendContext();
	m_pViewPort->ClearScreen();

	// 绘制边框。
	double dDeduction = 0.5;
	m_pViewPort->SetDrawColor( SkinManager::GetTextColor());
	m_pViewPort->SetLineWidth(1);
	m_pViewPort->DrawLinePoint2D(dDeduction, dDeduction, m_dMaxSize-dDeduction, dDeduction);
	m_pViewPort->DrawLinePoint2D(m_dMaxSize-dDeduction, dDeduction, m_dMaxSize-dDeduction, m_dMaxSize-dDeduction);
	m_pViewPort->DrawLinePoint2D(m_dMaxSize-dDeduction, m_dMaxSize-dDeduction, dDeduction, m_dMaxSize-dDeduction);
	m_pViewPort->DrawLinePoint2D(dDeduction, dDeduction, dDeduction, m_dMaxSize-dDeduction);

	// 绘制角特征。
	DrawRangeCorner();

	m_pViewPort->SwapBuffers();
}

void CornerDrawStatic::DrawRangeCorner()
{
	double dDrawAngle = 0.0, dRadius = 0.0;
	double dRangeAngle = m_pRangeCornerInfo->GetRangeAngle();
	double dCornerVal = m_pRangeCornerInfo->GetCornerValue();

	int iCornerType = (int)m_pRangeCornerInfo->GetCornerType();
	switch(iCornerType)
	{
	case 0:	// Corner_None
		break;
	case 1: // Corner_Cross
		{
			if (dCornerVal == 0)
            {
				break;
            }

			if (dRangeAngle <= PI)
			{
				// draw angle arc
				m_pViewPort->SetLineWidth(1);
				m_pViewPort->SetLineStipple(false, 2, 0xffff);
				m_pViewPort->SetDrawColor(RGB(0, 255, 0));
				m_pViewPort->DrawArc2D(50, 50, 10, 0, dRangeAngle);

				// Draw Base Line
				m_pViewPort->SetLineWidth(3);
				m_pViewPort->SetDrawColor(RGB(0, 0, 255));
				m_pViewPort->DrawLine2D(50, 50, 50, 0);
				m_pViewPort->DrawLine2D(50, 50, 50, dRangeAngle);

				// Draw cross.
				m_pViewPort->SetLineWidth(2);
				m_pViewPort->SetDrawColor(RGB(255, 0, 0));
				m_pViewPort->DrawLine2D(50, 50, 20, PI);
				m_pViewPort->DrawLine2D(50, 50, 20, dRangeAngle-PI);
				//
				Point2D tmpPt(50, 50);
				Vector2D v1;
				v1.Magnitude(20);
				v1.Rotate(dRangeAngle-PI);
				tmpPt.Transfer(v1);
				m_pViewPort->DrawLinePoint2D(30, 50, tmpPt.X(), tmpPt.Y());
			}
			else
			{
				dDrawAngle = TWOPI - dRangeAngle;

				// draw angle arc
				m_pViewPort->SetLineWidth(1);
				m_pViewPort->SetLineStipple(false, 2, 0xffff);
				m_pViewPort->SetDrawColor(RGB(0, 255, 0));
				m_pViewPort->DrawArc2D(50, 50, 10, dDrawAngle, dRangeAngle);

				// Draw Base Line
				m_pViewPort->SetLineWidth(3);
				m_pViewPort->SetDrawColor(RGB(0, 0, 255));
				m_pViewPort->DrawLine2D(50, 50, 50, 0);
				m_pViewPort->DrawLine2D(50, 50, 50, dDrawAngle);

				// Draw cross.
				m_pViewPort->SetLineWidth(2);
				m_pViewPort->SetDrawColor(RGB(255, 0, 0));
				m_pViewPort->DrawLine2D(50, 50, 20, PI);
				m_pViewPort->DrawLine2D(50, 50, 20, dDrawAngle-PI);
				//
				Point2D tmpPt(50, 50);
				Vector2D v1;
				v1.Magnitude(20);
				v1.Rotate(dDrawAngle-PI);
				tmpPt.Transfer(v1);
				m_pViewPort->DrawLinePoint2D(30, 50, tmpPt.X(), tmpPt.Y());
			}

			break;
		}
	case 2: // Corner_Radius
		{
			if (dRangeAngle <= PI)
			{
				// draw angle arc
				m_pViewPort->SetLineWidth(1);
				m_pViewPort->SetLineStipple(false, 2, 0xffff);
				m_pViewPort->SetDrawColor(RGB(0, 255, 0));
				m_pViewPort->DrawArc2D(50, 50, 10, 0, dRangeAngle);

				// draw base Line
				m_pViewPort->SetLineWidth(3);
				m_pViewPort->SetDrawColor(RGB(0, 0, 255));
				m_pViewPort->DrawLine2D(50, 50, 50, 0);
				m_pViewPort->DrawLine2D(50, 50, 50, dRangeAngle);

				// draw dRadius
				m_pViewPort->SetLineWidth(2);
				m_pViewPort->SetDrawColor(RGB(255, 0, 0));
				if (dRangeAngle >= PI/2 && dRangeAngle <= PI/2)
				{
					m_pViewPort->DrawArc2D(50 + 30/tan(dRangeAngle/2), 50+30, 30, PI/2*3 - (PI-dRangeAngle), PI-dRangeAngle);
				}
				else if (dRangeAngle <= PI/2)
				{
					m_pViewPort->DrawArc2D(50 + 12/tan(dRangeAngle/2), 50+12, 12, PI/2*3 - (PI-dRangeAngle), PI-dRangeAngle);
				}
				else if (dRangeAngle >= PI/2)
				{
					m_pViewPort->DrawArc2D(50 + 50/tan(dRangeAngle/2), 50+50, 50, PI/2*3 - (PI-dRangeAngle), PI-dRangeAngle);
				}
			}
			else
			{
				dDrawAngle = TWOPI - dRangeAngle;

				// draw angle arc
				m_pViewPort->SetLineWidth(1);
				m_pViewPort->SetLineStipple(false, 2, 0xffff);
				m_pViewPort->SetDrawColor(RGB(0, 255, 0));
				m_pViewPort->DrawArc2D(50, 50, 10, dDrawAngle, dRangeAngle);

				// Draw Base Line
				m_pViewPort->SetLineWidth(3);
				m_pViewPort->SetDrawColor(RGB(0, 0, 255));
				m_pViewPort->DrawLine2D(50, 50, 50, 0);
				m_pViewPort->DrawLine2D(50, 50, 50, dDrawAngle);

				// Draw Radius
				m_pViewPort->SetLineWidth(2);
				m_pViewPort->SetDrawColor(RGB(255, 0, 0));
				if (dDrawAngle >= PI/2 && dDrawAngle <= PI/2)
				{
					m_pViewPort->DrawArc2D(50 + 30/tan(dDrawAngle/2), 50+30, 30, PI/2*3 - (PI-dDrawAngle), PI-dDrawAngle);
				}
				else if (dDrawAngle <= PI/2)
				{
					m_pViewPort->DrawArc2D(50 + 12/tan(dDrawAngle/2), 50+12, 12, PI/2*3 - (PI-dDrawAngle), PI-dDrawAngle);
				}
				else if (dDrawAngle >= PI/2)
				{
					m_pViewPort->DrawArc2D(50 + 50/tan(dDrawAngle/2), 50+50, 50, PI/2*3 - (PI-dDrawAngle), PI-dDrawAngle);
				}
			}
			break;
		}
	case 3:	// Corner_Tab
		{
			if (dCornerVal == 0) 
				break;

			if (dRangeAngle <= PI)
			{
				// draw angle arc
				m_pViewPort->SetLineWidth(1);
				m_pViewPort->SetLineStipple(false, 2, 0xffff);
				m_pViewPort->SetDrawColor(RGB(0, 255, 0));
				m_pViewPort->DrawArc2D(50, 50, 10, 0, dRangeAngle);

				// Draw Base Line
				m_pViewPort->SetLineWidth(3);
				m_pViewPort->SetDrawColor(RGB(0, 0, 255));
				m_pViewPort->DrawLine2D(50, 50, 50, 0);
				m_pViewPort->DrawLine2D(50, 50, 50, dRangeAngle);

				// Draw Tab
				m_pViewPort->SetLineWidth(2);
				m_pViewPort->SetDrawColor(RGB(255, 0, 0));
				//
				Point2D tmpPt(50, 50);
				Vector2D v1;
				v1.Magnitude(20);
				v1.Rotate(dRangeAngle);
				tmpPt.Transfer(v1);
				m_pViewPort->DrawLinePoint2D(70, 50, tmpPt.X(), tmpPt.Y());
			}
			else
			{
				dDrawAngle = 2 * PI - dRangeAngle;

				// draw angle arc
				m_pViewPort->SetLineWidth(1);
				m_pViewPort->SetLineStipple(false, 2, 0xffff);
				m_pViewPort->SetDrawColor(RGB(0, 255, 0));
				m_pViewPort->DrawArc2D(50, 50, 10, dDrawAngle, dRangeAngle);

				// Draw Base Line
				m_pViewPort->SetLineWidth(3);
				m_pViewPort->SetDrawColor(RGB(0, 0, 255));
				m_pViewPort->DrawLine2D(50, 50, 50, 0);
				m_pViewPort->DrawLine2D(50, 50, 50, dDrawAngle);

				// Draw Tab
				m_pViewPort->SetLineWidth(2);
				m_pViewPort->SetDrawColor(RGB(255, 0, 0));
				//
				Point2D tmpPt(50, 50);
				Vector2D v1;
				v1.Magnitude(20);
				v1.Rotate(dDrawAngle);
				tmpPt.Transfer(v1);
				m_pViewPort->DrawLinePoint2D(70, 50, tmpPt.X(), tmpPt.Y());
			}
			break;
		}
	case 4:	// Corner_Loop
		{
            if (dCornerVal == 0)
            {
                break;
            }

            if (dRangeAngle <= PI)
            {
                dRadius = 50 / (1 + (1 / sin(dRangeAngle/2)));

                // draw angle arc
                m_pViewPort->SetLineWidth(1);
                m_pViewPort->SetLineStipple(false, 2, 0xffff);
                m_pViewPort->SetDrawColor(RGB(0, 255, 0));
                m_pViewPort->DrawArc2D(50, 50, 10, 0, dRangeAngle);

                // Draw Base Line
                m_pViewPort->SetLineWidth(3);
                m_pViewPort->SetDrawColor(RGB(0, 0, 255));
                m_pViewPort->DrawLine2D(50, 50, 50, 0);
                m_pViewPort->DrawLine2D(50, 50, 50, dRangeAngle);
                m_pViewPort->DrawLine2D(50, 50, dRadius / tan(dRangeAngle*0.5), PI);
                m_pViewPort->DrawLine2D(50, 50, dRadius / tan(dRangeAngle*0.5), dRangeAngle + PI);

                // Draw Loop
                m_pViewPort->SetLineWidth(2);
                m_pViewPort->SetDrawColor(RGB(255, 0, 0));
                m_pViewPort->DrawArc2D(50 - (50-dRadius)*cos(dRangeAngle*0.5), 50 - (50-dRadius)*sin(dRangeAngle*0.5), dRadius, PI/2, PI + dRangeAngle);
            }
            else
            {
                dDrawAngle = TWOPI - dRangeAngle;

                dRadius = 50 / (1 + (1 / sin(dDrawAngle/2)));

                // draw angle arc
                m_pViewPort->SetLineWidth(1);
                m_pViewPort->SetLineStipple(false, 2, 0xffff);
                m_pViewPort->SetDrawColor(RGB(0, 255, 0));
                m_pViewPort->DrawArc2D(50, 50, 10, dDrawAngle, dRangeAngle);

                // Draw Base Line
                m_pViewPort->SetLineWidth(3);
                m_pViewPort->SetDrawColor(RGB(0, 0, 255));
                m_pViewPort->DrawLine2D(50, 50, 50, 0);
                m_pViewPort->DrawLine2D(50, 50, 50, dDrawAngle);
                m_pViewPort->DrawLine2D(50, 50, dRadius / tan(dDrawAngle*0.5), PI);
                m_pViewPort->DrawLine2D(50, 50, dRadius / tan(dDrawAngle*0.5), dDrawAngle + PI);

                // Draw Loop
                m_pViewPort->SetLineWidth(2);
                m_pViewPort->SetDrawColor(RGB(255, 0, 0));
                m_pViewPort->DrawArc2D(50 - (50-dRadius)*cos(dDrawAngle*0.5), 50 - (50-dRadius)*sin(dDrawAngle*0.5), dRadius, PI/2, PI + dDrawAngle);
            }
			break;
		}
    case 5: // CoolingPoint
        {
            if (dRangeAngle <= PI)
            {
                // draw angle arc
                m_pViewPort->SetLineWidth(1);
                m_pViewPort->SetLineStipple(false, 2, 0xffff);
                m_pViewPort->SetDrawColor(RGB(0, 255, 0));
                m_pViewPort->DrawArc2D(50, 50, 10, 0, dRangeAngle);

                // draw base Line
                m_pViewPort->SetLineWidth(3);
                m_pViewPort->SetDrawColor(RGB(0, 0, 255));
                m_pViewPort->DrawLine2D(50, 50, 50, 0);
                m_pViewPort->DrawLine2D(50, 50, 50, dRangeAngle);
            }
            else
            {
                dDrawAngle = TWOPI - dRangeAngle;

                // draw angle arc
                m_pViewPort->SetLineWidth(1);
                m_pViewPort->SetLineStipple(false, 2, 0xffff);
                m_pViewPort->SetDrawColor(RGB(0, 255, 0));
                m_pViewPort->DrawArc2D(50, 50, 10, dDrawAngle, dRangeAngle);

                // Draw Base Line
                m_pViewPort->SetLineWidth(3);
                m_pViewPort->SetDrawColor(RGB(0, 0, 255));
                m_pViewPort->DrawLine2D(50, 50, 50, 0);
                m_pViewPort->DrawLine2D(50, 50, 50, dDrawAngle);
            }
            break;
        }
	default:
		break;
	}
}

END_CUTLEADER_NAMESPACE()
