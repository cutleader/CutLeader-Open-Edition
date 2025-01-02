#pragma once

#include "clGeometryBaseCommon.h"
#include "Polygon2DList.h"
#include <vector>


DECLARE_CUTLEADER_CLASS(Rect2D)
DECLARE_CUTLEADER_CLASS(ILineArc2D)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Arc2D)
DECLARE_CUTLEADER_CLASS(Point2D)


BEGIN_CUTLEADER_NAMESPACE()


// 注意，这个值本来定义在clResource工程中的，为了不依赖clResource工程，这里重复定义了一次，后续修改这个值时需要保持一致。
#define IDD_clGeometryBase_PolygonViewer       474

// use this dialog to display the line/arc/polygons.
class ClGeometryBase_Export PolyViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PolyViewDlg)

public:
	PolyViewDlg(CString strCaption = _T(""), CWnd* pParent = NULL, BOOL bDisplayText = TRUE);

	virtual ~PolyViewDlg();

	// Dialog Data
	enum { IDD = IDD_clGeometryBase_PolygonViewer };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

public:
	void AddPoint(Point2DPtr pointPtr, COLORREF color = 0xFF0000);
	void AddLine(Line2DPtr linePtr, COLORREF color = 0xFF0000, int lineNo = -1);
	void AddArc(Arc2DPtr arc, COLORREF color = 0xFF0000);
	void AddGeom(ILineArc2DPtr pGeom, COLORREF color = 0xFF0000);
	void AddRectangle(Rect2DPtr rectPtr, COLORREF color = 0xFF0000);
	void AddPolygon(Polygon2DPtr polygon, COLORREF color = 0xFF0000);

private:
	void DrawCrossPoint2D(CDC& dc, double x, double y);
	void DrawPointText(CDC& dc, double x, double y, int i = -1);
	void DrawPointNo(CDC& dc, double x, double y, int i);
	void DrawLine2D(CDC& dc, Line2DPtr linePtr, int index);
	void DrawArc2D(CDC& dc, Arc2DPtr arcPtr, int index);

	void DrawArrowOfLine(CDC &dc, CPoint &sPoint, CPoint &ePoint);

	void DrawPolygon(CDC& dc, Polygon2DPtr polygonPtr, bool bDispCoordinate = TRUE);
	CPoint TransformPoint(double x, double y);

	void DrawRectangle(CDC& dc, Rect2DPtr rect);

	void DrawPolygons(CDC& dc);
	void DrawPoints(CDC& dc);
	void DrawLines(CDC& dc);
	void DrawRectangles(CDC& dc);
	void DrawLineArcs(CDC& dc);

	void AdjustDispCoordinate(Point2DPtr point);
	void AdjustDispRegion();
	void AdjustDispRegion(Point2DPtr point);

private:
	Polygon2DList m_vecPolygon;
	std::vector<COLORREF> m_vecPolygonColor;

	std::vector<Rect2DPtr> m_vecRectangle;
	std::vector<COLORREF> m_vecRectangleColor;

	std::vector<Point2DPtr> m_vecPoint;
	std::vector<COLORREF> m_vecPointColor;

	std::vector<Line2DPtr>  m_vecLine;
	std::vector<COLORREF> m_vecLineColor;
	std::vector<int> m_vecLineNo;

	std::vector<Arc2DPtr> m_vecArc;
	std::vector<COLORREF> m_vecArcColor;

private:
	// the caption of the dialog.
	CString m_strCaption;

	// the size of the dialog.
	int m_dDlgWidth;
	int m_dDlgHeight;

	// the distance from the text to cross point.
	int m_iDistance;

	// the distance from the draw area to the dialog border.
	int m_iToTop;
	int m_iToLeft;
	int m_iToRight;
	int m_iToBottom;

	// the size of the display area
	double m_dOrgMinX;
	double m_dOrgMinY;
	double m_dOrgMaxX;
	double m_dOrgMaxY;

	double m_dDisMinX;
	double m_dDisMinY;
	double m_dDisMaxX;
	double m_dDisMaxY;

	// whether display the text.
	BOOL m_bDisplayText;

	BOOL m_bLButtonDown;
	BOOL m_bMButtonDown;

	// the factor for zoom.
	double m_dZoomFactor;

	CPoint m_LButtonDownPt;
	CPoint m_MouseMovePt;
	CPoint m_MButtonDownPt;
};

END_CUTLEADER_NAMESPACE()
