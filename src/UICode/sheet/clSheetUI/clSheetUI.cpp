#include "stdafx.h"
#include "clSheetUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClSheetUIApp, CWinApp)
END_MESSAGE_MAP()

ClSheetUIApp::ClSheetUIApp()
{
}

ClSheetUIApp theApp;

BOOL ClSheetUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
