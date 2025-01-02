#pragma once

#include "drawWrapperCommon.h"
#include "MemDC.h"
#include <gdiplus.h>
#include <map>

BEGIN_CUTLEADER_NAMESPACE()

typedef enum tagMSDrawSmoothingMode
{
	SMOOTHINGMODE_INVALID = -1,
	SMOOTHINGMODE_DEFAULT = 0,
	SMOOTHINGMODE_HIGHSPEED = 1,
	SMOOTHINGMODE_HIGHQUALITY = 2,
	SMOOTHINGMODE_NONE,
	SMOOTHINGMODE_ANTIALIAS8X4,
	SMOOTHINGMODE_ANTIALIAS = SMOOTHINGMODE_ANTIALIAS8X4,
	SMOOTHINGMODE_ANTIALIAS8X8
} DRAW_SMOOTHINGMODE;

typedef enum tagMSDrawBrushType
{
	BRUSHTYPE_SOLID = 0,
	BRUSHTYPE_HATCH = 1,
	BRUSHTYPE_TEXTURE = 2,
	BRUSHTYPE_LINEAR_GRADIENT = 3,
	BRUSHTYPE_PATH_GRADIENT = 4
} DRAW_BRUSHTYPE;

typedef enum tagMSDrawBrushHatchStyle
{
	HATCHSTYLEHORIZONTAL = 0,
	HATCHSTYLEVERTICAL = 1,
	HATCHSTYLEFORWARDDIAGONAL = 2,
	HATCHSTYLEBACKWARDDIAGONAL = 3,
	HATCHSTYLECROSS = 4,
	HATCHSTYLEDIAGONALCROSS = 5,
	HATCHSTYLE05PERCENT = 6,
	HATCHSTYLE10PERCENT = 7,
	HATCHSTYLE20PERCENT = 8,
	HATCHSTYLE25PERCENT = 9,
	HATCHSTYLE30PERCENT = 10,
	HATCHSTYLE40PERCENT = 11,
	HATCHSTYLE50PERCENT = 12,
	HATCHSTYLE60PERCENT = 13,
	HATCHSTYLE70PERCENT = 14,
	HATCHSTYLE75PERCENT = 15,
	HATCHSTYLE80PERCENT = 16,
	HATCHSTYLE90PERCENT = 17,
	HATCHSTYLELIGHTDOWNWARDDIAGONAL = 18,
	HATCHSTYLELIGHTUPWARDDIAGONAL = 19,
	HATCHSTYLEDARKDOWNWARDDIAGONAL = 20,
	HATCHSTYLEDARKUPWARDDIAGONAL = 21,
	HATCHSTYLEWIDEDOWNWARDDIAGONAL = 22,
	HATCHSTYLEWIDEUPWARDDIAGONAL = 23,
	HATCHSTYLELIGHTVERTICAL = 24,
	HATCHSTYLELIGHTHORIZONTAL = 25,
	HATCHSTYLENARROWVERTICAL = 26,
	HATCHSTYLENARROWHORIZONTAL = 27,
	HATCHSTYLEDARKVERTICAL = 28,
	HATCHSTYLEDARKHORIZONTAL = 29,
	HATCHSTYLEDASHEDDOWNWARDDIAGONAL = 30,
	HATCHSTYLEDASHEDUPWARDDIAGONAL = 311,
	HATCHSTYLEDASHEDHORIZONTAL = 32,
	HATCHSTYLEDASHEDVERTICAL = 33,
	HATCHSTYLESMALLCONFETTI = 34,
	HATCHSTYLELARGECONFETTI = 35,
	HATCHSTYLEZIGZAG = 36,
	HATCHSTYLEWAVE = 37,
	HATCHSTYLEDIAGONALBRICK = 38,
	HATCHSTYLEHORIZONTALBRICK = 39,
	HATCHSTYLEWEAVE = 40,
	HATCHSTYLEPLAID = 41,
	HATCHSTYLEDIVOT = 42,
	HATCHSTYLEDOTTEDGRID = 43,
	HATCHSTYLEDOTTEDDIAMOND = 44,
	HATCHSTYLESHINGLE = 45,
	HATCHSTYLETRELLIS = 46,
	HATCHSTYLESPHERE = 47,
	HATCHSTYLESMALLGRID = 48,
	HATCHSTYLESMALLCHECKERBOARD = 49,
	HATCHSTYLELARGECHECKERBOARD = 50,
	HATCHSTYLEOUTLINEDDIAMOND = 51,
	HATCHSTYLESOLIDDIAMOND = 52,
	HATCHSTYLETOTAL = 53,
	HATCHSTYLELARGEGRID = HATCHSTYLECROSS,
	HATCHSTYLEMIN = HATCHSTYLEHORIZONTAL,
	HATCHSTYLEMAX = HATCHSTYLETOTAL - 1
} DRAW_BRUSHHATCHSTYLE;

typedef enum tagMSDrawLineCap
{
	LINECAP_FLAT			= 0,
	LINECAP_SQUARE			= 1,
	LINECAP_ROUND			= 2,
	LINECAP_TRINAGLE		= 3,
	LINECAP_NOANCHOR		= 0x10,
	LINECAP_SQUAREANCHOR	= 0x11,
	LINECAP_ROUNDANCHOR		= 0x12,
	LINECAP_DIAMONDANCHOR	= 0x13,
	LINECAP_ARROW			= 0x14,
	LINECAP_FWARROW			= 0x15
} DRAW_LINECAP;

// GDI+视口。
class ClDrawWrapper_Export GplusViewPort
{
public:
	GplusViewPort(void);
	~GplusViewPort(void);

public: // implement interface IDrawer.
	/************************************************************************/
	// about init the environment.

	// init the environment.
	BOOL Initialize(HWND hWnd);

	// 用设备上下文来初始化。
	BOOL Initialize(HDC hDC, int cx, int cy, BOOL bDrawToDC);

	// set the view port.
	BOOL SetViewPort(double dWorldRange, double dClientWidth, double dClientHeight, 
					 double dLeftBottomX, double dLeftBottomY);

	// set the left bottom of world coordinate system.
	BOOL SetLeftBottomPt(double dX, double dY);

	// get the "fit all" X-range and the left bottom point of the drawing area, in world coordinate.
	/* params:
	*  dClientWidth, dClientHeight: the drawing area, in pixel.
	*  dGeomLeft, dGeomRight, dGeomTop, dGeomBottom: the boundary rect of all the geometry items.
	*  dMarginRatio: the margin ratio between the geometry border and the window.
	*  leftBottomPt: the left bottom point of the drawing area, in world coordinate.
	*/
	// notes:
	//   1) return the drawing area range at X-direction, in world coordinate.
	static double GetFitAllParam(double dClientWidth, double dClientHeight, 
								 double dGeomLeft, double dGeomRight, double dGeomBottom, double dGeomTop,
								 double dMarginRatio, double& dLeftBottomPtX, double& dLeftBottomPtY);
	/************************************************************************/

	/************************************************************************/
	// about drawing.

	BOOL SetDrawColor(COLORREF dwDrawColor);

	BOOL SetBackGroundColor(COLORREF dwBkColor);

	// set the width of the line.
	BOOL SetLineWidth(int iWidth);

	// set the mode which will be used to draw the stipple.
	BOOL SetLineStipple(BOOL bStipple, int iRepeat, unsigned short iPattern);

	BOOL DrawLine2D(double dX1, double dY1, double dLineLength, double dLineAngle);
	BOOL DrawLinePoint2D(double dX1, double dY1, double dX2, double dY2);

	// draw arc.
	BOOL DrawArc2D(double dX, double dY, double dRad, double dStartAng, double dMoveAng);

	// draw round point.
	BOOL DrawRoundPoint2D(double dX, double dY, double dSize, BOOL bFill);

	// 画文字。
	BOOL DrawText(double dX, double dY, CString str, LOGFONT logFont);
	/************************************************************************/

	/************************************************************************/
	// about coordinate transform.

	BOOL WToD(double& dPtX, double& dPtY) const;
	BOOL DToW(double& dPtX, double& dPtY) const;

	BOOL WToD(double& dLength) const;
	BOOL DToW(double& dLength) const;
	/************************************************************************/

	// clear the screen.
	BOOL ClearScreen();

	// swap the buffer of OpenGL.
	BOOL SwapBuffers();

public:
	BOOL SetSmoothingMode(DRAW_SMOOTHINGMODE newVal);
	BOOL SetPointMap(double DPtX, double DPtY, double WPtX, double WPtY);
	BOOL SetLineCap(DRAW_LINECAP StartCap, DRAW_LINECAP EndCap);

	MemDC* GetMemDC() { return &m_MemDC; }

private:
	BOOL InitGDIPlusBrush(void);

	BOOL CheckZoomRatio(double& ZoomRatio);
	BOOL SetGDIPlusPenColor();

	BOOL SetGDIPlusBrushColor();
	BOOL GetCurGDIPlusBrush(Gdiplus::Brush** ppBrush);

	BOOL SetGDIPlusBurshHatchAtrribute();
	BOOL SetArrowLineCap(bool bIsStartCap = false);

	void ConvertShortPatternToArray(unsigned short Pattern, int iRepeat, Gdiplus::REAL *pArray, int& nRetCount);

private:
	BYTE m_ColorAlphaVal;
	BYTE m_DrawColorR;
	BYTE m_DrawColorG;
	BYTE m_DrawColorB;

	Gdiplus::GdiplusStartupInput m_GDIPlusStartupInput;
	ULONG_PTR m_GDIPlusToken;

	Gdiplus::SmoothingMode m_SmoothingMode;
	Gdiplus::LineCap m_LineStartCap;
	Gdiplus::LineCap m_LineEndCap;

	Gdiplus::Graphics* m_pGraphic;
	Gdiplus::Pen* m_pPen;
	Gdiplus::Color m_colorBrushHatchBack;
	DRAW_BRUSHTYPE m_curBrushType;
	DRAW_BRUSHHATCHSTYLE m_curBrushHatchStyle;
	std::map<DRAW_BRUSHTYPE, Gdiplus::Brush*> m_mapBrush;
	float m_FWArrowWidthScale;

protected:
	// the window where we draw.
	HWND m_hWnd;

	BOOL m_bInitWithDC;

	// the range of the world coordinate system.
	double m_dWorldRange;

	// the size of the client area(in pixel unit).
	double m_dClientWidth;
	double m_dClientHeight;

	// the left bottom of world coordinate system.
	double m_dLeftBottomX;
	double m_dLeftBottomY;

	// the ratio of the device/world coordinate system.
	// notes:
	//   1) after zoom in/out, this value will be changed.
	double m_dZoomRatio;

	// min zoom ratio.
	double m_dMinZoomRatio;

	// max zoom ratio.
	double m_dMaxZoomRatio;

	int	m_ScreenCX;
	int m_ScreenCY;

	HDC m_hDC;
	MemDC m_MemDC;
	COLORREF m_BKColor;

	int m_LineWidth;

	BOOL m_bDrawToDC;
};

END_CUTLEADER_NAMESPACE()
