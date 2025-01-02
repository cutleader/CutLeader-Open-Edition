#include "StdAfx.h"
#include "CornerBoxWnd.h"

#include "GDIDrawer.h"

BEGIN_CUTLEADER_NAMESPACE()

CornerBoxWnd::CornerBoxWnd(void)
{
}

CornerBoxWnd::~CornerBoxWnd(void)
{
}

BEGIN_MESSAGE_MAP(CornerBoxWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()

END_MESSAGE_MAP()

BOOL CornerBoxWnd::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CornerBoxWnd::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rect;
	GetClientRect( rect );
	int width = rect.Width();
	int height = rect.Height();

	CDC	memDC;
	memDC.CreateCompatibleDC( &dc );
	CBitmap	bitmap;
	bitmap.CreateCompatibleBitmap( &dc, width, height );
	CBitmap* oldbitmap = memDC.SelectObject( &bitmap );

	memDC.SelectObject( GDIDrawer::dialogPen() );
	memDC.SelectObject( GDIDrawer::dialogBrush() );
	memDC.Rectangle( rect );

	rect.InflateRect( -2, -2 );
	GDIDrawer::draw3dFrame( &memDC, rect );

	dc.BitBlt( 0, 0, width, height, &memDC, 0, 0, SRCCOPY );
	memDC.SelectObject( oldbitmap );
	memDC.SelectStockObject( NULL_PEN );
	memDC.SelectStockObject( NULL_BRUSH );
}

END_CUTLEADER_NAMESPACE()
