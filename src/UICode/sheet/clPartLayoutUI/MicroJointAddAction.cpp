#include "StdAfx.h"
#include "MicroJointAddAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "ParamConfig.h"
#include "MicroJointConfigItem.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "SnapPtParam.h"
#include "IMaterialSize.h"

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "PatternDrawer.h"
#include "IArrayPattern.h"
#include "LoopCutFeatureList.h"
#include "LoopDrawer.h"
#include "MicroJoint.h"
#include "LoopCutFeature.h"
#include "MicroJointAddCommand.h"
#include "MicroJointList.h"
#include "LoopMicroJoint.h"
#include "MicroJointManager.h"
#include "clCutFeatureResource.h"
#include "LoopCutFeatureDrawer.h"
#include "LoopToolManager.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"
#include "LoopInstanceList.h"
#include "MicroJointsAddCommand.h"
#include "LoopProcessor.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

MicroJointAddAction::MicroJointAddAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
	GlViewPortPtr pViewPort, ParamConfigPtr pParamConfig, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
    m_iMsgID = IDS_MSG_AddMicroJoint;
	m_pParamConfig = pParamConfig;
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(pPartInstances);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas) {
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
	}
}

MicroJointAddAction::~MicroJointAddAction(void)
{
}

BOOL MicroJointAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (!m_pActiveLoopInstance) {
		MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AddMicroJoint), strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	const MicroJointConfigItem* pMicroJointConfigItem = dynamic_cast<const MicroJointConfigItem*>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_CUT_MJ_ASSIGN).get());
	double dManualWidth = pMicroJointConfigItem->GetManualWidth();
	const IPatternLoop* pPatternLoop = m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get();

	// 得到微连位置(零件坐标系下)。
	int iLoopNodeIndex = INVALID_IDX;
	Point2D microJointPt = LoopProcessor::GetClosestPt(pPatternLoop, m_pActiveLoopInstance->GetTransformMatrix(), Point2D(dPtX, dPtY), iLoopNodeIndex);
	microJointPt = microJointPt.InverseTransform(m_pActiveLoopInstance->GetTransformMatrix()); // 转到零件坐标系下面。

	// check whether the geom can hold the micro joint.
	IPatternPtr pPattern = pPatternLoop->GetPatternByLoopNodeIndex(iLoopNodeIndex);
	if (!MicroJointManager::GeomCanHoldMJ(pPattern, microJointPt, dManualWidth)) {
		MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_GEOM_NOT_HOLD_MicroJoint), strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	LoopMicroJointPtr pSelectedLoopMicroJoint = m_pActiveLoopInstance->GetCacheData()->GetLoopMicroJoint();
	if (m_pSpreadSamePartActionBar->IsSpread())
	{
		// 需要新增的微连。
		vector<pair<LoopMicroJointPtr, MicroJointPtr>> microJointDataList;
		for (unsigned int i = 0; i < m_pAllLoopInstances->size(); i++) {
			LoopInstancePtr pLoopInstance = m_pAllLoopInstances->operator [](i);
			if (pLoopInstance->GetCacheData()->GetPatternLoop()->GetID() == pSelectedLoopMicroJoint->GetPatternLoop()->GetID()) {
				LoopMicroJointPtr pLoopMicroJoint = pLoopInstance->GetCacheData()->GetLoopMicroJoint();

				// 对于零件阵列，这里需要防止重复的创建微连。
				bool bIgnoreThisLoopInstance = false;
				for (unsigned int j = 0; j < microJointDataList.size(); j++) {
					if (microJointDataList[j].first->GetID() == pLoopMicroJoint->GetID()) {
						bIgnoreThisLoopInstance = true;
						break;
					}
				}
				if (bIgnoreThisLoopInstance) {
					continue;
				}

				MicroJointPtr pMicroJoint = MicroJointManager::BuildMicroJoint(pLoopMicroJoint, iLoopNodeIndex, microJointPt, dManualWidth, pMicroJointConfigItem);
				microJointDataList.push_back(make_pair(pLoopMicroJoint, pMicroJoint));
			}
		}

		ICommandPtr pCommand(new MicroJointsAddCommand(GetEditorData(), microJointDataList));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}
	else {
		MicroJointPtr pMicroJoint = MicroJointManager::BuildMicroJoint(pSelectedLoopMicroJoint, iLoopNodeIndex, microJointPt, dManualWidth, pMicroJointConfigItem);
		ICommandPtr pCommand(new MicroJointAddCommand(GetEditorData(), pSelectedLoopMicroJoint, pMicroJoint));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);
	}

	return TRUE;
}

BOOL MicroJointAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到当前捕获到的轮廓实例。
	m_pActiveLoopInstance.reset();
	double dSnapRangeInThisView = CLOptionInfo::GetInstance()->GetSnapPtParam()->GetSnapRangeInThisView(m_pViewPort.get());
	LoopInstancePtr pLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pAllLoopInstances, pt);
	if (pLoopInstance && !pLoopInstance->GetCacheData()->GetPatternLoop()->IsOpenPath() && pLoopInstance->DistanceTo(pt) < dSnapRangeInThisView) {
		m_pActiveLoopInstance = pLoopInstance;
	}

	return TRUE;
}

BOOL MicroJointAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

SnapInputPtr MicroJointAddAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	if (m_pActiveLoopInstance) {
		PatternListPtr pPatternsInSelectedLoopInst = m_pActiveLoopInstance->GetCacheData()->GetPatternLoop()->GetPatternList();
		pSnapInput.reset(new SnapInput(SnapMiddlePtOf2DLineArc | SnapClosestPtTo2DLineArc, pPatternsInSelectedLoopInst, m_pActiveLoopInstance->GetTransformMatrix()));
	}

	return pSnapInput;
}

BOOL MicroJointAddAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);
	if (m_pActiveLoopInstance) {
		COLORREF iColor = LoopToolManager::GetLoopDisplayColor(m_pActiveLoopInstance->GetCacheData()->GetLoopTool().get());
		loopDrawer.DrawActiveLoop(m_pActiveLoopInstance->GetCacheData()->GetPatternLoop().get(), iColor, m_pActiveLoopInstance->GetTransformMatrix());
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
