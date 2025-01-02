#include "StdAfx.h"
#include "MicroJointDeleteAction2.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "Point2DList.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"

#include "IMaterialSize.h"

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
#include "PartInstanceList.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "PartPlacementList.h"
#include "LoopInstanceManager.h"

BEGIN_CUTLEADER_NAMESPACE()

MicroJointDeleteAction2::MicroJointDeleteAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
	GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : MicroJointPreSelectAction2(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_iMsgID = IDS_CUTFEATURE_SELECT_MicroJoint;
	m_pAllLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(pPartInstances);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas)
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
}

MicroJointDeleteAction2::~MicroJointDeleteAction2(void)
{
}

BOOL MicroJointDeleteAction2::LButtonDown(double& dPtX, double& dPtY)
{
	MicroJointPreSelectAction2::LButtonDown(dPtX, dPtY);

	return TRUE;
}

BOOL MicroJointDeleteAction2::LButtonUp(double& dPtX, double& dPtY)
{
	MicroJointPreSelectAction2::LButtonUp(dPtX, dPtY);

	// 删除选中的“微连接特征”。
	if (m_selMjPtPairs.size() > 0)
	{
		LoopMicroJointListPtr pLoopMicroJointList = m_pPartPlacementList->GetAllLoopMicroJointList();
		if (m_pSpreadSamePartActionBar->IsSpread())
		{
			// 希望删除的微连接
			MicroJointListPtr pMJList_involveSameParts(new MicroJointList);
			for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
			{
				MicroJointPtr pMJ = m_selMjPtPairs.at(i).first;
				Point2D microjointPt = m_selMjPtPairs.at(i).second;
				LoopInstancePtr pLoopInstance = m_pAllLoopInstanceList->GetInstanceByMicroJointPt(microjointPt); // 微连接所在轮廓

				// 把这个微连接点转到part模式 
				Point2D mjPt_underPartMode = microjointPt;
				Matrix2D mat = pLoopInstance->GetTransformMatrix();
				mat.Inverse();
				mjPt_underPartMode *= mat;

				for (unsigned int j = 0; j < m_pAllLoopInstanceList->size(); j++)
				{
					LoopInstancePtr pLoopInstance_tmp = m_pAllLoopInstanceList->at(j);
					if (pLoopInstance_tmp->GetPatternLoopID() == pLoopInstance->GetPatternLoopID())
					{
						LoopMicroJointPtr pLoopMicroJoint = pLoopInstance_tmp->GetCacheData()->GetLoopMicroJoint();
						MicroJointPtr pMicroJoint = pLoopMicroJoint->GetMicroJointAtLocation(mjPt_underPartMode);
						if (pMicroJoint)
						{
							pMJList_involveSameParts->push_back(pMicroJoint);
						}
					}
				}
			}

			ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), pLoopMicroJointList, pMJList_involveSameParts));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
		else
		{
			// 希望删除的微连接
			MicroJointListPtr pDelMJList(new MicroJointList);
			for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
			{
				pDelMJList->push_back(m_selMjPtPairs.at(i).first);
			}

			ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), pLoopMicroJointList, pDelMJList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}

		// 清除选中的“微连接特征”。
		m_pActiveMJPt.reset();
		m_selMjPtPairs.clear();
	}

	return TRUE;
}

BOOL MicroJointDeleteAction2::MovePoint(double& dPtX, double& dPtY)
{
	MicroJointPreSelectAction2::MovePoint(dPtX, dPtY);

	return TRUE;
}

BOOL MicroJointDeleteAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	MicroJointPreSelectAction2::OnKeyDown(nChar, nRepCnt, nFlags);

	// 删除选中的“微连接特征”。
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
	{
		if (m_selMjPtPairs.size() > 0)
		{
			LoopMicroJointListPtr pLoopMicroJointList = m_pPartPlacementList->GetAllLoopMicroJointList();
			MicroJointListPtr pDelMJList(new MicroJointList);
			for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
				pDelMJList->push_back(m_selMjPtPairs.at(i).first);
			ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), pLoopMicroJointList, pDelMJList));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 清除选中的“微连接特征”。
			m_pActiveMJPt.reset();
			m_selMjPtPairs.clear();
		}
	}

	return TRUE;
}

BOOL MicroJointDeleteAction2::DrawBeforeFinish()
{
	MicroJointPreSelectAction2::DrawBeforeFinish();

	return TRUE;
}

void MicroJointDeleteAction2::SelectMJList(MicroJointListPtr pMicroJointList)
{
	LoopMicroJointListPtr pLoopMicroJointList = m_pPartPlacementList->GetAllLoopMicroJointList();
	ICommandPtr pCommand(new MicroJointDeleteCommand(GetEditorData(), pLoopMicroJointList, pMicroJointList));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);
}

END_CUTLEADER_NAMESPACE()
