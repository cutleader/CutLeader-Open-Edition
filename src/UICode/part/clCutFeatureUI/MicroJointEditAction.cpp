#include "StdAfx.h"
#include "MicroJointEditAction.h"

#include "ValueInputManager.h"
#include "DummyAction.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IPattern.h"
#include "IArrayPattern.h"
#include "PatternLoopNode.h"
#include "PatternLoopNodeList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "PatternList.h"
#include "LoopMicroJointList.h"
#include "MicroJointList.h"
#include "MicroJoint.h"
#include "LoopMicroJoint.h"
#include "LoopCutFeatureList.h"
#include "LoopCutFeature.h"
#include "MicroJointPtModifyCommand.h"
#include "MicroJointDeleteCommand.h"
#include "MicroJointPropertyBar.h"
#include "LoopCutFeatureDrawer.h"
#include "clCutFeatureResource.h"
#include "MicroJointManager.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

MicroJointEditAction::MicroJointEditAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
    : MicroJointPreSelectAction(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
	MicroJointPropertyBar::GetInstance()->IsChangingMJPt(FALSE);
}

MicroJointEditAction::~MicroJointEditAction(void)
{
}

BOOL MicroJointEditAction::LButtonDown(double& dPtX, double& dPtY)
{
	BOOL bChangingMJPt = MicroJointPropertyBar::GetInstance()->IsChangingMJPt();
	if (!bChangingMJPt)
	{
		MicroJointPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		// 要调整的微连接特征。
		MicroJointPtr pMJ = m_selMjPtPairs.at(0).first;

		// 调整微连接点。
		BOOL bDir;
		IPatternPtr pPattern = m_pActiveLoop->GetPatternByLoopNodeIndex(m_iPatIndex, bDir);
		if (MicroJointManager::GeomCanHoldMJ(pPattern, m_newPt, pMJ->GetWidth()))
		{
			// 修改微连接点。
			PatternPosition secondPatPos;
			PatternPosition patternPosition = pPattern->GetPatPosByPoint(m_newPt, secondPatPos);
			ICommandPtr pCommand(new MicroJointPtModifyCommand(GetEditorData(), pMJ, pPattern->GetID(), patternPosition));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			// 显示调整位置后的微连接特征信息。
			MicroJointPropertyBar::GetInstance()->IsChangingMJPt(FALSE);
			MicroJointListPtr pMicroJointList(new MicroJointList);
			pMicroJointList->push_back(m_selMjPtPairs.at(0).first);
			Point2DListPtr pMJPtList(new Point2DList);
			pMJPtList->push_back(m_selMjPtPairs.at(0).second);
			MicroJointPropertyBar::GetInstance()->DisplayMJData(GetEditorData(), m_pLoopFeatureList->GetLoopMicroJointList(), pMicroJointList, pMJPtList);

			// 更新当前的微连接点和选中的微连接点。
			m_pActiveMJPt.reset(new Point2D(m_newPt));
			m_selMjPtPairs.at(0).second = m_newPt;
		}
		else
		{
			CString str, strProductName;
			strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_GEOM_NOT_HOLD_MicroJoint);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
		}
	}

	return TRUE;
}

BOOL MicroJointEditAction::LButtonUp(double& dPtX, double& dPtY)
{
	BOOL bChangingMJPt = MicroJointPropertyBar::GetInstance()->IsChangingMJPt();
	if (!bChangingMJPt)
	{
		MicroJointPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_selMjPtPairs.size() > 0)
		{
			// 更新当前回路。
			if (m_selMjPtPairs.size() == 1)
			{
				LoopMicroJointListPtr pLoopMicroJointList = m_pLoopFeatureList->GetLoopMicroJointList();
				LoopMicroJointPtr pLoopMicroJoint = pLoopMicroJointList->GetItemByID(m_selMjPtPairs.at(0).first->GetParentID());
				m_pActiveLoop = pLoopMicroJoint->GetPatternLoop();
			}

			// 显示微连接编辑界面。
			MicroJointListPtr pMicroJointList(new MicroJointList);
			Point2DListPtr pMJPtList(new Point2DList);
			for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
			{
				pMicroJointList->push_back(m_selMjPtPairs.at(i).first);
				pMJPtList->push_back(m_selMjPtPairs.at(i).second);
			}
			MicroJointPropertyBar::GetInstance()->DisplayMJData(GetEditorData(), m_pLoopFeatureList->GetLoopMicroJointList(), pMicroJointList, pMJPtList);
		}
	}

	return TRUE;
}

BOOL MicroJointEditAction::MovePoint(double& dPtX, double& dPtY)
{
	BOOL bChangingMJPt = MicroJointPropertyBar::GetInstance()->IsChangingMJPt();
	if (!bChangingMJPt)
	{
		MicroJointPreSelectAction::MovePoint(dPtX, dPtY);
	}
	else
	{
		// 新的微连接点。
		Point2D pt(dPtX, dPtY);
		if (m_pActiveLoop->GetPatternLoopType() == PatternLoop_Array)
		{
			PatternLoopNodePtr pPatternLoopNode = m_pActiveLoop->GetPatternLoopNodes()->at(0);
			IArrayPatternPtr pArrayPattern = boost::dynamic_pointer_cast<IArrayPattern>(pPatternLoopNode->GetPattern());
			IPatternPtr pFirstPat = pArrayPattern->GetFirstInstance();
			m_newPt = pFirstPat->GetClosestPt(pt, Matrix2D());
			m_iPatIndex = 0;
		}
		else
			m_pActiveLoop->GetClosestPt(pt, m_newPt, m_iPatIndex, Matrix2D());
	}

	return TRUE;
}

BOOL MicroJointEditAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bChangingMJPt = MicroJointPropertyBar::GetInstance()->IsChangingMJPt();
	if (bChangingMJPt)
	{
		if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
		{
			MicroJointPropertyBar::GetInstance()->IsChangingMJPt(FALSE);
			MicroJointPropertyBar::GetInstance()->ShowBar(FALSE);
		}
	}
	else
	{
		MicroJointPreSelectAction::OnKeyDown(nChar, nRepCnt, nFlags);

		// 选中全部“微连接特征”。
		if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar))
		{
			// 显示选中的“微连接特征”属性。
			if (m_selMjPtPairs.size() > 0)
			{
				MicroJointListPtr pMicroJointList(new MicroJointList);
				Point2DListPtr pMJPtList(new Point2DList);
				for (unsigned int i = 0; i < m_selMjPtPairs.size(); i++)
				{
					pMicroJointList->push_back(m_selMjPtPairs.at(i).first);
					pMJPtList->push_back(m_selMjPtPairs.at(i).second);
				}
				MicroJointPropertyBar::GetInstance()->DisplayMJData(GetEditorData(), m_pLoopFeatureList->GetLoopMicroJointList(), pMicroJointList, pMJPtList);
			}
		}
		else if (ValueInputManager::GetInstance()->IsInputDel(nChar))
		{
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

				// 隐藏微连接属性界面。
				MicroJointPropertyBar::GetInstance()->ShowBar(FALSE);
			}
		}
	}

	return TRUE;
}

BOOL MicroJointEditAction::DrawBeforeFinish()
{
	BOOL bChangingMJPt = MicroJointPropertyBar::GetInstance()->IsChangingMJPt();
	if (!bChangingMJPt)
	{
		MicroJointPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// 绘制环。
		LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);
		loopFeatureDrawer.DrawPosRing(m_newPt);

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

void MicroJointEditAction::UpdateForUndoRedo()
{
	// 不显示“当前的”和“选中的”微连接点。
	m_pActiveMJPt.reset();
	m_selMjPtPairs.clear();

	// 隐藏微连接属性界面。
	MicroJointPropertyBar::GetInstance()->ShowBar(FALSE);
}

END_CUTLEADER_NAMESPACE()
