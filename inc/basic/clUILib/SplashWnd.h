#pragma once

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export SplashWnd : public CWnd
{
public:
	SplashWnd(void);
	~SplashWnd(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

public:
	void SetBitmapToUse(const CString& sFilename);
	void SetBitmapToUse(UINT nResourceID);
	void SetBitmapToUse(LPCTSTR pszResourceName); 
	BOOL Create();
	void SetOKToClose() 
	{ 
		m_bOKToClose = TRUE; 
	};  

protected:
	BOOL SelRelPal(BOOL bForceBkgnd);
	BOOL LoadBitmap();

private:
	BOOL      m_bOKToClose;
	CBitmap   m_Bitmap;
	CPalette  m_Palette;
	int       m_nHeight;
	int       m_nWidth;
	CWnd      m_wndOwner;                   
	BOOL      m_bUseFile;
	LPCTSTR   m_pszResourceName;
	CString   m_sFilename;
};

END_CUTLEADER_NAMESPACE()
