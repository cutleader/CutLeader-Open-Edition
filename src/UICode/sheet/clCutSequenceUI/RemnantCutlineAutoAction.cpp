#include "StdAfx.h"
#include "RemnantCutlineAutoAction.h"

#include <map>
#include "DBConnect.h"
#include "DataBaseManager.h"
#include "baseConst.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"
#include "Point2DList.h"
#include "Polygon2D.h"
#include "Polygon2DList.h"
#include "Line2D.h"
#include "Line2DList.h"
#include "LineArc2DList.h"
#include "ValueInputManager.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"

#include "IMaterialSize.h"
#include "PolygonMaterial.h"
#include "MaterialLibWriter.h"
#include "SequenceConfigItem.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

#include "PartPlacementList.h"
#include "PartPlacementManager.h"
#include "PartPlacementEditAction.h"
#include "PartLayoutData.h"
#include "RemnantLineSeq.h"
#include "CutSequenceList.h"
#include "CutSequenceSelectAction.h"
#include "CutNodeManager.h"
#include "clCutSequenceResource.h"
#include "ConfigData.h"
#include "CutSequenceManager.h"
#include "CutSequenceDrawer.h"
#include "SequenceData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

RemnantCutlineAutoAction::RemnantCutlineAutoAction(PartLayoutDataPtr pPartLayoutData, SequenceDataPtr pSequenceData, ConfigDataPtr pConfigData,
	GlViewPortPtr pViewPort, GenerateRemCutLineActionBar* pGenerateRemCutLineActionBar)
	: SequenceEditActionBase(pPartLayoutData, pSequenceData, pConfigData, pViewPort)
{
	m_iMsgID = IDS_SEQ_AUTO_REM_CUTLINE_ACTION;
	m_pRemCutLines.reset(new Line2DList);

	// 初始化对话框
	{
		m_pGenerateRemCutLineActionBar = pGenerateRemCutLineActionBar;
		m_pGenerateRemCutLineActionBar->AddObserver(this);

		// 初始的余料方位
		RECT_SIDE iRemnantPosition;
		Rect2D sheetRect = m_pConfigData->GetMatSize()->GetOuterRect();
		Point2D sheetCenterPt = sheetRect.GetCenterPt();
		Rect2D partsRect = m_pPartLayoutData->GetPartPlacements()->GetRectBox();
		Point2D partsCenterPt = partsRect.GetCenterPt();
		if (fabs(sheetCenterPt.Y() - partsCenterPt.Y()) > fabs(sheetCenterPt.X() - partsCenterPt.X())) { // 从板材上侧或下侧取余料
			if (sheetCenterPt.Y() > partsCenterPt.Y()) { // 从上侧取余料
				iRemnantPosition = RECT_TOP;
			}
			else { // 从下侧取余料
				iRemnantPosition = RECT_BOTTOM;
			}
		}
		else { // 从板材左侧或右侧取余料
			if (sheetCenterPt.X() < partsCenterPt.X()) { // 从左侧取余料
				iRemnantPosition = RECT_LEFT;
			}
			else { // 从右侧取余料
				iRemnantPosition = RECT_RIGHT;
			}
		}

		// 初始的光束宽度
		double dBeamWidth;
		if (iRemnantPosition == RECT_TOP || iRemnantPosition == RECT_BOTTOM) {
			dBeamWidth = sheetRect.GetWidth() / 2.0;
		}
		else if (iRemnantPosition == RECT_LEFT || iRemnantPosition == RECT_RIGHT) {
			dBeamWidth = sheetRect.GetHeight() / 2.0;
		}

		m_pGenerateRemCutLineActionBar->Init(iRemnantPosition, dBeamWidth);
		m_pGenerateRemCutLineActionBar->Show_GenerateRemCutLineActionBar();
	}
}

RemnantCutlineAutoAction::~RemnantCutlineAutoAction(void)
{
	m_pGenerateRemCutLineActionBar->RemoveObserver(GetID());
}

BOOL RemnantCutlineAutoAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SequenceEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
	{
		m_pNextAction.reset(new CutSequenceSelectAction(m_pPartLayoutData, m_pSequenceData, m_pConfigData, m_pViewPort));
	}

	return TRUE;
}

BOOL RemnantCutlineAutoAction::DrawBeforeFinish()
{
	// prepare some data.
	CLOptionInfoPtr pCLOptionInfo = CLOptionInfo::GetInstance();
	DisplayParamPtr pDisplayParam = pCLOptionInfo->GetDisplayParam();

	// 老的设置。
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制余料切割线
    m_pViewPort->SetDrawColor(*pDisplayParam->GetSequenceColor());
    m_pViewPort->SetLineWidth(1);
	for (unsigned int i = 0; i < m_pRemCutLines->size(); i++)
	{
		Line2DPtr pLine2D = m_pRemCutLines->at(i);
		const Point2D& startPt = pLine2D->GetStartPoint();
		const Point2D& endPt = pLine2D->GetEndPoint();
		m_pViewPort->DrawLinePoint2D(startPt.X(), startPt.Y(), endPt.X(), endPt.Y());
	}

	// 恢复设置。
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// draw cursor.
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void RemnantCutlineAutoAction::Update(int iEventID, const CComVariant& varEventContent)
{
	CString strMsg, strProductName;
	strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);

	// 检查
	if (m_pPartLayoutData->GetPartPlacements()->size() == 0)
	{
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_NO_PART_EXIST);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
		return;
	}

	if (m_pGenerateRemCutLineActionBar->GetCmdType() == GenerateRemCutLineActionBar_previewCmd || // 预览
		m_pGenerateRemCutLineActionBar->GetCmdType() == GenerateRemCutLineActionBar_generateCmd) // 生成余料切割线
	{
		// 用光照的方式得到余料切割线。
		RECT_SIDE iRayDir = m_pGenerateRemCutLineActionBar->GetRayDir();
		double dOffsetDistance = m_pGenerateRemCutLineActionBar->GetOffsetDistance();
		int iEvalFactor = BeamWidth_2_BeamNum(iRayDir, m_pGenerateRemCutLineActionBar->GetStepWidth());
		Line2DListPtr pRemCutLines = PartPlacementManager::CalcRemCutLine(m_pPartLayoutData->GetPartPlacements(), m_pConfigData->GetMatSize(), iRayDir, iEvalFactor, dOffsetDistance);

		// 提取切割线
		m_pRemCutLines->clear();
		if (pRemCutLines->size() > 0)
		{
			m_pRemCutLines->insert(m_pRemCutLines->end(), pRemCutLines->begin(), pRemCutLines->end());
			CWnd::FromHandle(m_pViewPort->GetWnd())->Invalidate();

			// 生成余料切割线
			if (m_pGenerateRemCutLineActionBar->GetCmdType() == GenerateRemCutLineActionBar_generateCmd)
			{
				// 是否要调整顺序切割线的切割反向。切割线的开始点要距离板材的起切点近一些。
				const SequenceConfigItem* pSeqConfigItem = m_pConfigData->GetSeqConfigItem();
				Point2D startCutPt_of_wholeSheet = m_pConfigData->GetMatSize()->GetOuterRect().GetCornerPt(pSeqConfigItem->GetStartCorner());
				if (startCutPt_of_wholeSheet.DistanceTo(m_pRemCutLines->front()->GetStartPoint()) > startCutPt_of_wholeSheet.DistanceTo(m_pRemCutLines->back()->GetEndPoint()))
				{
					m_pRemCutLines->Reverse();
				}

				RemnantLineSeqPtr pRemnantLineSeq(new RemnantLineSeq);
				Line2DListPtr pLines = pRemnantLineSeq->GetCutLines();
				pLines->insert(pLines->end(), m_pRemCutLines->begin(), m_pRemCutLines->end());
				CutSequenceManager::InsertCutSeq(m_pSequenceData->GetCutSequences(), -1, pRemnantLineSeq, m_pConfigData->GetMatSize(), m_pConfigData->GetSeqConfigItem());
				m_pSequenceData->GetCutSequences()->SetModified(TRUE);
				RefreshCutNodes(); // 更新“切割节点”缓存。

				strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTO_REM_CUTLINE_OK);
				MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_AUTO_REM_CUTLINE_ERROR);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
		}
	}
	else if (m_pGenerateRemCutLineActionBar->GetCmdType() == GenerateRemCutLineActionBar_saveMatCmd) // 保存余料
	{
		// 用光照的方式得到有效区域。
		const IMaterialSize* pMaterialSize = m_pConfigData->GetMatSize();
		RECT_SIDE iRayDir = m_pGenerateRemCutLineActionBar->GetRayDir();
		int iEvalFactor = BeamWidth_2_BeamNum(iRayDir, m_pGenerateRemCutLineActionBar->GetStepWidth());
		Polygon2DPtr pUsefulRegion = PartPlacementManager::CalcUsefulRegion(m_pPartLayoutData->GetPartPlacements(), pMaterialSize, iRayDir, iEvalFactor);
		if (!pUsefulRegion)
		{
			strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REM_MAT_ERROR);
			MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
			return;
		}

		// 保存余料
		DBConnectPtr pMatLibDBCon = DataBaseManager::GetInstance()->GetMatLibDBConnect();
		LineArc2DListPtr pOuterLoop(new LineArc2DList);
		Line2DListPtr pLine2DList = pUsefulRegion->GetLineList();
		for (unsigned int i = 0; i < pLine2DList->size(); i++)
		{
			pOuterLoop->push_back(pLine2DList->at(i));
		}
		Rect2D matRect = pUsefulRegion->GetBoundaryRect();
		CString strMatName;
		strMatName.Format(_T("%.1fx%.1f"), matRect.GetWidth(), matRect.GetHeight());
		PolygonMaterialPtr pPolyMat(new PolygonMaterial(pMaterialSize->GetParentID(), strMatName, TRUE, matRect.GetWidth(), matRect.GetHeight(), 1, 1));
		pPolyMat->SetOuterLoop(pOuterLoop);
		MaterialLibWriter::SaveMatSize(pMatLibDBCon, pPolyMat, TRUE);

		// 提示
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_SEQ_REM_MAT_OK);
		strMsg.Replace(_T("WILLBEREPLACED1"), strMatName);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);
	}
}

int RemnantCutlineAutoAction::BeamWidth_2_BeamNum(RECT_SIDE iRayDir, double dBeamWidth)
{
	int iBeamNum;

	Rect2D sheetRect = m_pConfigData->GetMatSize()->GetOuterRect();
	if (iRayDir == RECT_TOP || iRayDir == RECT_BOTTOM)
	{
		iBeamNum = (int)(sheetRect.GetWidth() / dBeamWidth);
	}
	else if (iRayDir == RECT_LEFT || iRayDir == RECT_RIGHT)
	{
		iBeamNum = (int)(sheetRect.GetHeight() / dBeamWidth);
	}

	if (iBeamNum == 0)
	{
		iBeamNum = 1;
	}

	return iBeamNum;
}

END_CUTLEADER_NAMESPACE()
