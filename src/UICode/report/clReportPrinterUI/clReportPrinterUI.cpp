#include "stdafx.h"
#include "clReportPrinterUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ClReportPrinterUIApp, CWinApp)
END_MESSAGE_MAP()



ClReportPrinterUIApp::ClReportPrinterUIApp()
{

}


ClReportPrinterUIApp theApp;


BOOL ClReportPrinterUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
