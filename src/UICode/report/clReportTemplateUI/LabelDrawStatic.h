#pragma once

#include "clReportTemplateCommon.h"
#include "taoResource.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)
DECLARE_CUTLEADER_CLASS(LabelTplEntity)

BEGIN_CUTLEADER_NAMESPACE()

// 预料标签的窗口。
class LabelDrawStatic : public CStatic
{
	DECLARE_DYNAMIC(LabelDrawStatic)

public:
	LabelDrawStatic(void);
	~LabelDrawStatic(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();

public:
	void Init(ReportTplPagePtr pReportTplPage, LabelTplEntityPtr pLabelTplEntity);

private:
	// 得到页面在绘图区的位置。
	/* 参数：
	*  dScale: 为了显示在绘图区，页面要做的缩放。
	*/
	CRect GetPageRect(double& dScale);

private:
	ReportTplPagePtr m_pReportTplPage;
	LabelTplEntityPtr m_pLabelTplEntity;
};

END_CUTLEADER_NAMESPACE()
