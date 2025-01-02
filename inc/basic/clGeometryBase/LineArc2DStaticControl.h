#pragma once

#include "clGeometryBaseCommon.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(LineArc2DList)

BEGIN_CUTLEADER_NAMESPACE()

// the static control for displaying 2d line and arc, 用于显示二维直线圆弧的静态控件。
class ClGeometryBase_Export LineArc2DStaticControl : public CStatic
{
	DECLARE_DYNAMIC(LineArc2DStaticControl)

public:
	LineArc2DStaticControl(void);
	~LineArc2DStaticControl(void);

public:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

public: // get/set functions.
	void SetPanReferPt(const CPoint& pt) { m_referPt = pt; }
	CPoint GetPanReferPt() const { return m_referPt; }

	void SetSolidGeom2DList(LineArc2DListPtr pSolidGeom2DList) { m_pSolidGeom2DList = pSolidGeom2DList; }
	void SetDashGeom2DList(LineArc2DListPtr pDashGeom2DList) { m_pDashGeom2DList = pDashGeom2DList; }
	void SetSelGeom2DList(LineArc2DListPtr pSelGeom2DList) { m_pSelGeom2DList = pSelGeom2DList; }

public:
	// 初始化视口。
	// 注：
	//   1) 该函数会清空要绘制的图形。
	void InitViewPort(const Rect2D& geomRect);

	// zoom the view port.
	void ZoomViewPort(short iDelta, const CPoint& screenPt);

	// pan the view port.
	void PanViewPort(double dXClientOffSet, double dYClientOffSet);

	// 调整视口以适合矩形框。
	void FitRect(const Rect2D& geomRect);

	// 释放视口。
	void ReleaseViewPort();

	void SetBackgroundColor(COLORREF dwBackgroundColor) { m_dwBackgroundColor = dwBackgroundColor; }
	COLORREF GetBackgroundColor() const { return m_dwBackgroundColor; }

	void SetDrawColor(COLORREF dwDrawColor) { m_dwDrawColor = dwDrawColor; }
	COLORREF GetDrawColor() const { return m_dwDrawColor; }

private:
	// OpenGL视口。
	GlViewPortPtr m_pViewPort;
	COLORREF m_dwBackgroundColor;
	COLORREF m_dwDrawColor;

	// the reference point for pan the view port.
	CPoint m_referPt;

	// 要绘制的实线图形。
	// 注：
	//   1) 实线，线宽为1，白色。
	LineArc2DListPtr m_pSolidGeom2DList;

	// 要绘制的虚线图形。
	// 注：
	//   1) 虚线，线宽为1，红色。
	LineArc2DListPtr m_pDashGeom2DList;

	// 要绘制的被选中图形。
	// 注：
	//   1) 实线，线宽为2，黄色。
	LineArc2DListPtr m_pSelGeom2DList;
};

END_CUTLEADER_NAMESPACE()
