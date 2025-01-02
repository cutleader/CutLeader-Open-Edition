#pragma once

#include "cutLeaderCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)

public:
	CChildFrame();
	virtual ~CChildFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	// if you click "X" button, this will be called.
	// notes:
	//   1) even this function has been called, the frame maybe alive.
	//      e.g., if framework ask you to save the doc before close it, and 
	//		you choose "cancel". so do not do final clean work here, and do it in "OnDestroy".
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()
