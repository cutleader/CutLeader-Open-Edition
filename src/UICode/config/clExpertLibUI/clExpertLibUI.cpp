#include "stdafx.h"
#include "clExpertLibUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(ClExpertLibUIApp, CWinApp)
END_MESSAGE_MAP()


ClExpertLibUIApp::ClExpertLibUIApp()
{
}


ClExpertLibUIApp theApp;


BOOL ClExpertLibUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
