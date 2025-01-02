#include "StdAfx.h"
#include "PatternRotateAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "clUILibResource.h"
#include "CursorDrawer.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PatternList.h"
#include "PatternDrawer.h"
#include "PatternTransformCommand.h"
#include "clGeometryFeatureResource.h"
#include "PartCadData.h"

BEGIN_CUTLEADER_NAMESPACE()

PatternRotateAction::PatternRotateAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort, PatternListPtr pSelPats, TextStructListPtr pSelectedTextStructs)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;
	m_pSelectedPats = pSelPats;
	m_pSelectedTextStructs = pSelectedTextStructs;
}

PatternRotateAction::~PatternRotateAction(void)
{
}

CString PatternRotateAction::GetActionName()
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_ROTATE_PAT);

	return str;
}

void PatternRotateAction::SetMatrixValue()
{
	m_modifyPatMat.Reset();

	Point2D basePoint(m_dBasePtX, m_dBasePtY);
	double angle = atan2((m_dSecondPtY - m_dBasePtY), (m_dSecondPtX - m_dBasePtX)) - atan2((m_dFirstPtY - m_dBasePtY), (m_dFirstPtX - m_dBasePtX));
	m_modifyPatMat.RotateBy(basePoint, angle);
}

void PatternRotateAction::ConvertKBInput(double& dVal1, double& dVal2)
{
	if (m_actionState == TRANSFORM_PATTERN_NOTSTART || m_actionState == TRANSFORM_PATTERN_SECOND_POINT)
	{
		// here, we do not need to convert.
		;
	}
	else if (m_actionState == TRANSFORM_PATTERN_BASE_POINT)
	{
		// "dVal1" holds the rotate angle.

		Point2D basePoint(m_dBasePtX, m_dBasePtY);

		Vector2D v1;
		v1.SetAngle(dVal1*D2R);

		basePoint.Transfer(v1);

		dVal1 = basePoint.X();
		dVal2 = basePoint.Y();
	}
}

int PatternRotateAction::GetMsgID()
{
	int iMsgID = INVALID_ID;

	if (m_actionState == TRANSFORM_PATTERN_NOTSTART ||
		m_actionState == TRANSFORM_PATTERN_SECOND_POINT)
	{
		iMsgID = IDS_OUTPUT_ROTATEPAT_BASEPT;
	}
	else if (m_actionState == TRANSFORM_PATTERN_BASE_POINT)
	{
		iMsgID = IDS_OUTPUT_ROTATEPAT_ANGLE;
	}

	return iMsgID;
}

BOOL PatternRotateAction::DrawBeforeFinish()
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
