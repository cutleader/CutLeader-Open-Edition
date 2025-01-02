#include "stdafx.h"
#include "clTaskUI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(ClTaskUIApp, CWinApp)
END_MESSAGE_MAP()

ClTaskUIApp::ClTaskUIApp()
{
}

ClTaskUIApp theApp;

BOOL ClTaskUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
