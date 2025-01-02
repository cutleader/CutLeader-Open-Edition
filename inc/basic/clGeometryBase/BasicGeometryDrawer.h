#pragma once

#include "clGeometryBaseCommon.h"
#include "Matrix2D.h"
#include "Rect2D.h"


DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(Triangle2D)
DECLARE_CUTLEADER_CLASS(LineArc2DList)


BEGIN_CUTLEADER_NAMESPACE()


// we wrapped some basic draw functions in this class.
// notes: 
//   1) we only draw line/arcs in this class.
class ClGeometryBase_Export BasicGeometryDrawer
{
public:
	BasicGeometryDrawer(GlViewPortPtr pViewPort);
	~BasicGeometryDrawer(void);

public: // get/set function.
	GlViewPortPtr GetDrawer() const { return m_pViewPort; }

public:
	// draw 2d line/arcs.
	void Draw2DLineArcs(const LineArc2DList* pGeom2DList, const Matrix2D& mat = Matrix2D());
	void Draw2DLineArc(const ILineArc2D* pGeom2D, const Matrix2D& mat = Matrix2D());

	// draw 2d line.
	void DrawLine2D(const Line2D* pLine2D, const Matrix2D& mat = Matrix2D());

	// draw 2d arc.
	void DrawArc2D(const Arc2D* pArc2D, const Matrix2D& mat = Matrix2D());

	// draw the triangle.
	void DrawTriangle(const Triangle2D* pTriangle2D, const Matrix2D& mat = Matrix2D());

	// draw point.
	void DrawPoint2D(const Point2D& pt, const Matrix2D& mat = Matrix2D());

protected:
	// get the rect of the client area.
	// notes:
	//   1) we should convert it to world coordinate.
	Rect2D GetClientAreaRect();

	// convert the rect2D from world coordinate to device coordinate.
	Rect2D RectWToD(const Rect2D& rect);

protected:
	GlViewPortPtr m_pViewPort;
};

END_CUTLEADER_NAMESPACE()
