#include "stdafx.h"
#include "clPartLayoutUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClPartLayoutUIApp, CWinApp)
END_MESSAGE_MAP()

ClPartLayoutUIApp::ClPartLayoutUIApp()
{
}

ClPartLayoutUIApp theApp;

BOOL ClPartLayoutUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
