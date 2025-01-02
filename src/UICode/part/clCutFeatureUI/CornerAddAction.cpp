#include "StdAfx.h"
#include "CornerAddAction.h"

#include "MathEx.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "LineArc2DList.h"
#include "GlViewPort.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "IPatternLoop.h"
#include "PatternLoopList.h"
#include "PatternList.h"
#include "PolygonPatternLoop.h"
#include "LoopBuilder.h"
#include "LoopProcessor.h"

#include "CornerPropertyBar.h"
#include "CornerAddCommand.h"
#include "LoopCutFeatureList.h"
#include "LoopCornerData.h"
#include "LoopCutFeature.h"
#include "CornerManager.h"
#include "clCutFeatureResource.h"

BEGIN_CUTLEADER_NAMESPACE()

CornerAddAction::CornerAddAction(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems, 
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
    : CutFeatureEditBase(pPatList, pPatternLoopList, pLoopTopologyItems, pLoopFeatureList, pViewPort)
{
    m_iMsgID = IDS_CUTFEATURE_SELECT_CORNER_PT;
	m_state = CORNER_STATE_NONE;
}

CornerAddAction::~CornerAddAction(void)
{
}

BOOL CornerAddAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pCornerPt)
	{
		// the corner feature relative to this pattern loop.
		LoopCutFeaturePtr pLoopFeature = m_pLoopFeatureList->FindByPatternLoop(m_pActiveLoop);
		LoopCornerDataPtr pLoopCorner = pLoopFeature->GetLoopCorner();

		// 检查指定位置是不是有了角特征。
		if (pLoopCorner->GetCornerAtLoc(*m_pCornerPt))
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_EXIST);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	
		// 不能在光滑的地方创建角特征。
		if (!m_pActiveLoop->IsOpenPath())
		{
			double dCornerAngle = LoopProcessor::GetAngAtLoopCorner(m_pActiveLoop.get(), *(m_pCornerPt.get()));
			if (MathEx::Equal(dCornerAngle, PI, GEOM_TOLERANCE))
			{
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_ASSIGN_SMOOTH_CORNER);
				MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
				return FALSE;
			}
		}
		else
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_OPEN_LOOP);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
			return FALSE;
		}

		// 创建“角特征”。
		CornerType cornerType = CornerPropertyBar::GetInstance()->GetCornerType();
        if (CornerManager::WhetherHaveCornerValue(cornerType))
        {
            double dVal = CornerPropertyBar::GetInstance()->GetCornerValue();
            if (cornerType != Corner_None && dVal != 0)
            {
                // 创建角特征，然后检查它。
                const PolygonPatternLoop* pPolygonPatternLoop = dynamic_cast<const PolygonPatternLoop*>(m_pActiveLoop.get());
                ICornerPtr pCorner = CornerManager::BuildCorner(pLoopCorner->GetID(), pPolygonPatternLoop, *m_pCornerPt, cornerType, dVal);
                if (!CornerManager::CheckCornerValue(pPolygonPatternLoop, pCorner.get()))
                {
                    str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_WRONG_VAL);
                    MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
                    return FALSE;
                }

                // 添加角特征。
                ICommandPtr pCommand(new CornerAddCommand(GetEditorData(), pLoopCorner, pCorner));
                pCommand->Do();
                CommandManager::GetInstance()->AddCommand(pCommand);
            }
            else
            {
                str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_WRONG_VAL);
                MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
            }
        }
        else
        {
            // 创建角特征。
            const PolygonPatternLoop* pPolygonPatternLoop = dynamic_cast<const PolygonPatternLoop*>(m_pActiveLoop.get());
            ICornerPtr pCorner = CornerManager::BuildCorner(pLoopCorner->GetID(), pPolygonPatternLoop, *m_pCornerPt, cornerType, -1);

            // 添加角特征。
            ICommandPtr pCommand(new CornerAddCommand(GetEditorData(), pLoopCorner, pCorner));
            pCommand->Do();
            CommandManager::GetInstance()->AddCommand(pCommand);
        }
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_SELECT_CORNER_PT);
		MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

BOOL CornerAddAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// get the pattern which is closest to cursor.
	IPatternPtr pPattern = m_pPatList->GetPatternClosestTo(pt);

	// if no pattern exist, return.
	if (!pPattern)
    {
		return FALSE;
    }

	// get the loop that contains this pattern
	m_pActiveLoop = m_pPatternLoopList->FindLoopByPat(pPattern.get());

	// only can assign corner to polygon loop.
	if (m_pActiveLoop->GetPatternLoopType() == PatternLoop_Polygon)
	{
		PolygonPatternLoopPtr pPolygonPatternLoop = boost::dynamic_pointer_cast<PolygonPatternLoop>(m_pActiveLoop);

		// get the closest corner point to the cursor.
		Point2DListPtr pPtList = pPolygonPatternLoop->GetCtrlPtList(TRUE);
		m_pCornerPt.reset(new Point2D(pPtList->GetPtClosestTo(pt)));
	}
	else
	{
		m_pActiveLoop.reset();
		m_pCornerPt.reset();
	}

	return TRUE;
}

BOOL CornerAddAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		CornerPropertyBar::GetInstance()->ShowPane(FALSE, FALSE, TRUE);
	}
    else
    {
        CutFeatureEditBase::OnKeyDown(nChar, nRepCnt, nFlags);
    }

	return true;
}

BOOL CornerAddAction::DrawBeforeFinish()
{
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	// draw the corner point
	m_pViewPort->SetDrawColor(RGB(255, 0, 0));
	if (m_pCornerPt)
    {
		m_pViewPort->DrawRectPoint2D(m_pCornerPt->X(), m_pCornerPt->Y(), 10, TRUE, false);
    }

	// restore the draw color
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
