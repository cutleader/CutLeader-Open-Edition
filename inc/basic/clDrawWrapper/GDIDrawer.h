#pragma once

#include "drawWrapperCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClDrawWrapper_Export GDIDrawer
{
public:
	static CPen* shadowPen();
	static CPen* darkshadowPen();
	static CPen* lightPen();
	static CPen* highlightPen();
	static CPen* dialogPen();
	static CPen* windowPen();
	static CPen* scrollPen();
	static CBrush* dialogBrush();
	static CBrush* windowBrush();
	static CBrush* scrollBrush();

	static void drawframed3dBox( CDC* dc, CRect rect );
	static void draw3dFrame( CDC* dc, CRect rect );
	static void drawsunken3dFrame( CDC* dc, CRect rect );
	static void drawdoublesunken3dFrame( CDC* dc, CRect rect );
	static void drawsunkenframed3dWindow( CDC* dc, CRect rect );

private:
	static CPen s_shadowPen;
	static CPen s_darkshadowPen;
	static CPen s_lightPen;
	static CPen s_highlightPen;
	static CPen s_dialogPen;
	static CPen s_windowPen;
	static CPen s_scrollPen;
	static CBrush s_dialogBrush;
	static CBrush s_backgroundBrush;
	static CBrush s_windowBrush;
	static CBrush s_scrollBrush;
};

END_CUTLEADER_NAMESPACE()
