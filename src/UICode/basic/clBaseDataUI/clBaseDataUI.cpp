#include "stdafx.h"
#include "clBaseDataUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClBaseDataUIApp, CWinApp)
END_MESSAGE_MAP()

ClBaseDataUIApp::ClBaseDataUIApp()
{
}

ClBaseDataUIApp theApp;

BOOL ClBaseDataUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
