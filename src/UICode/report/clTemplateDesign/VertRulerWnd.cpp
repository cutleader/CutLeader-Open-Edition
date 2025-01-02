#include "StdAfx.h"
#include "VertRulerWnd.h"

#include <math.h>
#include "GDIDrawer.h"

#include "DisplayParam.h"
#include "TemplateParam.h"

BEGIN_CUTLEADER_NAMESPACE()

VertRulerWnd::VertRulerWnd(void)
{
	m_measurements = TPL_UNIT_PIXEL;
	m_startPos = 0;
	m_zoom = 1;
	m_pointerPos = -1;
}

VertRulerWnd::~VertRulerWnd(void)
{
}

BEGIN_MESSAGE_MAP(VertRulerWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

void VertRulerWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect( rect );
	int width = rect.Width();
	int height = rect.Height();

	// Create a memory CDC
	CDC	memDC;
	memDC.CreateCompatibleDC( &dc );
	CBitmap	bitmap;
	bitmap.CreateCompatibleBitmap( &dc, width, height );
	CBitmap* oldbitmap = memDC.SelectObject( &bitmap );

	memDC.FillSolidRect( rect, RGB( 255, 255, 255 ) );

	// Draw the scale
	switch( GetMeasurements() )
	{
		case TPL_UNIT_PIXEL:
			DrawPixelScale( &memDC, rect );
			break;
		case TPL_UNIT_INCH:
			DrawInchScale( &memDC, rect );
			break;
		case TPL_UNIT_CM:
			DrawCentimeterScale( &memDC, rect );
			break;
	}

	// The frame
	memDC.SelectObject( GDIDrawer::darkshadowPen() );
	memDC.SelectStockObject( NULL_BRUSH );
	memDC.Rectangle( rect );

	memDC.SelectObject( GDIDrawer::dialogPen() );
	rect.InflateRect( -1, -1 );
	memDC.Rectangle( rect );
	rect.InflateRect( -1, -1 );
	memDC.Rectangle( rect );

	rect.InflateRect( -1, -1 );
	memDC.SelectObject( GDIDrawer::shadowPen() );
	memDC.Rectangle( rect );

	memDC.SelectStockObject( BLACK_PEN );

	if( GetPointerPos() != -1 )
	{
		GetClientRect( rect );
		CRect rc( rect.right - 8, GetPointerPos() - 2, rect.right - 3, GetPointerPos() + 2 );
		memDC.MoveTo( rc.TopLeft() );
		memDC.LineTo( rc.right, rc.top + rc.Height() / 2 );
		memDC.LineTo( rc.left, rc.bottom );
		memDC.LineTo( rc.TopLeft() );
	}

	dc.BitBlt( 0, 0, width, height, &memDC, 0, 0, SRCCOPY );
	memDC.SelectObject( oldbitmap );
}

BOOL VertRulerWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void VertRulerWnd::SetStartPos( int startPos )
{
	if( m_startPos != startPos )
	{
		m_startPos = startPos;
		SetPointerPos( -1 );
		if( m_hWnd )
			RedrawWindow();
	}
}

void VertRulerWnd::SetMeasurements(TPL_UNIT_TYPE measurements)
{
	if( measurements != m_measurements )
	{
		m_measurements = measurements;
		SetPointerPos( -1 );
		if( m_hWnd )
			RedrawWindow();
	}
}

void VertRulerWnd::SetZoom( double zoom )
{
	if( m_zoom != zoom )
	{
		m_zoom = zoom;
		SetPointerPos( -1 );
		if( m_hWnd )
			RedrawWindow();
	}
}

void VertRulerWnd::SetPointerPos( int pointerPos )
{
	if( m_pointerPos != pointerPos )
	{
		m_pointerPos = pointerPos;
		if( m_hWnd )
			RedrawWindow();
	}
}

void VertRulerWnd::DrawPixelScale( CDC* dc, CRect rect )
{
	DrawScale( dc, rect, 100, 10 );
}

void VertRulerWnd::DrawInchScale( CDC* dc, CRect rect )
{
	double seg = static_cast< double >( dc->GetDeviceCaps( LOGPIXELSY ) );
	DrawScale( dc, rect, seg, 8 );
}

void VertRulerWnd::DrawCentimeterScale( CDC* dc, CRect rect )
{
	double seg = static_cast< double >( dc->GetDeviceCaps( LOGPIXELSY ) ) / 2.54;
	DrawScale( dc, rect, seg, 2 );
}

void VertRulerWnd::DrawScale( CDC* dc, CRect rect, double seg, double stepno )
{
	int segment = (int)( seg * GetZoom() );
	double step = static_cast< double >( segment ) / stepno;

	int count = 0;
	for( int t = -GetStartPos() ; t < rect.bottom ; t += segment )
	{
		for( int i = 0 ; i < static_cast< int >( stepno ); i ++ )
		{
			double pos = t + static_cast< double >( i ) * step;
			dc->MoveTo( rect.left + 11, (int)pos);
			dc->LineTo( rect.right - 11, (int)pos);
		}

		if( t >= rect.top )
		{
			CString str;
			str.Format( _T( "%d" ), count );

			dc->MoveTo( rect.left + 3, t );
			dc->LineTo( rect.left + 5, t );

			CFont font;
			int height = rect.Width() - 14;
			font.CreateFont( -height, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );
			dc->SelectObject( &font );
			int hgt = dc->GetTextExtent( str ).cy;
			CRect text( rect.left + 3, t - hgt / 2, rect.right - 3, t + hgt / 2 );
			dc->DrawText( str, text, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			dc->SelectStockObject( ANSI_VAR_FONT );

			dc->MoveTo( rect.right - 5, t );
			dc->LineTo( rect.right - 3, t );
		}

		count++;
	}
}

END_CUTLEADER_NAMESPACE()
