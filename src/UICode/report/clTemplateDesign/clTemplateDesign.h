#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "Resource.h"
#include "tplDesignDoc.h"

class ClTemplateDesignApp : public CWinAppEx
{
public:
	ClTemplateDesignApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnNewReportTpl();
	afx_msg void OnOpenReportTpl();
	afx_msg void OnSaveReportTpl();

	afx_msg void OnPreference();
	afx_msg void OnMgrReportTpl();

	DECLARE_MESSAGE_MAP()

private:
	// do some initializations when the application starts.
	void Init();

	// do some un-initializations when the application ends.
	void UnInit();

	// get active view
	CtplDesignDoc* GetActiveDocument();

private:
	CMultiDocTemplate* m_pDocTemplate;
};

extern ClTemplateDesignApp theApp;
