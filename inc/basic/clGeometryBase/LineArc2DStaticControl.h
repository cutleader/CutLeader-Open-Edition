#pragma once

#include "clGeometryBaseCommon.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(LineArc2DList)

BEGIN_CUTLEADER_NAMESPACE()

// the static control for displaying 2d line and arc, ������ʾ��άֱ��Բ���ľ�̬�ؼ���
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
	// ��ʼ���ӿڡ�
	// ע��
	//   1) �ú��������Ҫ���Ƶ�ͼ�Ρ�
	void InitViewPort(const Rect2D& geomRect);

	// zoom the view port.
	void ZoomViewPort(short iDelta, const CPoint& screenPt);

	// pan the view port.
	void PanViewPort(double dXClientOffSet, double dYClientOffSet);

	// �����ӿ����ʺϾ��ο�
	void FitRect(const Rect2D& geomRect);

	// �ͷ��ӿڡ�
	void ReleaseViewPort();

	void SetBackgroundColor(COLORREF dwBackgroundColor) { m_dwBackgroundColor = dwBackgroundColor; }
	COLORREF GetBackgroundColor() const { return m_dwBackgroundColor; }

	void SetDrawColor(COLORREF dwDrawColor) { m_dwDrawColor = dwDrawColor; }
	COLORREF GetDrawColor() const { return m_dwDrawColor; }

private:
	// OpenGL�ӿڡ�
	GlViewPortPtr m_pViewPort;
	COLORREF m_dwBackgroundColor;
	COLORREF m_dwDrawColor;

	// the reference point for pan the view port.
	CPoint m_referPt;

	// Ҫ���Ƶ�ʵ��ͼ�Ρ�
	// ע��
	//   1) ʵ�ߣ��߿�Ϊ1����ɫ��
	LineArc2DListPtr m_pSolidGeom2DList;

	// Ҫ���Ƶ�����ͼ�Ρ�
	// ע��
	//   1) ���ߣ��߿�Ϊ1����ɫ��
	LineArc2DListPtr m_pDashGeom2DList;

	// Ҫ���Ƶı�ѡ��ͼ�Ρ�
	// ע��
	//   1) ʵ�ߣ��߿�Ϊ2����ɫ��
	LineArc2DListPtr m_pSelGeom2DList;
};

END_CUTLEADER_NAMESPACE()
