#pragma once

#include "clUILibCommon.h"
#include "SplashWnd.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export SplashThread : public CWinThread
{
public:
	SplashThread(void);
	~SplashThread(void);

public:
	DECLARE_DYNCREATE(SplashThread)

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	void HideSplash();
	void SetBitmapToUse(const CString& sFilename);
	void SetBitmapToUse(UINT nResourceID);
	void SetBitmapToUse(LPCTSTR pszResourceName);

private:
	SplashWnd m_SplashScreen;
};

END_CUTLEADER_NAMESPACE()
