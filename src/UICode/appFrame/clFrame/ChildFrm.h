#pragma once

#include "clFrameCommon.h"
#include "clFrameResource.h"

BEGIN_CUTLEADER_NAMESPACE()

class CLFRAME_Export CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)

public:
	CChildFrame();
	virtual ~CChildFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	DECLARE_MESSAGE_MAP()
};

END_CUTLEADER_NAMESPACE()
