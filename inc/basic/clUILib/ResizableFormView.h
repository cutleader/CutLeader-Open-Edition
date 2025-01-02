#if !defined(AFX_RESIZABLEFORMVIEW_H__INCLUDED_)
#define AFX_RESIZABLEFORMVIEW_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include <afxext.h>
#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClUILib_Export CResizableFormView : public CFormView, 
										 public CResizableLayout,
										 public CResizableGrip, 
										 public CResizableMinMax
{
	DECLARE_DYNAMIC(CResizableFormView)

// Construction
protected:      // must derive your own class
	CResizableFormView(UINT nIDTemplate);
	CResizableFormView(LPCTSTR lpszTemplateName);
	virtual ~CResizableFormView();

private:
	void PrivateConstruct();
	
	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;
	enum GripHideReason		// bitmask
	{
		GHR_MAXIMIZED = 0x01,
		GHR_SCROLLBAR = 0x02,
		GHR_ALIGNMENT = 0x04,
	};

// called from base class
protected:
	virtual void GetTotalClientRect(LPRECT lpRect) const;

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLEFORMVIEW_H__INCLUDED_)
