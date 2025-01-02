#include "StdAfx.h"
#include "PatternTransformActionBase.h"

#include "ValueInputManager.h"
#include "CommandManager.h"
#include "GlViewPort.h"
#include "clUILibResource.h"
#include "clBaseDataResource.h"

#include "PatternSelectAction.h"
#include "PatternTransformCommand.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"
#include "TextStructList.h"
#include "TextStruct.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternTransformActionBase::PatternTransformActionBase(void)
{
	m_actionState = TRANSFORM_PATTERN_NOTSTART;
	m_pTmpPatterns.reset(new PatternList);
}

PatternTransformActionBase::~PatternTransformActionBase(void)
{
}

BOOL PatternTransformActionBase::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == TRANSFORM_PATTERN_NOTSTART || m_actionState == TRANSFORM_PATTERN_SECOND_POINT) // we selected base point.
		{
			m_actionState = TRANSFORM_PATTERN_BASE_POINT;
			m_iMsgID = GetMsgID();

			// set the transform matrix.
			m_dBasePtX = dPtX;
			m_dBasePtY = dPtY;
			m_dFirstPtX = dPtX;
			m_dFirstPtY = dPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			SetMatrixValue();

			// create a new copy of selected patterns.
			for (unsigned int i = 0; i < m_pSelectedPats->size(); i++) {
				IDataPtr pTmpData = m_pSelectedPats->at(i)->Clone();
				IPatternPtr pPattern = boost::dynamic_pointer_cast<IPattern>(pTmpData);
				m_pTmpPatterns->push_back(pPattern);
			}
		}
		else if (m_actionState == TRANSFORM_PATTERN_BASE_POINT) // we selected the second point.
		{
			m_actionState = TRANSFORM_PATTERN_SECOND_POINT;
			m_iMsgID = GetMsgID();

			// set the transform matrix.
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			m_dFirstPtX = m_dBasePtX;
			m_dFirstPtY = m_dBasePtY;
			SetMatrixValue();

			// create command to update the pattern(s)
			{
				vector<LONGLONG> patterns_willTransform;
				vector<Matrix2D> patternTransformMatrixData;
				for (unsigned int i = 0; i < m_pSelectedPats->size(); i++) {
					patterns_willTransform.push_back(m_pSelectedPats->operator[](i)->GetID());
					patternTransformMatrixData.push_back(m_modifyPatMat);
				}

				vector<LONGLONG> textStructs_willTransform;
				vector<Matrix2D> textTransformMatrixList;
				for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++) {
					textStructs_willTransform.push_back(m_pSelectedTextStructs->operator[](i)->GetID());
					textTransformMatrixList.push_back(m_modifyPatMat);
				}

				ICommandPtr pCommand(new PatternTransformCommand(GetEditorData(), GetActionName(), m_pPartCadData, patterns_willTransform, patternTransformMatrixData,
					textStructs_willTransform, textTransformMatrixList));
				pCommand->Do();
				CommandManager::GetInstance()->AddCommand(pCommand);
			}

			// enter the selecting pattern action.
			PostMessage2EnterPatternSelectAction(m_pSelectedPats.get(), m_pSelectedTextStructs.get());
		}
	}

	return TRUE;
}

BOOL PatternTransformActionBase::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	// 检查不能选中阵列。
	if (GetActionType() == EDIT_ROTATE)
	{
		for (unsigned int i = 0; i < m_pSelectedPats->size(); i++)
		{
			if (m_pSelectedPats->at(i)->IsArrayPattern()) {
				CString strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_ROTATEPAT_ARRAY);
				MessageBox(m_pViewPort->GetWnd(), strMsg, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
				return TRUE;
			}
		}
	}

	return TRUE;
}

BOOL PatternTransformActionBase::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::MovePoint(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == TRANSFORM_PATTERN_NOTSTART)
		{
			m_iMsgID = GetMsgID();
		}
		else if (m_actionState == TRANSFORM_PATTERN_BASE_POINT)
		{
			// set the transform matrix.
			m_dFirstPtX = m_dSecondPtX;
			m_dFirstPtY = m_dSecondPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			SetMatrixValue();

			// update the new copy of the selected patterns.
			for (unsigned int i = 0; i < m_pTmpPatterns->size(); i++)
			{
				m_pTmpPatterns->at(i)->Transform(m_modifyPatMat);
			}
		}
	}

	return TRUE;
}

BOOL PatternTransformActionBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

SnapInputPtr PatternTransformActionBase::GetSnapInput() const
{
	SnapInputPtr pSnapInput;

	if (m_pSelectedPats->size() == 0) {
		pSnapInput = SnapInput::BuildSnapInput4SnapNone(); // 不要开启捕捉功能。
	}
	else
	{
		DWORD dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
			SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		pSnapInput.reset(new SnapInput(dwSnapStyle, m_pPartCadData->GetPatternList(), Matrix2D()));
	}

	return pSnapInput;
}

void PatternTransformActionBase::UpdateForUndoRedo()
{
}

END_CUTLEADER_NAMESPACE()
