#include "stdafx.h"
#include "clNestUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClNestUIApp, CWinApp)
END_MESSAGE_MAP()

ClNestUIApp::ClNestUIApp()
{
}

ClNestUIApp theApp;

BOOL ClNestUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
