#include "stdafx.h"
#include "clReportTemplateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(ClReportTemplateUIApp, CWinApp)
END_MESSAGE_MAP()


ClReportTemplateUIApp::ClReportTemplateUIApp()
{
}


ClReportTemplateUIApp theApp;


BOOL ClReportTemplateUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
