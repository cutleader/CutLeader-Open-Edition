#include "stdafx.h"
#include "clReportConfigUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(ClReportConfigUIApp, CWinApp)
END_MESSAGE_MAP()


ClReportConfigUIApp::ClReportConfigUIApp()
{

}


ClReportConfigUIApp theApp;


BOOL ClReportConfigUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
