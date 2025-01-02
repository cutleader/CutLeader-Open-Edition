#pragma once

#include "ChildFrm.h"
#include "clFrameCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// the frame window of the part view.
class CLFRAME_Export PartFrame : public CChildFrame
{
	DECLARE_DYNCREATE(PartFrame)

public:
	PartFrame(void);
	~PartFrame(void);

public:
	// when activate a part frame, refresh the UI here.
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);

	/************************************************************************/
	// clean something in these functions.

	// if you click "X" button, this will be called.
	// notes:
	//   1) even this function has been called, the frame maybe alive. e.g., if framework ask you to save the doc before close it, and 
	//		you choose "cancel". so do not do final clean work here, and do it in "OnDestroy".
	afx_msg void OnClose();

	// do something when the frame will be really destroyed.
	afx_msg void OnDestroy();
	/************************************************************************/

	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE();
