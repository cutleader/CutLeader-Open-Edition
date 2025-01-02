#include "StdAfx.h"
#include "HorzRulerWnd.h"

#include "GDIDrawer.h"

#include "DisplayParam.h"

BEGIN_CUTLEADER_NAMESPACE()

HorzRulerWnd::HorzRulerWnd(void)
{
	m_measurements = TPL_UNIT_PIXEL;
	m_startPos = 0;
	m_dZoom = 1;
	m_pointerPos = -1;
}

HorzRulerWnd::~HorzRulerWnd(void)
{
}

BEGIN_MESSAGE_MAP(HorzRulerWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()

END_MESSAGE_MAP()

BOOL HorzRulerWnd::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void HorzRulerWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect( rect );
	int width = rect.Width();
	int height = rect.Height();

	// Create memory CDC
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

	// Draw frame
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

	// Pointer
	if( GetPointerPos() != -1 )
	{
		GetClientRect( rect );
		CRect rc( GetPointerPos() - 2, rect.bottom - 8, GetPointerPos() + 2, rect.bottom - 3 );
		memDC.MoveTo( rc.TopLeft() );
		memDC.LineTo( rc.right, rc.top );
		memDC.LineTo( rc.left + rc.Width() / 2, rc.bottom );
		memDC.LineTo( rc.TopLeft() );
	}

	dc.BitBlt( 0, 0, width, height, &memDC, 0, 0, SRCCOPY );
	memDC.SelectObject( oldbitmap );
}

void HorzRulerWnd::SetStartPos( int startPos )
{
	if( m_startPos != startPos )
	{
		m_startPos = startPos;
		SetPointerPos( -1 );
		if( m_hWnd )
			RedrawWindow();
	}
}

void HorzRulerWnd::SetMeasurements(TPL_UNIT_TYPE measurements)
{
	if (measurements != m_measurements)
	{
		m_measurements = measurements;
		SetPointerPos( -1 );
		if( m_hWnd )
			RedrawWindow();
	}
}

void HorzRulerWnd::SetZoom( double dZoom )
{
	if( m_dZoom != dZoom )
	{
		m_dZoom = dZoom;
		SetPointerPos( -1 );
		if( m_hWnd )
			RedrawWindow();
	}
}

void HorzRulerWnd::SetPointerPos( int pointerPos )
{
	if( m_pointerPos != pointerPos )
	{
		m_pointerPos = pointerPos;
		if( m_hWnd )
			RedrawWindow();
	}
}

void HorzRulerWnd::DrawPixelScale( CDC* dc, CRect rect )
{
	DrawScale( dc, rect, 100, 10 );
}

void HorzRulerWnd::DrawInchScale( CDC* dc, CRect rect )
{
	double seg = static_cast< double >( dc->GetDeviceCaps( LOGPIXELSX ) );
	DrawScale( dc, rect, seg, 8 );
}

void HorzRulerWnd::DrawCentimeterScale( CDC* dc, CRect rect )
{
	double seg = static_cast<double>(dc->GetDeviceCaps(LOGPIXELSX)) / 2.54;
	DrawScale( dc, rect, seg, 2 );
}

void HorzRulerWnd::DrawScale( CDC* dc, CRect rect, double seg, double stepno )
{
	int segment = (int)( seg * GetZoom() ) ;
	double step = static_cast< double >( segment ) / stepno;

	int count = 0;
	for( int t = -GetStartPos() ; t < rect.right ; t += segment )
	{
		for( int i = 0 ; i < static_cast< int >( stepno ); i ++ )
		{
			double pos = t + static_cast< double >( i ) * step;
			dc->MoveTo( (int)pos, rect.top + 11 );
			dc->LineTo( (int)pos, rect.bottom - 11 );
		}

		if( t >= rect.left )
		{
			dc->MoveTo( t, rect.top + 5 );
			dc->LineTo( t, rect.bottom - 5 );

			CRect text( t - rect.Height(), rect.top + 5, t + rect.Height(), rect.bottom - 5 );
			CFont font;
			int height = rect.Height() - 14;
			font.CreateFont( -height, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Arial" ) );
			dc->SelectObject( &font );
			CString str;
			str.Format( _T( "%d" ), count );
			dc->DrawText( str, text, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
			dc->SelectStockObject( ANSI_VAR_FONT );
		}

		count++;
	}
}

END_CUTLEADER_NAMESPACE()
