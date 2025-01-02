#include "StdAfx.h"
#include "LeadDrawStatic.h"

#include "math.h"
#include "baseConst.h"
#include "GlViewPort.h"

#include "LeadConfigItem.h"
#include "SkinManager.h"

BEGIN_CUTLEADER_NAMESPACE()

IMPLEMENT_DYNAMIC(LeadDrawStatic, CStatic)

LeadDrawStatic::LeadDrawStatic(void)
{
	m_dMaxSize = 100;
	m_pViewPort.reset(new GlViewPort);
}

LeadDrawStatic::~LeadDrawStatic(void)
{
}

BEGIN_MESSAGE_MAP(LeadDrawStatic, CStatic)
	ON_WM_PAINT()

END_MESSAGE_MAP()

void LeadDrawStatic::InitDrawer()
{
	m_pViewPort->InitEnv(m_hWnd, 0.00001, 10000);
	m_pViewPort->SetBackgroundColor( SkinManager::GetBackgroundColor() );
	m_pViewPort->SetDrawColor(RGB(255, 0, 0));

	CRect rect;
	GetClientRect(&rect);
	m_pViewPort->SetDrawingArea(m_dMaxSize, rect.Width(), rect.Height(), 0, 0);
}

void LeadDrawStatic::InitLead(LeadInOutDataPtr pLeadIn, LeadInOutDataPtr pLeadOut)
{
	m_pLeadIn = pLeadIn;
	m_pLeadOut = pLeadOut;
}

void LeadDrawStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	m_pViewPort->BindRendContext();
	m_pViewPort->ClearScreen();

	// 绘制边框。
	int iDeduction = 1;
	m_pViewPort->SetDrawColor( SkinManager::GetBackgroundColor() );
	m_pViewPort->SetLineWidth(1);
	m_pViewPort->DrawLinePoint2D(iDeduction, iDeduction, m_dMaxSize-iDeduction, iDeduction);
	m_pViewPort->DrawLinePoint2D(m_dMaxSize-iDeduction, iDeduction, m_dMaxSize-iDeduction, m_dMaxSize-iDeduction);
	m_pViewPort->DrawLinePoint2D(m_dMaxSize-iDeduction, m_dMaxSize-iDeduction, iDeduction, m_dMaxSize-iDeduction);
	m_pViewPort->DrawLinePoint2D(iDeduction, iDeduction, iDeduction, m_dMaxSize-iDeduction);

	// 绘制线。
	m_pViewPort->SetDrawColor( SkinManager::GetBackgroundColor() );
	m_pViewPort->SetLineWidth(1);
	m_pViewPort->DrawLine2D(10, 10, 80, 0);

	// 绘制引入线。
	m_pViewPort->SetLineWidth(3);
	m_pViewPort->SetDrawColor(RGB(255, 0, 0));
	DrawLeadIn();

	// 绘制引出线。
	m_pViewPort->SetLineWidth(3);
	m_pViewPort->SetDrawColor(RGB(0, 0, 255));
	DrawLeadOut();

	m_pViewPort->SwapBuffers();
}

void LeadDrawStatic::DrawLeadIn()
{
	CRect rect;
	GetClientRect(&rect);

	double dLen = 0.0, dAng = 0.0, dRad = 0.0;

	if (!m_pLeadIn || !m_pLeadOut)
		return;

	LeadType leadInType = m_pLeadIn->GetType();
	LeadType leadOutType = m_pLeadOut->GetType();
	switch (leadInType)
	{
	case Lead_Line:
		{
			if (leadOutType != Lead_Arc)
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetLength() ? 40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetLength();
			}
			else
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetArcRadius() ? 40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetArcRadius();
			}
			dAng = PI - m_pLeadIn->GetAngle()*D2R;
			m_pViewPort->DrawLine2D(50, 10, dLen, dAng);

			break;
		}
	case Lead_TwoLines:
		{
			if (leadOutType != Lead_Arc)
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetLength() ? 
					   40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetLength();
			}
			else
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetArcRadius() ? 
					   40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetArcRadius();
			}
			dAng = PI - m_pLeadIn->GetAngle()*D2R;

			double dSplit = m_pLeadIn->GetLineSplit();
			double dTmpLen = dLen * (1-dSplit/100);
			m_pViewPort->DrawLinePoint2D(50, 10, 50 + dTmpLen/tan(dAng), 10+dTmpLen);
			m_pViewPort->DrawLinePoint2D(50 + dTmpLen/tan(dAng), 10 + dTmpLen, 50, dLen+10);

			break;
		}
	case Lead_LineArc:
		{
			if (leadOutType != Lead_Arc)
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetLength() ? 
					   40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetLength();
			}
			else
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetArcRadius() ? 
					   40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetArcRadius();
			}
			dAng = m_pLeadIn->GetArcAngle()*D2R;

			double dDisX = dLen * tan(dAng);
			dRad = dDisX * tan((PID2 - dAng)/2);
			double dLineLen = 0.0;
			if (dAng >= PID2 && dAng <= PID2)
			{
				dLineLen = 0;
				dRad = dLen / 2;
			}
			else 
				dLineLen = dRad / tan(dAng);

			double dInterX = 50 - dRad * cos(dAng);
			double dInterY = 10 + dRad + dRad* sin(dAng);
			m_pViewPort->DrawLinePoint2D(50, dInterY + dLineLen*cos(dAng), dInterX, dInterY);

			double dMoveAng = PID2 + dAng;
			m_pViewPort->DrawArc2D(50, 10 + dRad, dRad, 3*PID2 - dMoveAng, dMoveAng);

			break;
		}
	case Lead_Hook:
		{
			if (leadOutType != Lead_Arc)
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetLength() ? 
					   40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetLength();
			}
			else
			{
				dLen = m_pLeadIn->GetLength() >= m_pLeadOut->GetArcRadius() ? 
					   40 : 40 * m_pLeadIn->GetLength() / m_pLeadOut->GetArcRadius();
			}
			dRad = dLen * m_pLeadIn->GetArcRadius() / m_pLeadIn->GetLength(); 

			m_pViewPort->DrawLinePoint2D(50 - dRad, 10 + dRad, 50 - dRad, 50);
			m_pViewPort->DrawArc2D(50, 10 + dRad, dRad, PI, PID2);

			break;
		}
	}
}

void LeadDrawStatic::DrawLeadOut()
{
	CRect rect;
	GetClientRect(&rect);

	double dLen = 0.0, dAng = 0.0, dRad = 0.0;

	if (!m_pLeadIn || !m_pLeadOut)
		return;

	LeadType leadOutType = m_pLeadOut->GetType();
	switch (leadOutType)
	{
	case Lead_Line:
		{
			dLen = m_pLeadOut->GetLength() >= m_pLeadIn->GetLength() ? 
				   40 : 40 * m_pLeadOut->GetLength() / m_pLeadIn->GetLength();
			dAng = PI - m_pLeadOut->GetAngle()*D2R;

			m_pViewPort->DrawLine2D(50 + m_pLeadOut->GetOverCut() / m_pLeadOut->GetLength()*dLen, 10, dLen, dAng);

			break;
		}
	case Lead_Arc:
		{
			dRad = m_pLeadOut->GetArcRadius() >= m_pLeadIn->GetLength() ? 
				   40 : 40 * m_pLeadOut->GetArcRadius() / m_pLeadIn->GetLength();
			dAng = m_pLeadOut->GetArcAngle()*D2R;

			m_pViewPort->DrawArc2D(50 + m_pLeadOut->GetOverCut() / m_pLeadOut->GetArcRadius() * dRad, 
								   10 + dRad, dRad, 3*PID2, dAng);

			break;
		}
	case Lead_LineArc:
		{
			dLen = m_pLeadOut->GetLength() >= m_pLeadIn->GetLength() ? 
				   40 : 40 * m_pLeadOut->GetLength() / m_pLeadIn->GetLength();
			dAng = m_pLeadOut->GetArcAngle()*D2R;

			double dDisX = dLen * tan(dAng);
			dRad = dDisX * tan((PID2 - dAng)/2);

			double dLineLen = 0.0;
			if (dAng >= PID2 && dAng <= PID2)
			{
				dLineLen = 0;
				dRad = dLen/2;
			}
			else 
				dLineLen = dRad / tan(dAng);

			//
			double dInterX = 50 + m_pLeadOut->GetOverCut() / m_pLeadOut->GetLength() * dLen + dRad*cos(dAng);
			double dInterY = 10 + dRad + dRad*sin(dAng);
			m_pViewPort->DrawLinePoint2D(50 + m_pLeadOut->GetOverCut() / m_pLeadOut->GetLength() * dLen, 
									   dInterY + dLineLen * cos(dAng), dInterX, dInterY);
			m_pViewPort->DrawArc2D(50 + m_pLeadOut->GetOverCut() / m_pLeadOut->GetLength() * dLen, 
								   10 + dRad, dRad, 3*PID2, PID2 + dAng);

			break;
		}
	}
}

END_CUTLEADER_NAMESPACE()
