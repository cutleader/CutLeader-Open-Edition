#include "stdafx.h"
#include "clPostProcessorUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClPostProcessorUIApp, CWinApp)
END_MESSAGE_MAP()

ClPostProcessorUIApp::ClPostProcessorUIApp()
{
}

ClPostProcessorUIApp theApp;

BOOL ClPostProcessorUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
