#pragma once

#include "clReportTemplateCommon.h"

DECLARE_CUTLEADER_CLASS(ReportTplPageList)
DECLARE_CUTLEADER_CLASS(ReportTplPage)

BEGIN_CUTLEADER_NAMESPACE()

// this class will keep the opened template pages.
// notes:
//   1) when open/delete template pages, we should query info from this class.
class ClReportTemplate_Export TplPageTracker
{
public:
	~TplPageTracker(void);

private:
	TplPageTracker(void);

public:
	static TplPageTracker* GetInstance();

public:
	// check whether the template page is opened.
	BOOL IsItemOpened(LONGLONG iDataID);

	// when open a template page, add it to this class.
	void AddItem(ReportTplPagePtr pReportTplPage);

	// remove the opened template page.
	void RemoveItem(LONGLONG iDataID);

private:
	// the singleton object
	static TplPageTracker* m_pTplPageTracker;

	// the opened template pages.
	ReportTplPageListPtr m_pReportTplPageList;
};

END_CUTLEADER_NAMESPACE()
