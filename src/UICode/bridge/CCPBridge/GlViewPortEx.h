#pragma once

#include "ccpBridgeCommon.h"
#include "Point2DEx.h"
#include "Rect2DEx.h"
#include "LineItemEx.h"
#include "ArcItemEx.h"
#include "GeomItemListEx.h"
#include "Polygon2DEx.h"
#include <string>

/************************************************************************/
// declare class.
DECLARE_CUTLEADER_CLASS(GlViewPort)
/************************************************************************/

BEGIN_CUTLEADER_NAMESPACE()

[ComVisible(true)]
public enum class ROP_MODE_EX
{
	ROP_EX_NORMAL	= 0,
	ROP_EX_XOR		= 1,
	ROP_EX_COPY		= 2,
};

[ClassInterface(ClassInterfaceType::AutoDual)]
[ComVisible(true)]
public ref class GlViewPortEx
{
public:
	GlViewPortEx(void);
	!GlViewPortEx();

public:
	Boolean InitEnv(IntPtr hWnd, double dMinZoomRatio, double dMaxZoomRatio);
	Boolean BindRendContext();

	Boolean SetDrawingArea(double dXWorldRange, double dClientWidth, double dClientHeight, Point2DEx^ leftBottomPt);
	void GetDrawingArea(double% dXWorldRange, double% dClientWidth, double% dClientHeight, double% dLeftBottomPtX, double% dLeftBottomPtY);
	Boolean SetLeftBottomPt(Point2DEx^ leftBottomPt);
	double GetFitAllParam(double dClientWidth, double dClientHeight, Rect2DEx^ geomRect, double dMarginRatio, Point2DEx^ leftBottomPt);
	Boolean ZoomViewPort(double dZoomFactor, double dClientX, double dClientY);
	Boolean PanViewPort(double dXClientOffSet, double dYClientOffSet);

	Boolean ClearScreen();

	Boolean SetBackgroundColor(Color color);

	void WToD(Double% dPtX, Double% dPtY);
	void DToW(Double% dPtX, Double% dPtY);
	void WToD(Double% dLength);
	void DToW(Double% dLength);

	Boolean GetROP(ROP_MODE_EX% ropMode);
	Boolean SetROP(ROP_MODE_EX ropMode);
	bool GetDrawColor(Color% color);
	Boolean SetDrawColor(Color color);
	Int32 GetLineWidth();
	Boolean SetLineWidth(Int32 iWidth);
	Boolean GetLineStipple(Boolean% bStipple, Int32% iRepeat, UInt16% iPattern);
	Boolean SetLineStipple(Boolean bStipple, Int32 iRepeat, UInt16 iPattern);

	void DrawLineItem(LineItemEx^ lineItem);
	void DrawArcItem(ArcItemEx^ arcItem);
    void DrawGeomItem(GeomItemEx^ geomItem);
	void DrawWord(String^ strContent, double dLeftTopX, double dLeftTopY, double dRightBottomX, double dRightBottomY);
	void DrawCoordinate(double dOrgX, double dOrgY, double dAngle, bool bInverse);
    void DrawCross(double dX, double dY, double dSize, double dAngle);
	void DrawPoint(double dX, double dY, double dSize);
	void DrawPath(List<Tuple<double, double>^>^ ptList);

	Point2DEx^ GetCursorPos();

	bool SwapBuffers();

	void ReleaseResource();

public:
	GlViewPortPtr GetGlViewPort() { return *m_ppGlViewPort; }

private:
	GlViewPortPtr* m_ppGlViewPort;
};

END_CUTLEADER_NAMESPACE()
