#include "StdAfx.h"
#include "DummyAction.h"

#include "baseConst.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"

BEGIN_CUTLEADER_NAMESPACE()

DummyAction::DummyAction(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;

	m_iMsgID = IDS_OUTPUT_WELCOME;
}

DummyAction::~DummyAction(void)
{
}

BOOL DummyAction::DrawBeforeFinish()
{
	CursorDrawer::DrawDefaultCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
