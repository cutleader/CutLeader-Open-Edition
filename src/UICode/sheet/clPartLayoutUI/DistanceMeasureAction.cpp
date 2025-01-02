#include "StdAfx.h"
#include "DistanceMeasureAction.h"

#include "baseConst.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "LineArc2DList.h"
#include "BasicGeometryDrawer.h"

#include "IMaterialSize.h"
#include "MaterialProcessor.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "IPatternLoop.h"
#include "PatternLoopCache.h"
#include "LoopDrawer.h"
#include "LoopCutFeatureDrawer.h"
#include "clPartLayoutResource.h"
#include "LoopInstanceList.h"
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "PartPlacementManager.h"
#include "LoopInstanceManager.h"
#include "PatternList.h"

BEGIN_CUTLEADER_NAMESPACE()

DistanceMeasureAction::DistanceMeasureAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_iState = NONE;
	m_iMsgID = IDS_MEASURE_DIS_FST_LOOP;

	// 得到所有外围“回路实例”。
	m_pAllLoopInstances = LoopInstanceManager::GetLoopInstanceList(m_pPartInstanceList);

	// 缓存材料边框的基本图形。
	m_pSheetBorderGeoms = MaterialProcessor::GetBorderLineArcs(m_pMatSize.get());

	m_pPatternsInLoopInstance.reset(new PatternList());
}

DistanceMeasureAction::~DistanceMeasureAction(void)
{
}

BOOL DistanceMeasureAction::LButtonDown(double& dPtX, double& dPtY)
{
	CString strProductName, strMsg;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	Point2D pt(dPtX, dPtY);
	if (m_iState == NONE || m_iState == SECOND_PT_SELECTED)
	{
		m_iState = FIRST_LOOP_SELECTED;
		m_iMsgID = IDS_MEASURE_DIS_FST_PT;
	}
	else if (m_iState == FIRST_LOOP_SELECTED)
	{
		m_iState = FIRST_PT_SELECTED;
		m_iMsgID = IDS_MEASURE_DIS_SND_LOOP;
	}
	else if (m_iState == FIRST_PT_SELECTED)
	{
		m_iState = SECOND_LOOP_SELECTED;
		m_iMsgID = IDS_MEASURE_DIS_SND_PT;
	}
	else if (m_iState == SECOND_LOOP_SELECTED)
	{
		// 显示测量结果。
		CString strDis1, strDis2, strDis3;
		strDis1.Format(_T("%.3f"), m_fstPt.DistanceTo(m_sndPt));
		strDis2.Format(_T("%.3f"), m_fstPt.XDistanceTo(m_sndPt));
		strDis3.Format(_T("%.3f"), m_fstPt.YDistanceTo(m_sndPt));
		CString strMsg, strMsg1, strMsg2, strMsg3;
		strMsg1 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MEASURE_DIS_RESULT_1);
		strMsg1.Replace(_T("WILLBEREPLACED1"), strDis1);
		strMsg2 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MEASURE_DIS_RESULT_2);
		strMsg2.Replace(_T("WILLBEREPLACED2"), strDis2);
		strMsg3 = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MEASURE_DIS_RESULT_3);
		strMsg3.Replace(_T("WILLBEREPLACED3"), strDis3);
		strMsg = strMsg1 + _T("\n") + strMsg2 + _T("\n") + strMsg3;
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONINFORMATION);

		m_iState = SECOND_PT_SELECTED;
		m_iMsgID = IDS_MEASURE_DIS_FST_PT;
	}

	return TRUE;
}

BOOL DistanceMeasureAction::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	if (m_iState == NONE || m_iState == SECOND_PT_SELECTED)
	{
		// 计算出最近的回路。
		m_pCloseLoop = CalcClosestLoop(pt);
	}
	else if (m_iState == FIRST_LOOP_SELECTED)
	{
		// 第一个测量点。
		m_fstPt = m_pCloseLoop->GetClosestPt(pt);
	}
	else if (m_iState == FIRST_PT_SELECTED)
	{
		// 计算出最近的回路。
		m_pCloseLoop = CalcClosestLoop(pt);
	}
	else if (m_iState == SECOND_LOOP_SELECTED)
	{
		// 第二个测量点。
		m_sndPt = m_pCloseLoop->GetClosestPt(pt);
	}

	return TRUE;
}

BOOL DistanceMeasureAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return __super::OnKeyDown(nChar, nRepCnt, nFlags);
}

SnapInputPtr DistanceMeasureAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_iState == FIRST_LOOP_SELECTED)
	{
		if (m_pCloseLoopInstance)
		{
			Matrix2D transformMatrix = m_pCloseLoopInstance->GetTransformMatrix();
			pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapClosestPtTo2DLineArc, m_pPatternsInLoopInstance, transformMatrix));
		}
	}
	else if (m_iState == SECOND_LOOP_SELECTED)
	{
		if (m_pCloseLoopInstance)
		{
			Matrix2D transformMatrix = m_pCloseLoopInstance->GetTransformMatrix();
			pSnapInput.reset(new SnapInput(SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapClosestPtTo2DLineArc, m_pPatternsInLoopInstance, transformMatrix));
		}
	}

	return pSnapInput;
}

BOOL DistanceMeasureAction::DrawBeforeFinish()
{
	// prepare some data.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pDisplayParam = pCLOptionInfo->GetDisplayParam();
	BasicGeometryDrawer geomBaseDrawer(m_pViewPort);
	LoopCutFeatureDrawer loopFeatureDrawer(m_pViewPort);

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	if (m_iState == NONE || m_iState == SECOND_PT_SELECTED)
	{
		// 绘制最近的回路。
		if (m_pCloseLoop)
		{
			m_pViewPort->SetDrawColor(*pDisplayParam->GetActiveObjColor());
			m_pViewPort->SetLineWidth(2);
			geomBaseDrawer.Draw2DLineArcs(m_pCloseLoop.get());
		}
	}
	else if (m_iState == FIRST_LOOP_SELECTED)
	{
		loopFeatureDrawer.DrawPosRing(m_fstPt);
	}
	else if (m_iState == FIRST_PT_SELECTED)
	{
		// 绘制第一个测量点。
		loopFeatureDrawer.DrawFeatureLoc(m_fstPt, RGB(255,0,0));

		// 绘制最近的回路。
		if (m_pCloseLoop)
		{
			m_pViewPort->SetDrawColor(*pDisplayParam->GetActiveObjColor());
			m_pViewPort->SetLineWidth(2);
			geomBaseDrawer.Draw2DLineArcs(m_pCloseLoop.get());
		}
	}
	else if (m_iState == SECOND_LOOP_SELECTED)
	{
		// 绘制第一个测量点。
		loopFeatureDrawer.DrawFeatureLoc(m_fstPt, RGB(255,0,0));

		// 绘制第二个测量点。
		loopFeatureDrawer.DrawPosRing(m_sndPt);
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

LineArc2DListPtr DistanceMeasureAction::CalcClosestLoop(const Point2D& pt)
{
	LineArc2DListPtr pGeom2Ds;

	// 到板材边框的最小距离。
	double dBorderDis = m_pSheetBorderGeoms->GetClosestDis(pt);

	// 到所有“回路实例”的最短距离。
	double dLoopInstDis = INFINITE_VAL;
	LoopInstancePtr pLoopInstance = LoopInstanceManager::GetClosestLoopInst(m_pAllLoopInstances, pt);
	if (pLoopInstance)
		dLoopInstDis = pLoopInstance->DistanceTo(pt);

	if (dBorderDis > dLoopInstDis) // “回路实例”离得较近。
	{
		m_pCloseLoopInstance = pLoopInstance;
		m_pPatternsInLoopInstance = m_pCloseLoopInstance->GetCacheData()->GetPatternLoop()->GetPatternList();

		IPatternLoopPtr pPatternLoop = pLoopInstance->GetCacheData()->GetPatternLoop();
		pGeom2Ds = pPatternLoop->GetCacheData()->GetLoopBaseGeom()->Clone();
		pGeom2Ds->Transform(pLoopInstance->GetTransformMatrix());
	}
	else // 板材外框离得较近。
	{
		m_pCloseLoopInstance.reset();
		m_pPatternsInLoopInstance->clear();

		pGeom2Ds = m_pSheetBorderGeoms;
	}

	return pGeom2Ds;
}

END_CUTLEADER_NAMESPACE()
