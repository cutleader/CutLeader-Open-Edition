#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


class ClReportTemplateUIApp : public CWinApp
{
public:
	ClReportTemplateUIApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
