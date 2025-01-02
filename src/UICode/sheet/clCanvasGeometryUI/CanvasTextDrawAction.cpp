#include "StdAfx.h"
#include "CanvasTextDrawAction.h"

#include "boost/bind.hpp"
#include "Point2D.h"
#include "MathEx.h"
#include "baseConst.h"
#include "GlViewPort.h"
#include "CommandManager.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "clBaseDataResource.h"
#include "clUILibResource.h"
#include "DataBaseManager.h"
#include "ValueInputManager.h"
#include "ClWindowFinder.h"
#include "Polygon2D.h"
#include "Polygon2DList.h"

#include "ConfigItemLoader.h"
#include "SequenceConfigItem.h"
#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"
#include "PartEditParam.h"

#include "SnapPtDrawer.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartInstanceList.h"
#include "clPartLayoutResource.h"
#include "PartPlacementManager.h"
#include "LoopInstanceList.h"
#include "LoopInstance.h"
#include "PartInstance.h"
#include "clGeometryFeatureResource.h"
#include "TextParamActionBar.h"
#include "TextStruct.h"
#include "CanvasTextAddCommand.h"
#include "clGeometryFeatureUIResource.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasTextDrawAction::CanvasTextDrawAction(PartPlacementPtr pPartPlacement, TextParamActionBar* pTextParamActionBar, GlViewPortPtr pViewPort)
    : CanvasGeometryEditAction(pPartPlacement, pViewPort)
{
	m_pTextParamActionBar = pTextParamActionBar;
	m_iMsgID = IDS_MSG_SelectTextLeftBottomPt;

	m_pTextParamActionBar->AddObserver(this);
	m_pTextParamActionBar->Show_TextParamActionBar(false, CRect()); // 显示action bar。

	// 从文字设置生成文字的多边形轮廓。
	GenerateTextContours();
}

CanvasTextDrawAction::~CanvasTextDrawAction(void)
{
	m_pTextParamActionBar->RemoveObserver(GetID());
}

BOOL CanvasTextDrawAction::LButtonDown(double& dPtX, double& dPtY)
{
	// 考虑到特征点捕捉对光标位置的校正，这里需要更新一下移动向量。
	m_transferVect.Components(dPtX, dPtY);

	// 检查能不能创建文字对象。
	if (m_emErrorCode != Succeed)
	{
		CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CannotCreateText);
		MessageBox(m_pViewPort->GetWnd(), strMessage, strProductName, MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	// 准备数据。
	const TextParam* pTmpTextParam = m_pTextParamActionBar->GetTextParam();
	TextParamPtr pNewTextParam(new TextParam(*pTmpTextParam));
	Polygon2DListPtr pNewTextContours(new Polygon2DList);
	for (unsigned int i = 0; i < m_pTextContours->size(); i++)
		pNewTextContours->push_back(m_pTextContours->operator[](i)->Clone());

	// 执行命令。
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
	ICommandPtr pCommand(new CanvasTextAddCommand(GetEditorData(), pPartCadData, pPartCamData, pNewTextParam, m_transferVect, pNewTextContours));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	// 做一些更新。
	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

	return TRUE;
}

BOOL CanvasTextDrawAction::MovePoint(double& dPtX, double& dPtY)
{
	m_transferVect.Components(dPtX, dPtY);
	return TRUE;
}

BOOL CanvasTextDrawAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CanvasGeometryEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasTextDrawAction::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return TRUE;
}

BOOL CanvasTextDrawAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// 老的设置
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// 绘制文字。
	if (m_emErrorCode == Succeed)
	{
		double* pX = nullptr, *pY = nullptr;
		{
			unsigned int iCount = 0;
			for (unsigned int i = 0; i < m_pTextContours->size(); i++)
			{
				const Polygon2D* pTextContour = m_pTextContours->operator[](i).get();
				if (pTextContour->size() > iCount)
					iCount = pTextContour->size();
			}
			pX = new double[iCount];
			pY = new double[iCount];
		}

		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
		for (unsigned int i = 0; i < m_pTextContours->size(); i++)
		{
			const Polygon2D* pTextContour = m_pTextContours->operator[](i).get();
			for (unsigned int j = 0; j < pTextContour->size(); j++)
			{
				Point2D pt = pTextContour->operator[](j);
				pt += m_transferVect;
				pX[j] = pt.X();
				pY[j] = pt.Y();
			}
			m_pViewPort->DrawPolygon2D(pX, pY, pTextContour->size());
		}

		delete[] pX;
		delete[] pY;
	}

	// 恢复设置
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);

	// 绘制光标。
	CursorDrawer::DrawCrossLineCursor(m_pViewPort);

	return TRUE;
}

void CanvasTextDrawAction::UpdateForUndoRedo()
{
}

void CanvasTextDrawAction::Update(int iEventID, const CComVariant& varEventContent)
{
	if (iEventID == ID_EVENT_TextParamChanged) { // 从文字设置生成文字的多边形轮廓。
		GenerateTextContours();
	}
}

void CanvasTextDrawAction::GenerateTextContours()
{
	const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();
	const TextParam* pTextParam = m_pTextParamActionBar->GetTextParam();
	const CString& strTextContent = pTextParam->GetTextContent();
	const CString& strFontFileName = pTextParam->GetFontFileName();
	double dTextHeight = pTextParam->GetTextHeight();
	double dTextWidth = pTextParam->GetTextWidth();
	double dTextTiltAngle = pTextParam->GetTextTiltAngle();
	m_pTextContours.reset(new Polygon2DList);
	m_emErrorCode = Text2ContourHelper::GenerateTextContours(strTextContent, strFontFileName, dTextHeight, dTextWidth, dTextTiltAngle, strDefaultFontFileName, m_pTextContours);
}

END_CUTLEADER_NAMESPACE()
