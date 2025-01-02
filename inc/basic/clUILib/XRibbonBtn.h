#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// ��չ��ribbon��ť��������������ʾ��ʾ��Ϣ��
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
	// �Ƿ���ʾ��ť��ʾ��
	bool m_bShowTips;

	// ��ʾ���֡�
	CString m_strTips;
};

END_CUTLEADER_NAMESPACE()
