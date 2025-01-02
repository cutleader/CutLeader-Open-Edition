#if !defined(AFX_RESIZABLEPAGEEX_H__INCLUDED_)
#define AFX_RESIZABLEPAGEEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include "ResizableLayout.h"
#include "ResizableMinMax.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export CResizablePageEx : public CPropertyPageEx, 
									   public CResizableLayout,
									   public CResizableMinMax
{
	DECLARE_DYNCREATE(CResizablePageEx)

// Construction
public:
	CResizablePageEx();
	CResizablePageEx(UINT nIDTemplate, UINT nIDCaption = 0, UINT nIDHeaderTitle = 0, UINT nIDHeaderSubTitle = 0);
	CResizablePageEx(LPCTSTR lpszTemplateName, UINT nIDCaption = 0, UINT nIDHeaderTitle = 0, UINT nIDHeaderSubTitle = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizablePageEx)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizablePageEx();

// callable from derived classes
protected:

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

// Generated message map functions
protected:
	//{{AFX_MSG(CResizablePageEx)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLEPAGEEX_H__INCLUDED_)
