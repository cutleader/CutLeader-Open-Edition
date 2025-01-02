#include "StdAfx.h"
#include "CanvasPatternGridAction.h"

#include "GlViewPort.h"
#include "CommandManager.h"
#include "CursorDrawer.h"
#include "clUILibResource.h"
#include "ClWindowFinder.h"
#include "LineArc2DList.h"

#include "DisplayParam.h"
#include "CLOptionInfo.h"

#include "PartCamData.h"
#include "Part.h"
#include "PartPlacement.h"
#include "clPartLayoutResource.h"
#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PatternDrawer.h"
#include "CanvasLoopCopyCommand.h"
#include "CanvasGeometryHelper.h"
#include "clCanvasGeometryUIResource.h"
#include "TextStructList.h"
#include "TextStruct.h"
#include "GeometryFeatureHelper.h"
#include "PartCadData.h"
#include "CreateGeometryGridActionBar.h"
#include "PatternLoopList.h"
#include "CanvasLoopGridCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasPatternGridAction::CanvasPatternGridAction(PartPlacementPtr pPartPlacement, PatternListPtr pSelectedPatterns, TextStructListPtr pSelectedTextStructs,
	SelectPatternOrLoopActionBar* pSelectPatternOrLoopActionBar, CreateGeometryGridActionBar* pCreateGeometryGridActionBar, GlViewPortPtr pViewPort)
    : CanvasPatternPreSelectAction(pPartPlacement, pSelectPatternOrLoopActionBar, pViewPort)
{
	m_pSelectedPatterns = pSelectedPatterns;
	m_loopsOfSelectedPatterns = GeometryFeatureHelper::FindPatternLoopsOfPatterns(m_pPartPlacement->GetPart()->GetCadData().get(), m_pSelectedPatterns.get());
	m_pSelectedTextStructs = pSelectedTextStructs;

	m_pCreateGeometryGridActionBar = pCreateGeometryGridActionBar;
	m_pCreateGeometryGridActionBar->AddObserver(this);

	if (m_pSelectedPatterns->size() == 0) {
		m_iMsgID = IDS_OUTPUT_SelectCanvasGeometry2Grid;
		m_pSelectPatternOrLoopActionBar->DisplayActionBar(OnlyEnableLoopSelect);
		m_pLineArc2Ds4Preview.reset(new LineArc2DList);
	}
	else {
		m_iMsgID = IDS_OUTPUT_GridLoop;
		m_pCreateGeometryGridActionBar->Show_CreateGeometryGridActionBar();
		GenerateLineArc2Ds4Preview();
	}
}

CanvasPatternGridAction::~CanvasPatternGridAction(void)
{
	m_pCreateGeometryGridActionBar->RemoveObserver(GetID());
}

BOOL CanvasPatternGridAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		__super::LButtonDown(dPtX, dPtY);
	}

	return TRUE;
}

BOOL CanvasPatternGridAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0)
	{
		__super::LButtonUp(dPtX, dPtY);
		if (m_pSelectedPatterns->size() > 0)
		{
			m_pSelectPatternOrLoopActionBar->HideActionBar();
			m_pCreateGeometryGridActionBar->Show_CreateGeometryGridActionBar();
			GenerateLineArc2Ds4Preview();
		}
	}

	return TRUE;
}

BOOL CanvasPatternGridAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_pSelectedPatterns->size() == 0) // 还没选中几何特征
	{
		__super::MovePoint(dPtX, dPtY);
	}

	return TRUE;
}

SnapInputPtr CanvasPatternGridAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	return pSnapInput;
}

BOOL CanvasPatternGridAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnKeyDown(nChar, nRepCnt, nFlags);

	return TRUE;
}

BOOL CanvasPatternGridAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	BasicGeometryDrawer basicGeometryDrawer(m_pViewPort);

	if (m_pSelectedPatterns->size() == 0)
	{
		CanvasPatternPreSelectAction::DrawBeforeFinish();
	}
	else
	{
		// draw the selected patterns.
		DrawSelectedPatterns(m_pSelectedPatterns.get(), m_loopsOfSelectedPatterns);

		// keep the old color.
		COLORREF dwOldColor;
		m_pViewPort->GetDrawColor(dwOldColor);

		// 绘制用于预览的二维直线圆弧。
		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
		basicGeometryDrawer.Draw2DLineArcs(m_pLineArc2Ds4Preview.get());

		// restore the draw color
		m_pViewPort->SetDrawColor(dwOldColor);

		// draw cursor.
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	}

	return TRUE;
}

void CanvasPatternGridAction::UpdateForUndoRedo()
{
}

void CanvasPatternGridAction::Update(int iEventID, const CComVariant& varEventContent)
{
	if (iEventID == ID_EVENT_GridParamChanged)
	{
		GenerateLineArc2Ds4Preview();
	}
	else if (iEventID == ID_EVENT_GridGeometry_OK)
	{
		const GeometryGridParam* pGeometryGridParam = m_pCreateGeometryGridActionBar->GetGeometryGridParam();

		// 要拷贝的文字。
		vector<LONGLONG> textStructs_willCopy;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
			textStructs_willCopy.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

		// 执行命令。
		PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
		PartCamDataPtr pPartCamData = m_pPartPlacement->GetCamData();
		map<LONGLONG, PatternListPtr> patternsInLoop; // 这个结构记录了要删除的几何特征分别在哪个几何轮廓中。
		PatternLoopListPtr pPatternLoops_willCopy = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get(), patternsInLoop); // 找出所有涉及到的几何轮廓。
		ICommandPtr pCommand(new CanvasLoopGridCommand(GetEditorData(), pPartCadData, pPartCamData, pPatternLoops_willCopy, textStructs_willCopy, pGeometryGridParam));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 做一些更新。
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

		// 回到选择action。
		PostMessage2EnterCanvasLoopSelectAction();
	}
	else if (iEventID == ID_EVENT_GridGeometry_Cancel)
	{
		// 回到选择action。
		PostMessage2EnterCanvasLoopSelectAction();
	}
}

void CanvasPatternGridAction::GenerateLineArc2Ds4Preview()
{
	PartCadDataPtr pPartCadData = m_pPartPlacement->GetPart()->GetCadData();
	map<LONGLONG, PatternListPtr> patternsInLoop; // 这个结构记录了要删除的几何特征分别在哪个几何轮廓中。
	PatternLoopListPtr pAffectedPatternLoops = GeometryFeatureHelper::FindAffectedPatternLoops(pPartCadData.get(), m_pSelectedPatterns.get(), patternsInLoop); // 找出所有涉及到的几何轮廓。
	LineArc2DListPtr pLineArc2DsOfSelectedLoops = pAffectedPatternLoops->Calculate2DLineArcs();
	const GeometryGridParam* pGeometryGridParam = m_pCreateGeometryGridActionBar->GetGeometryGridParam();
	m_pLineArc2Ds4Preview = CanvasGeometryHelper::CreateGeometryGrid(pLineArc2DsOfSelectedLoops.get(), pGeometryGridParam);
	CWnd::FromHandle(m_pViewPort->GetWnd())->Invalidate();
}

END_CUTLEADER_NAMESPACE()
