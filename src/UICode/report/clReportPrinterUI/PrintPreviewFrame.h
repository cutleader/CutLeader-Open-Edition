#pragma once

#include "clReportPrinterCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 打印预览的显示框架。
class PrintPreviewFrame : public CFrameWnd
{
	DECLARE_DYNCREATE (PrintPreviewFrame);

public:
	PrintPreviewFrame(void);

protected:
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()
