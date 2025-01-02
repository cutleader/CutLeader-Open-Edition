#include "stdafx.h"
#include "ClOptionUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClOptionUIApp, CWinApp)
END_MESSAGE_MAP()

ClOptionUIApp::ClOptionUIApp()
{
}

ClOptionUIApp theApp;

BOOL ClOptionUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
