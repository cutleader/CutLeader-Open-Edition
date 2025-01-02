#include "StdAfx.h"
#include "MicroJointDelAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "Point2DList.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"

#include "IPattern.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "PatternList.h"

#include "LoopMicroJointList.h"
#include "MicroJointList.h"
#include "MicroJoint.h"
#include "LoopMicroJoint.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "MicroJointDeleteCommand.h"
#include "clCutFeatureResource.h"

BEGIN_CUTLEADER_NAMESPACE()

MicroJointDelAction::MicroJointDelAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
    : MicroJointPreSelectAction(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
	m_iMsgID = IDS_CUTFEATURE_SELECT_MicroJoint;
}

MicroJointDelAction::~MicroJointDelAction(void)
{
}

BOOL MicroJointDelAction::LButtonDown(double& dPtX, double& dPtY)
{
	MicroJointPreSelectAction::LButtonDown(dPtX, dPtY);

	return TRUE;
}

BOOL MicroJointDelAction::LButtonUp(double& dPtX, double& dPtY)
{
	MicroJointPreSelectAction::LButtonUp(dPtX, dPtY);

	// 删除选中的“微连接特征”。
	if (m_selMjPtPairs.size() > 0)
	{
		MicroJointListPtr pDelMJList(new MicroJointList);
		for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
			pDelMJList->push_back(m_selMjPtPairs.at(i).first);
		ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), m_pLoopFeatureList->GetLoopMicroJointList(), pDelMJList));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 清除选中的“微连接特征”。
		m_pActiveMJPt.reset();
		m_selMjPtPairs.clear();
	}

	return TRUE;
}

BOOL MicroJointDelAction::MovePoint(double& dPtX, double& dPtY)
{
	MicroJointPreSelectAction::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL MicroJointDelAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	MicroJointPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);

	// 删除选中的“微连接特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		if (m_selMjPtPairs.size() > 0)
		{
			MicroJointListPtr pDelMJList(new MicroJointList);
			for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
				pDelMJList->push_back(m_selMjPtPairs.at(i).first);
			ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), m_pLoopFeatureList->GetLoopMicroJointList(), pDelMJList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 清除选中的“微连接特征”。
			m_pActiveMJPt.reset();
			m_selMjPtPairs.clear();
		}
	}

	return TRUE;
}

BOOL MicroJointDelAction::DrawBeforeFinish()
{
	MicroJointPreSelectAction::DrawBeforeFinish();

	return TRUE;
}

void MicroJointDelAction::SelectMJList(MicroJointListPtr pMicroJointList)
{
	ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), m_pLoopFeatureList->GetLoopMicroJointList(), pMicroJointList));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

END_CUTLEADER_NAMESPACE()
