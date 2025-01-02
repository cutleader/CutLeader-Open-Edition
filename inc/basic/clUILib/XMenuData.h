#pragma once

#include "clUILibCommon.h"
#include <afxtempl.h>

BEGIN_CUTLEADER_NAMESPACE()

// Fill this class structure to define a single menu item.
class ClUILib_Export XMenuData
{
public:
	XMenuData();
	~XMenuData();

public:
	void SetAnsiString(LPCSTR szAnsiString);
	void SetWideString(const wchar_t *szWideString);
	const wchar_t* GetWideString(void) { return m_szMenuText; }

	// returns the menu text in ANSI or UNICODE
	CString GetString(void);

public:
	int xoffset, global_offset;
	int menuIconNormal;
	UINT nFlags, nID, syncflag;
	CImageList* bitmap;

	// used to attach user data
	void *pContext;

private:
	wchar_t *m_szMenuText;
};

END_CUTLEADER_NAMESPACE()
