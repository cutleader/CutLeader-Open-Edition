#if !defined(AFX_RESIZABLEWNDSTATE_H__INCLUDED_)
#define AFX_RESIZABLEWNDSTATE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"
#include "ResizableState.h"

BEGIN_CUTLEADER_NAMESPACE()

///////////////////////////////////////////////////////////////////////////////

// Persists window position, size and state
// notes:
//   1) Derive from this class when you want to persist the size, position and
//		minimized/maximized state of top level windows.
//		This class is used in the provided resizable counterparts of
//		the standard MFC window and dialog classes.

///////////////////////////////////////////////////////////////////////////////

class ClUILib_Export CResizableWndState : public CResizableState  
{
protected:

	//! @brief Load and set the window position and size
	BOOL LoadWindowRect(LPCTSTR pszName, BOOL bRectOnly);

	//! @brief Save the current window position and size
	BOOL SaveWindowRect(LPCTSTR pszName, BOOL bRectOnly);

	//! @brief Override to provide the parent window
	virtual CWnd* GetResizableWnd() const = 0;

public:
	CResizableWndState();
	virtual ~CResizableWndState();
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLEWNDSTATE_H__INCLUDED_)
