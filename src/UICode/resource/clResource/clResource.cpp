#include "stdafx.h"
#include "clResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClResourceApp, CWinApp)
END_MESSAGE_MAP()

ClResourceApp::ClResourceApp()
{
}

ClResourceApp theApp;

BOOL ClResourceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
