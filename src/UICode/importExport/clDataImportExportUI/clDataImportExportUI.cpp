#include "stdafx.h"
#include "clDataImportExportUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClDataImportExportUIApp, CWinApp)
END_MESSAGE_MAP()

ClDataImportExportUIApp::ClDataImportExportUIApp()
{
}

ClDataImportExportUIApp theApp;

BOOL ClDataImportExportUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
