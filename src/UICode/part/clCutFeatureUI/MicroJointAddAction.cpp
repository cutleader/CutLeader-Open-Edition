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

#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopCutFeatureList.h"
#include "LoopDrawer.h"
#include "MicroJoint.h"
#include "LoopCutFeature.h"
#include "MicroJointAddCommand.h"
#include "MicroJointList.h"
#include "LoopMicroJoint.h"
#include "MicroJointManager.h"
#include "clCutFeatureResource.h"
#include "LoopToolDataList.h"
#include "LoopToolManager.h"
#include "LoopProcessor.h"

BEGIN_CUTLEADER_NAMESPACE()

MicroJointAddAction::MicroJointAddAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort, ParamConfigPtr pParamConfig)
    : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
    m_iMsgID = IDS_MSG_AddMicroJoint;
	m_pParamConfig = pParamConfig;
}

MicroJointAddAction::~MicroJointAddAction(void)
{
}

BOOL MicroJointAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
	if (!m_pActivePatternLoop) {
		MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_AddMicroJoint), strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	// 得到微连位置。
	int iLoopNodeIndex = INVALID_IDX;
	Point2D microJointPt = LoopProcessor::GetClosestPt(m_pActivePatternLoop.get(), Matrix2D(), Point2D(dPtX, dPtY), iLoopNodeIndex);

	// 检查该位置能不能创建微连。
	const MicroJointConfigItem* pMicroJointConfigItem = dynamic_cast<const MicroJointConfigItem*>(m_pParamConfig->GetItemByType(PARAMCONFIG_ITEM_CUT_MJ_ASSIGN).get());
	double dManualWidth = pMicroJointConfigItem->GetManualWidth();
	IPatternPtr pPattern = m_pActivePatternLoop->GetPatternByLoopNodeIndex(iLoopNodeIndex);
	if (!MicroJointManager::GeomCanHoldMJ(pPattern, microJointPt, dManualWidth)) {
		MessageBox(m_pViewPort->GetWnd(), MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_GEOM_NOT_HOLD_MicroJoint), strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	// 创建微连。
	LoopMicroJointPtr pLoopMicroJoint = m_pLoopFeatureList->FindByPatternLoop(m_pActivePatternLoop)->GetLoopMicroJoint();
	MicroJointPtr pMicroJoint = MicroJointManager::BuildMicroJoint(pLoopMicroJoint, iLoopNodeIndex, microJointPt, dManualWidth, pMicroJointConfigItem);
	ICommandPtr pCommand(new MicroJointAddCommand(GetEditorData(), pLoopMicroJoint, pMicroJoint));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	return TRUE;
}

BOOL MicroJointAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到距离光标最近的几何轮廓。
	IPatternLoopPtr pClosestPatternLoop;
	IPatternPtr pClosestPattern = m_pPatList->GetPatternClosestTo(pt);
	if (pClosestPattern) {
		pClosestPatternLoop = m_pPatternLoopList->FindLoopByPat(pClosestPattern.get());
	}

	// 得到当前捕获到的轮廓。
	m_pActivePatternLoop.reset();
	double dSnapRangeInThisView = CLOptionInfo::GetInstance()->GetSnapPtParam()->GetSnapRangeInThisView(m_pViewPort.get());
	if (pClosestPatternLoop && !pClosestPatternLoop->IsOpenPath() && pClosestPattern->DistanceTo(pt) < dSnapRangeInThisView) {
		m_pActivePatternLoop = pClosestPatternLoop;
	}

	return TRUE;
}

BOOL MicroJointAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

SnapInputPtr MicroJointAddAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	if (m_pActivePatternLoop) {
		PatternListPtr pPatternsInSelectedPatternLoop = m_pActivePatternLoop->GetPatternList();
		pSnapInput.reset(new SnapInput(SnapMiddlePtOf2DLineArc | SnapClosestPtTo2DLineArc, pPatternsInSelectedPatternLoop, Matrix2D()));
	}

	return pSnapInput;
}

BOOL MicroJointAddAction::DrawBeforeFinish()
{
	LoopDrawer loopDrawer(m_pViewPort);
	if (m_pActivePatternLoop) {
		const LoopToolData* pLoopToolData = m_pLoopFeatureList->GetLoopToolList()->GetLoopTool_by_loopID(m_pActivePatternLoop->GetID()).get();
		loopDrawer.DrawActiveLoop(m_pActivePatternLoop.get(), LoopToolManager::GetLoopDisplayColor(pLoopToolData));
	}

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
