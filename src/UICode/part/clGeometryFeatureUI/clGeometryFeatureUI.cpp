#include "stdafx.h"
#include "clGeometryFeatureUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClGeometryFeatureUIApp, CWinApp)
END_MESSAGE_MAP()

ClGeometryFeatureUIApp::ClGeometryFeatureUIApp()
{
}

ClGeometryFeatureUIApp theApp;

BOOL ClGeometryFeatureUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
