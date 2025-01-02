#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 扩展的ribbon按钮，可以在下面显示提示信息。
class ClUILib_Export XRibbonBtn : public CMFCRibbonButton
{
public:
	XRibbonBtn(UINT nID, LPCTSTR lpszText, int nSmallImageIndex = -1, int nLargeImageIndex = -1);
	~XRibbonBtn(void);

public:
	virtual COLORREF OnFillBackground(CDC* pDC);

public:
	void EnableTips(CString strTips);
	void DisableTips() { m_bShowTips = false; }

private:
	// 是否显示按钮提示。
	bool m_bShowTips;

	// 提示文字。
	CString m_strTips;
};

END_CUTLEADER_NAMESPACE()
