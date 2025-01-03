#if !defined(AFX_RESIZABLESHEET_H__INCLUDED_)
#define AFX_RESIZABLESHEET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"
#include "ResizableSheetState.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export CResizableSheet : public CPropertySheet, 
									  public CResizableLayout,
									  public CResizableGrip, 
									  public CResizableMinMax,
									  public CResizableSheetState
{
	DECLARE_DYNAMIC(CResizableSheet)

// Construction
public:
	CResizableSheet();
	CResizableSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CResizableSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
private:
	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;

	// flags
	BOOL m_bEnableSaveRestore;
	BOOL m_bRectOnly;
	BOOL m_bSavePage;

	// layout vars
	UINT m_nCallbackID;
	CSize m_sizePageTL, m_sizePageBR;
	BOOL m_bLayoutDone;

	// internal status
	CString m_sSection;			// section name (identifies a parent window)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	protected:

// Implementation
public:
	virtual ~CResizableSheet();

// used internally
private:
	void PrivateConstruct();

	BOOL IsWizard() const;

// callable from derived classes
protected:
	void PresetLayout();
	void RefreshLayout();

	// section to use in app's profile
	void EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE,
		BOOL bWithPage = FALSE);
	int GetMinWidth();	// minimum width to display all buttons


	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

// Generated message map functions
protected:
	virtual BOOL CalcSizeExtra(HWND hWndChild, CSize sizeChild, CSize& sizeExtra);
	virtual BOOL ArrangeLayoutCallback(LAYOUTINFO& layout) const;
	//{{AFX_MSG(CResizableSheet)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg BOOL OnPageChanging(NMHDR* pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()

#endif	// AFX_RESIZABLESHEET_H__INCLUDED_
