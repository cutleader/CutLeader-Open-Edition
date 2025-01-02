#pragma once

#include "clFrameCommon.h"
#include "ChildFrm.h"


BEGIN_CUTLEADER_NAMESPACE()


class CLFRAME_Export TaskFrame : public CChildFrame
{
	DECLARE_DYNCREATE(TaskFrame)

public:
	TaskFrame();

public:
    virtual ~TaskFrame();

protected:
	DECLARE_MESSAGE_MAP()

    // when activate a sheet frame, refresh the UI here.
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
};

END_CUTLEADER_NAMESPACE();
