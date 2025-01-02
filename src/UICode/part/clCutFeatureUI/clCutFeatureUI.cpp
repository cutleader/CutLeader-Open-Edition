#include "stdafx.h"
#include "ClCutFeatureUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClCutFeatureUIApp, CWinApp)
END_MESSAGE_MAP()

ClCutFeatureUIApp::ClCutFeatureUIApp()
{
}

ClCutFeatureUIApp theApp;

BOOL ClCutFeatureUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
