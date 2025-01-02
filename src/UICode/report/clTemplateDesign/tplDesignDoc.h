#pragma once

#include "cutLeaderCommon.h"

DECLARE_CUTLEADER_CLASS(ReportTplPage)

BEGIN_CUTLEADER_NAMESPACE()

class CtplDesignDoc : public CDocument
{
	DECLARE_DYNCREATE(CtplDesignDoc)

public:
	CtplDesignDoc();
	virtual ~CtplDesignDoc();

public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument();
	virtual BOOL DoFileSave() { return OnSaveDocument(); }

	virtual void SetModifyStatus();
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE) {}

protected:
	DECLARE_MESSAGE_MAP()

public: // get/set functions.
	ReportTplPagePtr GetReportTplPage() { return m_pReportTplPage; }

private:
	ReportTplPagePtr m_pReportTplPage;
};

END_CUTLEADER_NAMESPACE()
