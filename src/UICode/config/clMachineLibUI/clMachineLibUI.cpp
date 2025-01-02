#include "stdafx.h"
#include "clMachineLibUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(ClMachineLibUIApp, CWinApp)
END_MESSAGE_MAP()


ClMachineLibUIApp::ClMachineLibUIApp()
{
}


ClMachineLibUIApp theApp;


BOOL ClMachineLibUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
