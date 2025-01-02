#include "StdAfx.h"
#include "PatternLoopOffsetAction.h"

#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "IPattern.h"
#include "PatternList.h"
#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "LoopDrawer.h"
#include "LoopProcessor.h"
#include "clGeometryFeatureResource.h"
#include "PatternLoopOffsetDlg.h"
#include "PartCadData.h"
#include "TextStructList.h"

BEGIN_CUTLEADER_NAMESPACE()

PatternLoopOffsetAction::PatternLoopOffsetAction(PartCadDataPtr pPartCadData, GlViewPortPtr pViewPort)
{
	m_iMsgID = IDS_OUTPUT_OFFSETLOOP_SELECTLOOP;
	m_pViewPort = pViewPort;

	m_pPartCadData = pPartCadData;
	m_state = OFFSET_LOOP_NOTSTART;
	m_pActiveLoop.reset();
}

PatternLoopOffsetAction::~PatternLoopOffsetAction(void)
{
}

BOOL PatternLoopOffsetAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strMsg, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pActiveLoop)
	{
		// 检查是不是闭合回路。
		if (m_pActiveLoop->IsOpenPath())
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_OFFSETLOOP_OPEN_LOOP);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
		}
		else
		{
			// 设置偏移参数。
			PatternLoopOffsetDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				// 执行偏移。
				BOOL bInside = dlg.IsInside();
				double dOffsetVal = dlg.GetOffsetVal();
				PatternListPtr pPatList = m_pPartCadData->GetPatternList();
				if (!LoopProcessor::OffsetPolyLoop(pPatList, m_pActiveLoop, bInside, dOffsetVal))
				{
					strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_OFFSETLOOP_PROP_VAL);
					MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
				}
			}
		}

		// 进入选择图形模式。
		TextStructListPtr pTextStructs(new TextStructList);
		PostMessage2EnterPatternSelectAction(m_pActiveLoop->GetPatternList().get(), pTextStructs.get());
	}

	return TRUE;
}

BOOL PatternLoopOffsetAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// get the loop which is nearest to the cursor.
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	IPatternPtr pPattern = pPatList->GetPatternClosestTo(pt);
	if (pPattern)
    {
		m_pActiveLoop = pPatternLoopList->FindLoopByPat(pPattern.get());
    }

	return TRUE;
}

BOOL PatternLoopOffsetAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL PatternLoopOffsetAction::DrawBeforeFinish()
{
	// get the color setting.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pDisplayParam = pCLOptionInfo->GetDisplayParam();

	// keep the old setting.
	COLORREF dwOldColor;
	ROP_MODE ropMode;
	m_pViewPort->GetDrawColor(dwOldColor);
	m_pViewPort->GetROP(ropMode);

	/************************************************************************/
	// draw the selected loop in highlight mode.

	if (m_pActiveLoop)
	{
		m_pViewPort->SetROP(ROP_COPY);
		m_pViewPort->SetLineWidth(2);
		m_pViewPort->SetDrawColor(*pDisplayParam->GetActiveObjColor());

		// draw the loop.
		LoopDrawer loopDrawer(m_pViewPort);
		loopDrawer.DrawLoopData(m_pActiveLoop.get());

		m_pViewPort->SetLineWidth(1);
	}
	/************************************************************************/

	// restore the draw setting.
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetROP(ropMode);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
