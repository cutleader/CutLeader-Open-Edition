#if !defined(AFX_RESIZABLEGRIP_H__INCLUDED_)
#define AFX_RESIZABLEGRIP_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include "ResizableState.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export CResizableGrip  
{
private:
	class CSizeGrip : public CScrollBar
	{
	public:
		CSizeGrip()
		{
			m_bTransparent = FALSE;
			m_bTriangular = FALSE;
		}

		void SetTriangularShape(BOOL bEnable);
		void SetTransparency(BOOL bActivate);

		BOOL IsRTL();			// right-to-left layout support

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

		SIZE m_size;			// holds grip size

	protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

		BOOL m_bTriangular;		// triangular shape active
		BOOL m_bTransparent;	// transparency active

		// memory DCs and bitmaps for transparent grip
		CDC m_dcGrip, m_dcMask;
		CBitmap m_bmGrip, m_bmMask;
	};

	CSizeGrip m_wndGrip;		// grip control
	int m_nShowCount;			// support for hiding the grip

protected:
	// create a size grip, with options
	BOOL CreateSizeGrip(BOOL bVisible = TRUE,
		BOOL bTriangular = TRUE, BOOL bTransparent = FALSE);

	BOOL IsSizeGripVisible();	// TRUE if grip is set to be visible
	void SetSizeGripVisibility(BOOL bVisible);	// set default visibility
	void UpdateSizeGrip();		// update the grip's visibility and position
	void ShowSizeGrip(DWORD* pStatus, DWORD dwMask = 1);	// temp show the size grip
	void HideSizeGrip(DWORD* pStatus, DWORD dwMask = 1);	// temp hide the size grip
	BOOL SetSizeGripBkMode(int nBkMode);		// like CDC::SetBkMode
	void SetSizeGripShape(BOOL bTriangular);

	virtual CWnd* GetResizableWnd() const = 0;

public:
	CResizableGrip();
	virtual ~CResizableGrip();
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLEGRIP_H__INCLUDED_)
