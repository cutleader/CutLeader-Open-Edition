#include "stdafx.h"
#include "clCanvasGeometryUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClCanvasGeometryUIApp, CWinApp)
END_MESSAGE_MAP()

ClCanvasGeometryUIApp::ClCanvasGeometryUIApp()
{
}

ClCanvasGeometryUIApp theApp;

BOOL ClCanvasGeometryUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
