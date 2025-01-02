#pragma once

#include "clReportConfigCommon.h"
#include "cutLeaderCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class HorzRulerWnd : public CWnd
{
public:
	HorzRulerWnd(void);
	~HorzRulerWnd(void);

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

public:
	void SetStartPos( int startPos );
	int GetStartPos() const { return m_startPos; }

	void SetMeasurements(TPL_UNIT_TYPE measurements);
	TPL_UNIT_TYPE GetMeasurements() const { return m_measurements; }

	void SetZoom(double dZoom);
	double GetZoom() const { return m_dZoom; }

	void SetPointerPos(int pointerPos);
	int GetPointerPos() const { return m_pointerPos; }

private:
	void DrawPixelScale( CDC* dc, CRect rect );
	void DrawInchScale( CDC* dc, CRect rect );
	void DrawCentimeterScale( CDC* dc, CRect rect );
	void DrawScale( CDC* dc, CRect rect, double seg, double stepno );

private:
	int	m_startPos;
	TPL_UNIT_TYPE m_measurements;
	int	m_pointerPos;
	double m_dZoom;
};

END_CUTLEADER_NAMESPACE()
