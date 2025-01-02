#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export ColorStatic : public CStatic
{
public:
	ColorStatic(void);
	~ColorStatic(void);

public:
	void SetColor( COLORREF color );
	COLORREF GetColor() { return m_color; }

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	COLORREF m_color;
};

END_CUTLEADER_NAMESPACE()
