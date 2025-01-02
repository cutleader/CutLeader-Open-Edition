#include "StdAfx.h"
#include "PatternMoveAction.h"

#include "baseConst.h"
#include "CommandManager.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "clGeometryFeatureResource.h"
#include "PatternTransformCommand.h"
#include "PatternDrawer.h"
#include "PatternList.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

PatternMoveAction::PatternMoveAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelPats, TextStructListPtr pSelectedTextStructs)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;
	m_pSelectedPats = pSelPats;
	m_pSelectedTextStructs = pSelectedTextStructs;
}

PatternMoveAction::~PatternMoveAction(void)
{
}

CString PatternMoveAction::GetActionName()
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_MOVE_PAT);

	return str;
}

void PatternMoveAction::SetMatrixValue()
{
	m_modifyPatMat.Reset();

	Vector2D vect;
	vect.X((m_dSecondPtX - m_dFirstPtX));
	vect.Y((m_dSecondPtY - m_dFirstPtY));

	m_modifyPatMat.Transfer(vect);
}

void PatternMoveAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	// here, we do not need to convert.
}

int PatternMoveAction::GetMsgID()
{
	int iMsgID = INVALID_ID;

	if (m_actionState == TRANSFORM_PATTERN_NOTSTART || m_actionState == TRANSFORM_PATTERN_SECOND_POINT)
	{
		iMsgID = IDS_OUTPUT_MOVEPAT_BASEPT;
	}
	else if (m_actionState == TRANSFORM_PATTERN_BASE_POINT)
	{
		iMsgID = IDS_OUTPUT_MOVEPAT_NEWLOCATION;
	}

	return iMsgID;
}

BOOL PatternMoveAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patternDrawer(m_pViewPort);

	if (m_pSelectedPats->size() == 0) {
		PatternPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// draw the selected patterns.
		patternDrawer.DrawSelPatList(m_pSelectedPats.get());

		// keep the old color.
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);

		//
		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
		if (m_actionState == TRANSFORM_PATTERN_BASE_POINT) {
			m_pViewPort->DrawLinePoint2D(m_dBasePtX, m_dBasePtY, m_dSecondPtX, m_dSecondPtY); // draw the trace line.
			m_pTmpPatterns->UpdateCache(); // update the cache data of the pattern object.
			patternDrawer.DrawPatList(m_pTmpPatterns.get()); // draw the new copy of the selected patterns.
		}

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);

		// restore the draw color
		m_pViewPort->SetDrawColor(dwOldColor);
	}

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
