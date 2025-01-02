#if !defined(AFX_RESIZABLEDIALOG_H__INCLUDED_)
#define AFX_RESIZABLEDIALOG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"
#include "ResizableWndState.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export CResizableDialog : public CDialogEx, 
									    public CResizableLayout,
									    public CResizableGrip, 
									    public CResizableMinMax,
									    public CResizableWndState
{
public:
	CResizableDialog();
	CResizableDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CResizableDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);

private:
	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;

	// flags
	BOOL m_bEnableSaveRestore;
	BOOL m_bRectOnly;

	// internal status
	CString m_sSection;			// section name (identifies a parent window)

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual ~CResizableDialog();

// used internally
private:
	void PrivateConstruct();

// callable from derived classes
protected:
	// section to use in app's profile
	void EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE);

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLEDIALOG_H__INCLUDED_)
