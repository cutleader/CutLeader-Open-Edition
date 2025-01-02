#pragma once

#include "CornerConfigItem.h"
#include "CornerRangeInfo.h"

DECLARE_CUTLEADER_CLASS(GlViewPort)
DECLARE_CUTLEADER_CLASS(CornerRangeInfo)

BEGIN_CUTLEADER_NAMESPACE()

// we can draw corner in the static control.
class CornerDrawStatic : public CStatic
{
	DECLARE_DYNAMIC(CornerDrawStatic)

public:
	CornerDrawStatic(void);
	~CornerDrawStatic(void);

public:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

public:
	// 初始化。
	void InitDrawer();

	void SetRangeCorner(CornerRangeInfoPtr pRangeCornerInfo) { m_pRangeCornerInfo = pRangeCornerInfo; }

private:
	void DrawRangeCorner();

private:
	GlViewPortPtr m_pViewPort;
	double m_dMaxSize;

	CornerRangeInfoPtr m_pRangeCornerInfo;
};

END_CUTLEADER_NAMESPACE()
