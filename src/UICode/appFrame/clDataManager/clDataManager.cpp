#include "stdafx.h"
#include "clDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClDataManagerApp, CWinApp)
END_MESSAGE_MAP()

ClDataManagerApp::ClDataManagerApp()
{
}

ClDataManagerApp theApp;

BOOL ClDataManagerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
