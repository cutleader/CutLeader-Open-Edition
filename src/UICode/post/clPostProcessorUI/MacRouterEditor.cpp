#include "StdAfx.h"
#include "MacRouterEditor.h"

#include "NCSimulateAction.h"
#include "clPostProcessorResource.h"
#include "MultiLanguageMgr.h"

BEGIN_CUTLEADER_NAMESPACE()

MacRouterEditor::MacRouterEditor(GlViewPortPtr pViewPort)
{
	m_pViewPort = pViewPort;

	m_pAction.reset();
}

MacRouterEditor::~MacRouterEditor(void)
{
}

BOOL MacRouterEditor::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL MacRouterEditor::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pAction)
	{
		m_pAction->MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

BOOL MacRouterEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pAction)
	{
		m_pAction->OnKeyDown(nChar, nRepCnt, nFlags);

		// check whether we will enter other action.
		IActionPtr pNextAction = m_pAction->GetNextAction();
		if (pNextAction)
			m_pAction = pNextAction;
	}

	return TRUE;
}

BOOL MacRouterEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

CString MacRouterEditor::GetEditorName() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_NC);
	return str;
}

BOOL MacRouterEditor::SetAction(MACROUTER_ACTION_TYPE actionType)
{
	if (actionType == MACROUTER_NC_SIMULATE)
	{
		NCSimulateAction* pNCSimulateAction= new NCSimulateAction(m_pMaterialSize, m_pViewPort);
		m_pAction.reset(pNCSimulateAction);
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
