#pragma once

#include "clReportPrinterCommon.h"
#include "IPrintWnd.h"

BEGIN_CUTLEADER_NAMESPACE()

// 打印预览框架中的视图。
// 注：
//  1) 这个视图会被PreviewViewEx替换，作为它的m_pOrigView成员。
//  2) 打印的方法会转到这个类中。
class WorkingView : public CView
{
	DECLARE_DYNCREATE(WorkingView)

public:
	WorkingView(void);
	~WorkingView(void);

public: // inherit from the base class.
	virtual void OnDraw(CDC* pDC);

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);

protected:
	DECLARE_MESSAGE_MAP()

public:
	void OnPrintPreview();
	void OnFilePrint();

	void SetPrintWnd(IPrintWnd* pPrintWnd) { m_pPrintWnd = pPrintWnd; }
	
	void IsDestroyParentFrm(BOOL bDestroyParentFrm) { m_bDestroyParentFrm = bDestroyParentFrm; }

public:
	friend class PreviewViewEx;

	IPrintWnd* m_pPrintWnd;
	CFrameWnd* m_pCurrentFrame;

	// 这个类要不要销毁父框架。
	BOOL m_bDestroyParentFrm;
};

END_CUTLEADER_NAMESPACE()
