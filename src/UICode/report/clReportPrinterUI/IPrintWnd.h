#pragma once

#include "clReportPrinterUICommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 支持打印的窗口。
class ClReportPrinterUI_Export IPrintWnd
{
public:
	/************************************************************************/	
	// 打印相关。

	// 准备打印。
	virtual void PreparePrinting(CPrintInfo* pInfo) = 0;

	// 开始打印。
	virtual void BeginPrinting(CDC* pDC, CPrintInfo* pInfo) = 0;

	// 准备设备环境。
	virtual void PrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL) = 0;

	// 打印一页。
	virtual void Print(CDC* pDC, CPrintInfo* pInfo) = 0;

	// 结束打印。
	virtual void EndPrinting(CDC* pDC, CPrintInfo* pInfo) = 0;

	// 预览和打印数据。
	virtual void PreviewData() = 0;
	virtual void PrintData() = 0;
	/************************************************************************/

	// 窗口相关。
	virtual CString GetWndText() = 0;
	virtual void SetWndFocus() = 0;
};

END_CUTLEADER_NAMESPACE()
