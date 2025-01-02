#pragma once

#include "drawWrapperCommon.h"
#include <vector>


BEGIN_CUTLEADER_NAMESPACE()


// 三种绘图模式。
typedef enum tagRopMode
{
	ROP_NORMAL		= 0,

    // 在开启逻辑运算后才能用下面两个状态，颜色异或或颜色拷贝，颜色拷贝就是用新颜色覆盖旧颜色。除这两个状态之外就是NORMAL方式绘制。
	ROP_XOR			= 1,
	ROP_COPY		= 2,
} ROP_MODE;

// OpenGL视口。
class ClDrawWrapper_Export GlViewPort
{
public:
	GlViewPort(void);
	~GlViewPort(void);

public:
	// init the view port.
	bool InitEnv(HWND hWnd, double dMinZoomRatio, double dMaxZoomRatio);

	// bind the OpenGL rendering context with device context.
	bool BindRendContext();


	/************************************************************************/
	// about the drawing area.

	// set the drawing area.
	/* params:
	*  dXWorldRange: the drawing area range at X-direction, in world coordinate.
	*  dClientWidth, dClientHeight: the size of drawing area, in pixel.
	*  leftBottomPt: the left bottom point of the drawing area, in world coordinate.
	*/
	bool SetDrawingArea(double dXWorldRange, double dClientWidth, double dClientHeight, double dLeftBottomPtX, double dLeftBottomPtY);

	// 得到绘图区域信息。
	void GetDrawingArea(double& dXWorldRange, double& dClientWidth, double& dClientHeight, double& dLeftBottomPtX, double& dLeftBottomPtY) const;

	// 设置客户区的大小。
	// 注：
	//  1) 一般在窗口尺寸变化时调用这个函数。
	//  2) 这个函数不改变窗口左下角点的坐标，也不改变“缩放率”，所以图形位置没有动，也没有产生缩放。
	bool SetClientSize(double dScreenLength, double dScreenHeight);

	// set the left bottom point of the drawing area.
	/* params:
	*  leftBtmPt: the left bottom point of the drawing area, in world coordinate.
	*/
	bool SetLeftBottomPt(double dLeftBottomPtX, double dLeftBottomPtY);

	// get the "fit all" X-range and the left bottom point of the drawing area, in world coordinate.
	/* params:
	*  dClientWidth, dClientHeight: the drawing area, in pixel.
	*  dGeomLeft, dGeomRight, dGeomTop, dGeomBottom: the boundary rect of all the geometry items.
	*  dMarginRatio: the margin ratio between the geometry border and the window.
	*  leftBottomPt: the left bottom point of the drawing area, in world coordinate.
	*/
	// notes:
	//   1) return the drawing area range at X-direction, in world coordinate.
	static double GetFitAllParam(double dClientWidth, double dClientHeight, double dGeomLeft, double dGeomRight, double dGeomBottom, double dGeomTop,
		double dMarginRatio, double& dLeftBottomPtX, double& dLeftBottomPtY);

	// zoom the view port.
	/* params:
	*  dZoomFactor: the zoom factor.
	*  dClientX, dClientY: the base point of client area(pixel unit).
	*/
	bool ZoomViewPort(double dZoomFactor, double dClientX, double dClientY);

	// pan the view port.
	/* params:
	*  dXClientOffSet, dYClientOffSet: the pan offset(pixel unit).
	*/
	bool PanViewPort(double dXClientOffSet, double dYClientOffSet);
	/************************************************************************/


	// clear the screen.
	bool ClearScreen();

	// set the background color of the drawing area.
	bool SetBackgroundColor(COLORREF dwBkColor);


	/************************************************************************/
	// coordinate transform between device coordinate and world coordinate.

	void WToD(double& dPtX, double& dPtY) const;
	void DToW(double& dPtX, double& dPtY) const;

	void WToD(double& dLength) const;
	void DToW(double& dLength) const;
	/************************************************************************/


	/************************************************************************/
	// about the drawing params.
    // 注：
    //  1）该类使用RGBA颜色模式，所以设置逻辑操作时使用GL_COLOR_LOGIC_OP，而不能使用GL_LOGIC_OP（这个值用于颜色索引模式）。

	// get logical pixel operation for rendering.
	bool GetROP(ROP_MODE& ropMode) const;

	// set logical pixel operation for rendering.
	bool SetROP(ROP_MODE ropMode);

	// get the drawing color.
	bool GetDrawColor(COLORREF& dwDrawColor) const;

	// set the drawing color.
	bool SetDrawColor(COLORREF dwDrawColor);

	// get the width of the line.
	int GetLineWidth() const;

	// set drawing line width.
	bool SetLineWidth(int iWidth);

	// get the mode which will be used to draw the stipple.
	bool GetLineStipple(bool& bStipple, int& iRepeat, unsigned short& iPattern) const;

	// set the mode which will be used to draw the stipple.
	bool SetLineStipple(bool bStipple, int iRepeat, unsigned short iPattern);
	/************************************************************************/


	/************************************************************************/
	// about drawing basic geometry.

	// draw the coordinate.
	BOOL DrawCoordinate(double dOrgX, double dOrgY, double dAngle, BOOL bInverse);

	// draw point.
	BOOL DrawPoint2D(double dX, double dY, double dSize = 1.0);

	// draw line.
	// notes:
	//   1) in "DrawLine2D2" function, "dLength" will be pixel unit.
	BOOL DrawLine2D(double dX1, double dY1, double dLineLength, double dLineAngle);
	BOOL DrawLine2D2(double dStartX, double dStartY, double dLength, double dAng);
	BOOL DrawLinePoint2D(double dX1, double dY1, double dX2, double dY2);

	// draw cross line.
	// notes:
	//   1) "dSize" will be pixel unit.
	//   2) the line width always be 1.
	BOOL DrawCrossPoint2D(double dX, double dY, double dSize, double dAngle);
	// 注：
	//   1) 这个函数不设置线宽。 
	BOOL DrawCrossPoint2DEx(double dX, double dY, double dSize, double dAngle);

	// draw circle.
	BOOL DrawCircle2D(double dX, double dY, double dRadius);

    // draw the circle which can be filled.
	// notes:
	//  1) "dSize" will be pixel unit.
	//  2) if "bFill" is TRUE, the circle will be filled.
	//  3) if "bFill" is FALSE, use the line width of current env.
    //  4) bBeRound: 为true时，离散的接近圆。在虚拟机下面时，建议用false，否则显示有问题。
	BOOL DrawRoundPoint2D(double dX, double dY, double dSize, BOOL bFill, bool bBeRound);

	// draw arc.
	BOOL DrawArc2D(double dX, double dY, double dRad, double dStartAng, double dMoveAng);

	// draw ellipse.
	BOOL DrawEllipse2D(double dX, double dY, double dLongRad, double dShortRad, double dAng);

	// draw rect.
	BOOL DrawRect(double dLeft, double dTop, double dRight, double dBottom);
	// notes:
	//   1) "dSize" will be pixel unit.
	//   2) if "bFill" is TRUE, the rect will be filled.
	//   3) if "bFill" is FALSE, use the line width of current env.
	BOOL DrawRectPoint2D(double dX, double dY, double dSize, BOOL bFill, bool bDebugging);

	// 绘制轮廓。
	void DrawLoop(const std::vector<std::pair<double, double>>& loop); // loop可以是开放轮廓。
	BOOL DrawPolygon2D(double* pX, double* pY, int iNum);

	// draw triangle.
	BOOL DrawTriangle(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3);

	// draw text.
	BOOL DrawString(std::wstring strContent, double dLeftTopX, double dLeftTopY, double dRightBottomX, double dRightBottomY);
	/************************************************************************/


	// get the cursor position.
	void GetCursorPos(double& d1, double& d2) const;

	// swap the buffer of OpenGL.
	bool SwapBuffers();

	// get the associated window handle.
	HWND GetWnd() const { return m_hWnd; }

	// release the resource.
	void ReleaseResource();

private:
	// find from indexes.
	UCHAR FindFromIndex(int iIndex, UINT ibits, UINT iShift) const;

	// set the projection for OpenGL.
	bool SetProjection();

private:
	// the drawing window and the drawing area(in pixel).
	HWND m_hWnd;
	double m_dClientWidth;
	double m_dClientHeight;

	// the zoom ratio of the device/world coordinate system.
	double m_dZoomRatio;

	// the left bottom of drawing area(in world coordinate).
	double m_dLeftBottomPtX;
	double m_dLeftBottomPtY;

	// min/max zoom ratio.
	double m_dMinZoomRatio;
	double m_dMaxZoomRatio;

	// OpenGL rendering context and the current device context.
	HGLRC m_hRC;
	HDC m_hDC;

	// the palette objects.
	HPALETTE m_hPalette;
	HPALETTE m_hOldPalette;

	// the display list.
	ULONG m_iStartList;
};

END_CUTLEADER_NAMESPACE()
