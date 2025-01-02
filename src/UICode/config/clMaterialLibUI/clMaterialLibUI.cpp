#include "stdafx.h"
#include "clMaterialLibUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(ClMaterialLibUIApp, CWinApp)
END_MESSAGE_MAP()


ClMaterialLibUIApp::ClMaterialLibUIApp()
{

}


ClMaterialLibUIApp theApp;


BOOL ClMaterialLibUIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
