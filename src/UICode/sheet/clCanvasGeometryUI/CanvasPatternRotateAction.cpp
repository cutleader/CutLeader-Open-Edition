#include "StdAfx.h"
#include "CanvasPatternRotateAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "PartCamData.h"
#include "Part.h"
#include "PartPlacement.h"
#include "clPartLayoutResource.h"
#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "CanvasLoopRotateCommand.h"
#include "CanvasGeometryHelper.h"
#include "PatternLoopList.h"
#include "clCanvasGeometryUIResource.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "GeometryFeatureHelper.h"
#include "PartCadData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternRotateAction::CanvasPatternRotateAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, GlViewPortPtr pViewPort)
    : CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort)
{
	m_pTmpPatterns.reset(new PatternList);
	m_pSelectedPatterns = pSelectedPatterns;
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = pSelectedTextStructs;
	m_actionState = RotatePattern_NOTSTART;

	if (pSelectedPatterns->size() > 0) {
		m_iMsgID = IDS_OUTPUT_ROTATEPAT_BASEPT;
		m_pSelectPatternOrLoopActionBar->HideActionBar();
	}
	else {
		m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Rotate;
		m_pSelectPatternOrLoopActionBar->DisplayActionBar(OnlyEnableLoopSelect);
	}
}

CanvasPatternRotateAction::~CanvasPatternRotateAction(void)
{
}

BOOL CanvasPatternRotateAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == RotatePattern_NOTSTART || m_actionState == RotatePattern_SECOND_PT_SELECTED) {
			m_actionState = RotatePattern_BASE_PT_SELECTED;
			m_iMsgID = IDS_OUTPUT_ROTATEPAT_ANGLE;

			// update the transform matrix.
			m_dBasePtX = dPtX;
			m_dBasePtY = dPtY;
			m_dFirstPtX = dPtX;
			m_dFirstPtY = dPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			CalcMatrixValue();

			m_pTmpPatterns = boost::dynamic_pointer_cast<PatternList>(m_pSelectedPatterns->Clone());
			m_pTmpPatterns->UpdateCache();
		}
		else if (m_actionState == RotatePattern_BASE_PT_SELECTED)
		{
			m_actionState = RotatePattern_SECOND_PT_SELECTED;

			// update the transform matrix.
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			m_dFirstPtX = m_dBasePtX;
			m_dFirstPtY = m_dBasePtY;
			CalcMatrixValue();

			// 要旋转的几何轮廓。
			vector<LONGLONG> patternLoops_willRotate;
			PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
			PatternLoopListPtr pPatternLoops_willRotate = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get());
			for (unsigned int i = 0; i < pPatternLoops_willRotate->size(); i++)
				patternLoops_willRotate.push_back(pPatternLoops_willRotate->operator[](i)->GetID());

			// 要旋转的文字。
			vector<LONGLONG> textStructs_willMove;
			for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
				textStructs_willMove.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

			// 执行命令。
			PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
			ICommandPtr pCommand(new CanvasLoopRotateCommand(GetEditorData(), pPartCadData, pPartCamData, patternLoops_willRotate, textStructs_willMove, m_modifyMatrix));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);

			PostMessage2EnterCanvasLoopSelectAction_PreSelectLoopAndText(pPatternLoops_willRotate.get(), m_pSelectedTextStructs.get()); // 选中编辑的几何轮廓。
			ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
			ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		}
	}

	return TRUE;
}

BOOL CanvasPatternRotateAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		CanvasPatternPreSelectAction::LButtonUp(dPtX, dPtY);

		if (m_pSelectedPatterns->size() > 0) {
			m_iMsgID = IDS_OUTPUT_ROTATEPAT_BASEPT;
			m_pSelectPatternOrLoopActionBar->HideActionBar();
		}
	}

	return TRUE;
}

BOOL CanvasPatternRotateAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) {
		__super::MovePoint(dPtX, dPtY);
	}
	else {
		if (m_actionState == RotatePattern_NOTSTART) {
			if (m_pSelectedPatterns->size() > 0)
				m_iMsgID = IDS_OUTPUT_ROTATEPAT_BASEPT;
			else
				m_iMsgID = IDS_OUTPUT_SELECTPATTERN;
		}
		else if (m_actionState == RotatePattern_BASE_PT_SELECTED) {
			// set the transform matrix.
			m_dFirstPtX = m_dSecondPtX;
			m_dFirstPtY = m_dSecondPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			CalcMatrixValue();

			// 更新拷贝出来的临时几何特征。
			for (unsigned int i = 0; i < m_pTmpPatterns->size(); i++)
				m_pTmpPatterns->operator[](i)->Transform(m_modifyMatrix);
			m_pTmpPatterns->UpdateCache();
		}
	}

	return TRUE;
}

SnapInputPtr CanvasPatternRotateAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	DWORD dwSnapStyle = 0;
	if (m_pSelectedPatterns->size() == 0)
	{
		pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	}
	else
	{
		dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
			SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		pSnapInput.reset(new SnapInput(dwSnapStyle, pPartCadData->GetPatternList(), Matrix2D()));
	}

	return pSnapInput;
}

BOOL CanvasPatternRotateAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasPatternRotateAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patDrawer(m_pViewPort);

	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// draw the selected patterns.
		DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

		if (m_actionState == RotatePattern_BASE_PT_SELECTED) {
			// 绘制牵引线。
			COLORREF dwOldColor;
			m_pViewPort->GetDrawColor(dwOldColor);
			m_pViewPort->SetDrawColor(*pDisplayParam->GetDrawAssistColor());
			m_pViewPort->DrawLinePoint2D(m_dBasePtX, m_dBasePtY, m_dSecondPtX, m_dSecondPtY);
			m_pViewPort->SetDrawColor(dwOldColor);

			// 绘制移动中的几何特征。
			patDrawer.DrawPatList(m_pTmpPatterns.get());
		}

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasPatternRotateAction::UpdateForUndoRedo()
{
}

void CanvasPatternRotateAction::CalcMatrixValue()
{
	m_modifyMatrix.Reset();
	Point2D basePoint(m_dBasePtX, m_dBasePtY);
	double dAngle = atan2((m_dSecondPtY - m_dBasePtY), (m_dSecondPtX - m_dBasePtX)) - atan2((m_dFirstPtY - m_dBasePtY), (m_dFirstPtX - m_dBasePtX));
	m_modifyMatrix.RotateBy(basePoint, dAngle);
}

END_CUTLEADER_NAMESPACE()
