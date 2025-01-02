#include "StdAfx.h"
#include "PatternCopyAction.h"

#include "ValueInputManager.h"
#include "CommandManager.h"
#include "GlViewPort.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IPattern.h"
#include "PatternSelectAction.h"
#include "PatternCopyCommand.h"
#include "PatternDrawer.h"
#include "PatternList.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"
#include "TextStruct.h"
#include "TextStructList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternCopyAction::PatternCopyAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelPats, TextStructListPtr pSelectedTextStructs)
{
	m_pTmpPatterns.reset(new PatternList);

	m_pViewPort = pViewPort;
	m_pPartCadData = pPartCadData;
	m_pSelectedPats = pSelPats;
	m_pSelectedTextStructs = pSelectedTextStructs;

	if (pSelPats->size() > 0) { // 预选了几何特征。
		m_iMsgID = IDS_OUTPUT_COPYPAT_BASEPT;
	}

	m_actionState = COPY_PATTERN_NOTSTART;
}

PatternCopyAction::~PatternCopyAction(void)
{
}

BOOL PatternCopyAction::LButtonDown(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonDown(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == COPY_PATTERN_NOTSTART || m_actionState == COPY_PATTERN_DEST_POINT)
		{
			m_actionState = COPY_PATTERN_BASE_POINT;
			m_iMsgID = IDS_OUTPUT_COPYPAT_NEWLOCATION;

			// set the transform matrix.
			m_dBasePtX = dPtX;
			m_dBasePtY = dPtY;
			m_dFirstPtX = dPtX;
			m_dFirstPtY = dPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			SetMatrixValue();

			// create a new copy of selected patterns.
			m_pTmpPatterns->clear();
			for (unsigned int i = 0; i < m_pSelectedPats->size(); i++)
			{
				IPatternPtr pPattern = boost::dynamic_pointer_cast<IPattern>(m_pSelectedPats->operator[](i)->Clone());
				m_pTmpPatterns->push_back(pPattern);
			}
		}
		else if (m_actionState == COPY_PATTERN_BASE_POINT) //select second point
		{
			m_actionState = COPY_PATTERN_DEST_POINT;
			m_iMsgID = IDS_OUTPUT_COPYPAT_BASEPT;

			// set the transform matrix.
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			m_dFirstPtX = m_dBasePtX;
			m_dFirstPtY = m_dBasePtY;
			SetMatrixValue();

			// create command to add the new pattern(s)
			EditorData editorData = GetEditorData();
			PatternListPtr pPatterns_willCopy(new PatternList); // 此处要浅克隆一份几何特征。
			pPatterns_willCopy->insert(pPatterns_willCopy->end(), m_pSelectedPats->begin(), m_pSelectedPats->end());
			vector<LONGLONG> textStructs_willCopy;
			for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
				textStructs_willCopy.push_back(m_pSelectedTextStructs->operator[](i)->GetID());
			ICommandPtr pCommand(new PatternCopyCommand(editorData, m_pPartCadData, pPatterns_willCopy, textStructs_willCopy, m_modifyPatMat));
			pCommand->Do();
			CommandManager::GetInstance()->AddCommand(pCommand);
		}
	}

	return TRUE;
}

BOOL PatternCopyAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::LButtonUp(dPtX, dPtY);
	}

	return TRUE;
}

BOOL PatternCopyAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::MovePoint(dPtX, dPtY);
	}
	else
	{
		if (m_actionState == COPY_PATTERN_BASE_POINT)
		{
			// set the transform matrix.
			m_dFirstPtX = m_dSecondPtX;
			m_dFirstPtY = m_dSecondPtY;
			m_dSecondPtX = dPtX;
			m_dSecondPtY = dPtY;
			SetMatrixValue();

			// update the new copy of the selected patterns.
			m_pTmpPatterns->Transform(m_modifyPatMat);
			m_pTmpPatterns->UpdateCache();
		}
	}

	return TRUE;
}

BOOL PatternCopyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL PatternCopyAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL PatternCopyAction::DrawBeforeFinish()
{
	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

		// draw the selected patterns.
		PatternDrawer patDrawer(m_pViewPort);
		patDrawer.DrawSelPatList(m_pSelectedPats.get());

		// keep the old color.
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);

		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());

		if (m_actionState == COPY_PATTERN_BASE_POINT)
		{
			m_pViewPort->DrawLinePoint2D(m_dBasePtX, m_dBasePtY, m_dSecondPtX, m_dSecondPtY); // draw the trace line.
			m_pTmpPatterns->UpdateCache(); // update the cache data of the pattern object.
			patDrawer.DrawPatList(m_pTmpPatterns.get()); // draw the new copy of the selected patterns.
		}

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);

		// restore the draw color
		m_pViewPort->SetDrawColor(dwOldColor);
	}

	return TRUE;
}

SnapInputPtr PatternCopyAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_pSelectedPats->size() == 0) { // 还没选中几何特征，不要开启捕捉功能。
		pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	}
	else
	{
		DWORD dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
			SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
		pSnapInput.reset(new SnapInput(dwSnapStyle, m_pPartCadData->GetPatternList(), Matrix2D()));
	}

	return pSnapInput;
}

void PatternCopyAction::UpdateForUndoRedo()
{
}

void PatternCopyAction::SetMatrixValue()
{
	m_modifyPatMat.Reset();

	Vector2D vect;
	vect.X((m_dSecondPtX - m_dFirstPtX));
	vect.Y((m_dSecondPtY - m_dFirstPtY));

	m_modifyPatMat.Transfer(vect);
}

END_CUTLEADER_NAMESPACE()
