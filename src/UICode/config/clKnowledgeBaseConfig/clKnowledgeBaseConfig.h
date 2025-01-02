#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "cutLeaderCommon.h"

DECLARE_CUTLEADER_CLASS(KBConfigDoc)

class ClKnowledgeBaseConfigApp : public CWinAppEx
{
public:
	ClKnowledgeBaseConfigApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	afx_msg void OnFileSave();
	afx_msg void OnMacMgr();
	afx_msg void OnExpertLibMgr();
	afx_msg void OnMatLibMgr();

	afx_msg void OpenLibrary(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	// get active document.
	KBConfigDoc* GetActiveDocument();

private:
	// do some initializations when the application starts.
	void Init();

private:
	CMultiDocTemplate* m_pMacMgrDocTpl;
	CMultiDocTemplate* m_pExpertLibDocTpl;
	CMultiDocTemplate* m_pMatLibDocTpl;
};

extern ClKnowledgeBaseConfigApp theApp;
