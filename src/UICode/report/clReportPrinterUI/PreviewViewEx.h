#pragma once

#include "clReportPrinterCommon.h"
#include "afxpriv.h"
#include "IPrintWnd.h"

BEGIN_CUTLEADER_NAMESPACE()

class PreviewViewEx : public CPreviewView
{
	DECLARE_DYNCREATE(PreviewViewEx)

public:
	PreviewViewEx(void);
	~PreviewViewEx(void);

public:
	virtual void OnDraw(CDC* pDC);

protected:
	afx_msg void OnPreviewPrint();
	afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	void SetPrintWnd(IPrintWnd* pPrintWnd) { m_pPrintWnd = pPrintWnd; }
	CView* GetOrigView() { return m_pOrigView; }

protected:
	IPrintWnd* m_pPrintWnd;
};

END_CUTLEADER_NAMESPACE()
