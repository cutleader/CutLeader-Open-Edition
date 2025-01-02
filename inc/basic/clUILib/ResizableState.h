#if !defined(AFX_RESIZABLESTATE_H__INCLUDED_)
#define AFX_RESIZABLESTATE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "clUILibCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

///////////////////////////////////////////////////////////////////////////////

// Provides basic persisting capabilities
// notes:
//   1) Derive from this class to persist user interface settings, or anything
//      suitable. The base implementation uses the application profile, which can
//		be set to either the Registry or an INI File. Other storing methods
//		can be implemented in derived classes.

///////////////////////////////////////////////////////////////////////////////

class ClUILib_Export CResizableState  
{
public:
	CResizableState();

	virtual ~CResizableState();

protected:
	//! @brief Get default path where state is stored
	static LPCTSTR GetDefaultStateStore();
	
	//! @brief Set default path where state is stored
	static void SetDefaultStateStore(LPCTSTR szPath);

	//! @brief Get current path where state is stored
	LPCTSTR GetStateStore();
	
	//! @brief Set current path where state is stored
	void SetStateStore(LPCTSTR szPath);

	//! @name Overridables
	//@{
	
	//! @brief Read state information
	virtual BOOL ReadState(LPCTSTR szId, CString& rsState);
	
	//! @brief Write state information
	virtual BOOL WriteState(LPCTSTR szId, LPCTSTR szState);

private:
	static CString m_sDefaultStorePath;

	CString m_sStorePath;
};

END_CUTLEADER_NAMESPACE()

#endif // !defined(AFX_RESIZABLESTATE_H__INCLUDED_)
