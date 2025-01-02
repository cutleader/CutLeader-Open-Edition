#pragma once

#include "clReportPrinterUICommon.h"
#include "IPrintWnd.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClReportPrinterUI_Export PrintWndBase : public IPrintWnd
{
public:
	PrintWndBase(void);
	~PrintWndBase(void);

public: // 接口实现。
	virtual void PreparePrinting(CPrintInfo* pInfo);
	virtual void BeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void PrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void Print(CDC* pDC, CPrintInfo* pInfo);
	virtual void EndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void PreviewData();
	virtual void PrintData();

private:
	// 显示打印预览的文档模板。
	CSingleDocTemplate*	m_pTemplate;
};

END_CUTLEADER_NAMESPACE()
