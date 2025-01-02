#pragma once

#include "cutLeaderCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class CornerBoxWnd : public CWnd
{
public:
	CornerBoxWnd(void);
	~CornerBoxWnd(void);

protected:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()
