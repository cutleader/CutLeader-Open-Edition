#include "stdafx.h"
#include "CLFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CLFrameApp, CWinApp)
END_MESSAGE_MAP()

CLFrameApp::CLFrameApp()
{
}

CLFrameApp theApp;

BOOL CLFrameApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
