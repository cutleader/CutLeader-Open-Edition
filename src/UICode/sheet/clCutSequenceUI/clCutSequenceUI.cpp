#include "stdafx.h"
#include "clCutSequenceUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClCutSequenceUIApp, CWinApp)
END_MESSAGE_MAP()

ClCutSequenceUIApp::ClCutSequenceUIApp()
{
}

ClCutSequenceUIApp theApp;

BOOL ClCutSequenceUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
