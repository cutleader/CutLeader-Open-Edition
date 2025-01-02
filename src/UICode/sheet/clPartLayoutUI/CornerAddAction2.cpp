#include "StdAfx.h"
#include "CornerAddAction2.h"

#include "MathEx.h"
#include "CommandManager.h"
#include "Point2DList.h"
#include "LineArc2DList.h"
#include "GlViewPort.h"
#include "DummyAction.h"
#include "ValueInputManager.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"

#include "IMaterialSize.h"

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
#include "LoopInstanceCache.h"
#include "LoopInstance.h"
#include "LoopInstanceList.h"
#include "PartInstanceList.h"
#include "LoopInstanceManager.h"
#include "CornersAddCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CornerAddAction2::CornerAddAction2(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances, PartGroupListPtr pPartGroups,
	GlViewPortPtr pViewPort, SpreadSamePartActionBar* pSpreadSamePartActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort)
{
	m_pSpreadSamePartActionBar = pSpreadSamePartActionBar;
	m_state = CORNER_STATE_NONE;
	m_iMsgID = IDS_CUTFEATURE_SELECT_CORNER_PT;
	m_pAllLoopInstanceList = LoopInstanceManager::GetLoopInstanceList(pPartInstances);

	if (m_pMatSize->GetType() != MAT_SIZE_Canvas)
		pSpreadSamePartActionBar->Show_SpreadSamePartActionBar();
}

CornerAddAction2::~CornerAddAction2(void)
{
}

BOOL CornerAddAction2::LButtonDown(double& dPtX, double& dPtY)
{
	CString str, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	if (m_pCurCornerPt)
	{
		IPatternLoopPtr pPatternLoop = m_pCurLoopInst->GetCacheData()->GetPatternLoop();
		LoopCornerDataPtr pLoopCorner = m_pCurLoopInst->GetCacheData()->GetLoopCorner();

		// 把角点变换到零件坐标系下。
		Point2D tmpPt = *m_pCurCornerPt;
		Matrix2D mat = m_pCurLoopInst->GetTransformMatrix();
		mat.Inverse();
		tmpPt *= mat;

		// 检查指定位置是不是有了角特征。
		if (pLoopCorner->GetCornerAtLoc(tmpPt))
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_EXIST);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	
		// 不能在光滑的地方创建角特征。
		if (!pPatternLoop->IsOpenPath())
		{
			double dCornerAngle = LoopProcessor::GetAngAtLoopCorner(pPatternLoop.get(), tmpPt);
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
		double dVal = CornerPropertyBar::GetInstance()->GetCornerValue();
		if (cornerType != Corner_None && dVal != 0)
		{
			// 创建角特征，然后检查它。
			const PolygonPatternLoop* pPolygonPatternLoop = dynamic_cast<const PolygonPatternLoop*>(pPatternLoop.get());
			Point2DPtr pTmpPt(new Point2D(tmpPt));
			ICornerPtr pCorner = CornerManager::BuildCorner(pLoopCorner->GetID(), pPolygonPatternLoop, *pTmpPt, cornerType, dVal);
			if (!CornerManager::CheckCornerValue(pPolygonPatternLoop, pCorner.get()))
			{
				str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_WRONG_VAL);
				MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
				return FALSE;
			}

			if (m_pSpreadSamePartActionBar->IsSpread())
			{
				// 需要新增的角特征数据
				vector<pair<LoopCornerDataPtr, ICornerPtr>> cornerDataList;
				for (unsigned int i = 0; i < m_pAllLoopInstanceList->size(); i++)
				{
					LoopInstancePtr pLoopInst_tmp =  m_pAllLoopInstanceList->at(i);
					if (pLoopInst_tmp->GetCacheData()->GetPatternLoop()->GetID() == pPatternLoop->GetID())
					{
						LoopCornerDataPtr pLoopCorner_tmp = pLoopInst_tmp->GetCacheData()->GetLoopCorner();
						const PolygonPatternLoop* pPolygonPatternLoop_tmp = dynamic_cast<const PolygonPatternLoop*>(pLoopCorner_tmp->GetPatternLoop().get());
						Point2DPtr pTmpPt(new Point2D(tmpPt));
						if (!pLoopCorner_tmp->GetCornerAtLoc(tmpPt))
						{
							ICornerPtr pCorner_new = CornerManager::BuildCorner(pLoopCorner_tmp->GetID(), pPolygonPatternLoop_tmp, *pTmpPt, cornerType, dVal);
							cornerDataList.push_back(make_pair(pLoopCorner_tmp, pCorner_new));
						}
					}
				}

				// 执行命令
				EditorData editorData = GetEditorData();
				ICommandPtr pCommand(new CornersAddCommand(editorData, cornerDataList));
				pCommand->Do();
				CommandManager::GetInstance()->AddCommand(pCommand);
			}
			else
			{
				// 添加角特征。
				EditorData editorData = GetEditorData();
				ICommandPtr pCommand(new CornerAddCommand(editorData, pLoopCorner, pCorner));
				pCommand->Do();
				CommandManager::GetInstance()->AddCommand(pCommand);
			}
		}
		else
		{
			str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_CORNER_WRONG_VAL);
			MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
		}
	}
	else
	{
		str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE_SELECT_CORNER_PT);
		MessageBox(m_pViewPort->GetWnd(), str, strProductName, MB_OK | MB_ICONWARNING);
	}

	return TRUE;
}

BOOL CornerAddAction2::MovePoint(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);

	// 得到当前的回路实例。
	m_pCurLoopInst = LoopInstanceManager::GetClosestLoopInst(m_pAllLoopInstanceList, pt);
	if (!m_pCurLoopInst)
		return FALSE;

	// 更新当前的角点。
	IPatternLoopPtr pPatternLoop = m_pCurLoopInst->GetCacheData()->GetPatternLoop();
	if (pPatternLoop->GetPatternLoopType() == PatternLoop_Polygon)
	{
		PolygonPatternLoopPtr pPolygonPatternLoop = boost::dynamic_pointer_cast<PolygonPatternLoop>(pPatternLoop);

		// 这个回路实例上距离“pt”最近的控制点。
		Point2DListPtr pPtList = pPolygonPatternLoop->GetCtrlPtList(TRUE);
		pPtList->Transform(m_pCurLoopInst->GetTransformMatrix());
		m_pCurCornerPt.reset(new Point2D(pPtList->GetPtClosestTo(pt)));
	}
	else
	{
		m_pCurLoopInst.reset();
		m_pCurCornerPt.reset();
	}

	return TRUE;
}

BOOL CornerAddAction2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		CornerPropertyBar::GetInstance()->ShowPane(FALSE, FALSE, TRUE);
	}

	return TRUE;
}

BOOL CornerAddAction2::DrawBeforeFinish()
{
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);

	// 绘制角点。
	m_pViewPort->SetDrawColor(RGB(255, 0, 0));
	if (m_pCurCornerPt)
    {
		m_pViewPort->DrawRectPoint2D(m_pCurCornerPt->X(), m_pCurCornerPt->Y(), 10, TRUE, false);
    }

	// restore the draw color.
	m_pViewPort->SetDrawColor(dwOldColor);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
