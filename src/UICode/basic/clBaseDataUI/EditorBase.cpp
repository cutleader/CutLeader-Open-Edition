#include "StdAfx.h"
#include "EditorBase.h"

#include "IAction.h"
#include "clUILibResource.h"
#include "DummyAction.h"
#include "GlViewPort.h"
#include "baseConst.h"

BEGIN_CUTLEADER_NAMESPACE()

EditorBase::EditorBase(void)
{
}

EditorBase::~EditorBase(void)
{
}

void EditorBase::SetDummyAction()
{
	m_pAction.reset(new DummyAction(m_pViewPort));
}

void EditorBase::UpdateActionForUndoRedo()
{
	if (m_pAction)
		m_pAction->UpdateForUndoRedo();
}

END_CUTLEADER_NAMESPACE()
