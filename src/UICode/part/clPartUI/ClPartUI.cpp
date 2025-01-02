#include "stdafx.h"
#include "ClPartUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClPartUIApp, CWinApp)
END_MESSAGE_MAP()

ClPartUIApp::ClPartUIApp()
{
}

ClPartUIApp theApp;

BOOL ClPartUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
