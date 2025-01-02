#if !defined(AFX_RESIZABLEMDICHILD_H__INCLUDED_)
#define AFX_RESIZABLEMDICHILD_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include "ResizableMinMax.h"
#include "ResizableWndState.h"
#include "ResizableLayout.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export CResizableMDIChild : public CMDIChildWndEx, 
										  public CResizableMinMax,
										  public CResizableWndState, 
										  public CResizableLayout
{
	DECLARE_DYNCREATE(CResizableMDIChild)

protected:
	CResizableMDIChild();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableMDIChild)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CResizableMDIChild();

	BOOL EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE);

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

private:
	// flags
	BOOL m_bEnableSaveRestore;
	BOOL m_bRectOnly;

	// internal status
	CString m_sSection;			// section name (identifies a parent window)

protected:
	// Generated message map functions
	//{{AFX_MSG(CResizableMDIChild)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLEMDICHILD_H__INCLUDED_)
