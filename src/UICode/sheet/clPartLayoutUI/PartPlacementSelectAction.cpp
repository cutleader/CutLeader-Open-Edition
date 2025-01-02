#include "StdAfx.h"
#include "PartPlacementSelectAction.h"

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
#include "LogMgr.h"

#include "ConfigItemLoader.h"
#include "SequenceConfigItem.h"
#include "DisplayParam.h"
#include "SheetEditParam.h"
#include "CLOptionInfo.h"
#include "SnapPtParam.h"

#include "SnapPtDrawer.h"
#include "LoopToolData.h"
#include "LoopCutFeature.h"
#include "PartCamData.h"
#include "PartItem.h"
#include "Part.h"
#include "PartPlacementList.h"
#include "PartPlacement.h"
#include "PartInstanceList.h"
#include "PartPlacementDrawer.h"
#include "clPartLayoutResource.h"
#include "PartPlacementPropertyBar.h"
#include "PartPlacementDeleteCommand.h"
#include "PartPlacementCopyCommand.h"
#include "PartPlacementTransformCommand.h"
#include "PartPlacementManager.h"
#include "PartPlacementMoveAction.h"
#include "PartPlacementCopyAction.h"
#include "PartPlacementRotateAction.h"
#include "PartGroup.h"
#include "PartGroupList.h"
#include "PartLayoutHelper.h"
#include "LoopInstanceList.h"
#include "LoopInstance.h"
#include "PartInstance.h"
#include "clPartLayoutUIResource.h"
#include "PartPlacementTransformCommand_2.h"
#include "PartPlacementAddPanel.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PartPlacementSelectAction::PartPlacementSelectAction(IMaterialSizePtr pMaterialSize, PartPlacementListPtr pPartPlacements, PartInstanceListPtr pPartInstances,
	PartGroupListPtr pPartGroups, GlViewPortPtr pViewPort, PartDistanceDetectActionBar* pPartDistanceDetectActionBar, SelectPartOrLoopActionBar* pSelectPartOrLoopActionBar)
    : PartPlacementEditAction(pMaterialSize, pPartPlacements, pPartInstances, pPartGroups, pViewPort), PartPlacementMoveBase(m_pViewPort, pPartDistanceDetectActionBar)
{
	m_pActivePartPlacements.reset(new PartPlacementList);
    m_pSelectedPartPlacements.reset(new PartPlacementList);
	m_state = PARTPMT_SELECT_NONE;
	m_iMsgID = IDS_OUTPUT_SELECTPRTPMT_EDIT;
	m_pBasePartPlacements.reset(new PartPlacementList);
	m_emQuickEditType = NoneQuickEdit;
    m_pSelectPartOrLoopActionBar = pSelectPartOrLoopActionBar;

	UpdateOuterRects_ofAllPartPlacements();
    m_pWrongToolSizeLoopInstances.reset(new WrongToolSizeLoopInstances);

	PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE); // 这一行不要忘记。
	m_pSelectPartOrLoopActionBar->Show_SelectPartOrLoopActionBar(true);
}

PartPlacementSelectAction::~PartPlacementSelectAction(void)
{
}

void PartPlacementSelectAction::UpdateOuterRects_ofAllPartPlacements()
{
    for (unsigned int i = 0; i < m_pPartPlacementList->size(); i++)
    {
        PartPlacementPtr pPartPlacement = m_pPartPlacementList->at(i);
        m_rects_of_partPmt[pPartPlacement->GetID()] = pPartPlacement->GetRectBox();
    }
}

BOOL PartPlacementSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	if (m_emQuickEditType == Ctrl_C_Ctrl_V) { // 执行快捷拷贝。
		ExecuteQuickCopy();
		return TRUE;
	}
	else if (m_emQuickEditType == Drag || m_emQuickEditType == Ctrl_Drag) {
		return TRUE;
	}

	// 判断是否进入快捷编辑模式（ctrl+拖拽拷贝几何特征或者拖拽移动几何特征）。
	if (m_pSelectedPartPlacements->size() > 0)
	{
		double dCaptureRange = CLOptionInfo::GetInstance()->GetSnapPtParam()->GetSnapRange(); // 捕捉距离。
		m_pViewPort->DToW(dCaptureRange);
		PartPlacementPtr pClosestPartPlacement = m_pSelectedPartPlacements->GetPartPlacementClosestTo(Point2D(dPtX, dPtY));
		if (pClosestPartPlacement->DistanceTo(Point2D(dPtX, dPtY)) < dCaptureRange)
		{
			m_quickEditBasePt = Point2D(dPtX, dPtY);
			if (ValueInputManager::GetInstance()->IsInputCtrl())
			{
				m_emQuickEditType = Ctrl_Drag;

				// 为移动准备数据。
				PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
				for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
					pMovePartPlacements->push_back(PartPlacementManager::ClonePartPmt(m_pSelectedPartPlacements->at(i).get()));
				UpdateBeforeMov(pMovePartPlacements, Point2D(dPtX, dPtY), m_pMatSize, m_pPartInstanceList);
			}
			else
			{
				m_emQuickEditType = Drag;

				// 为移动准备数据。
				PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
				for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
					pMovePartPlacements->push_back(PartPlacementManager::ClonePartPmt(m_pSelectedPartPlacements->at(i).get()));
				PartInstanceListPtr pFixPartInstances(new PartInstanceList);
				for (unsigned int i = 0; i < m_pPartPlacementList->size(); i++)
				{
					const PartPlacement* pPartPlacement = m_pPartPlacementList->operator [](i).get();
					PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement);
					if (!m_pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement->GetID()))
						pFixPartInstances->insert(pFixPartInstances->end(), pPartInstances->begin(), pPartInstances->end());
				}
				UpdateBeforeMov(pMovePartPlacements, Point2D(dPtX, dPtY), m_pMatSize, pFixPartInstances);
			}

			PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pSelectPartOrLoopActionBar->HideActionBar();
			GetPartDistanceDetectActionBar()->Show_PartDistanceDetectActionBar();
			return TRUE;
		}
	}

	if (m_state == PARTPMT_SELECT_NONE || m_state == PARTPMT_SELECT_SECOND_POINT)
	{
		m_state = PARTPMT_SELECT_FIRST_POINT;
		m_iMsgID = IDS_OUTPUT_SELECTPRTPMT_NEXTPT;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL PartPlacementSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	if (m_emQuickEditType == Ctrl_C_Ctrl_V) {
		return TRUE;
	} else if (m_emQuickEditType == Drag) { // 执行快捷移动。
		ExecuteQuickMove();
		return TRUE;
	} else if (m_emQuickEditType == Ctrl_Drag) { // 执行快捷拷贝。
		ExecuteQuickCopy();
		return TRUE;
	}

	if (m_state == PARTPMT_SELECT_FIRST_POINT)
	{
		m_state = PARTPMT_SELECT_SECOND_POINT;
		m_iMsgID = IDS_OUTPUT_SELECTPRTPMT_EDIT;

		// 要是没有按住ctrl键，就需要清除现有选择集。
		if (!ValueInputManager::GetInstance()->IsInputCtrl()) {
			m_pSelectedPartPlacements->clear();
        }

        if (m_pActivePartPlacements->size() > 0)
        {
            m_pSelectedPartPlacements->insert(m_pSelectedPartPlacements->end(), m_pActivePartPlacements->begin(), m_pActivePartPlacements->end());

            // 看看要不要选中相同零件。
            if (m_pSelectPartOrLoopActionBar->GetIsAutoSelectSamePartsOrLoops()) {
                for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++) {
                    const PartPlacement* pPartPlacement = m_pSelectedPartPlacements->operator [](i).get();
                    PartPlacementListPtr pPartPlacements_samePart = m_pPartPlacementList->GetPartPlacementByPart(pPartPlacement->GetPart()->GetID());
                    for (unsigned int j = 0; j < pPartPlacements_samePart->size(); j++) {
                        PartPlacementPtr pPartPlacement_samePart = pPartPlacements_samePart->operator [](j);
						if (!m_pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement_samePart->GetID())) {
							m_pSelectedPartPlacements->push_back(pPartPlacement_samePart);
						}
                    }
                }
            }

            PartPlacementPropertyBar::GetInstance()->DisplayPartPmt(m_pSelectedPartPlacements, m_pPartInstanceList, m_pViewPort);
        }
		else {
			PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
		}
	}

	return TRUE;
}

BOOL PartPlacementSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	if (m_emQuickEditType == Ctrl_C_Ctrl_V || m_emQuickEditType == Drag || m_emQuickEditType == Ctrl_Drag) {
		UpdateWhenMov(Point2D(dPtX, dPtY)); // 移动的过程中更新一些数据。
		return TRUE;
	}

	if (m_state == PARTPMT_SELECT_FIRST_POINT)
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		Rect2D rect;
		rect.SetXMin(m_data[0] < m_data[2] ? m_data[0] : m_data[2]);
		rect.SetXMax(m_data[0] < m_data[2] ? m_data[2] : m_data[0]);
		rect.SetYMin(m_data[1] < m_data[3] ? m_data[1] : m_data[3]);
		rect.SetYMax(m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		m_pActivePartPlacements = PartLayoutHelper::GetPartPlacements_byRectSelection(rect, m_pPartPlacementList.get(), m_pPartGroupList.get(), m_rects_of_partPmt);
	} else {
		Point2D pt(dPtX, dPtY);
		m_pActivePartPlacements = PartLayoutHelper::GetPartPlacements_byClickSelection(pt, m_pPartPlacementList.get(), m_pPartGroupList.get(), m_rects_of_partPmt);
	}

	return TRUE;
}

BOOL PartPlacementSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PartPlacementEditAction::OnKeyDown(nChar, nRepCnt, nFlags);

	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
	if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputA(nChar)) {
		m_pSelectedPartPlacements->clear();
		m_pSelectedPartPlacements->insert(m_pSelectedPartPlacements->end(), m_pPartPlacementList->begin(), m_pPartPlacementList->end());
		PartPlacementPropertyBar::GetInstance()->DisplayPartPmt(m_pSelectedPartPlacements, m_pPartInstanceList, m_pViewPort);
	}
	else if (ValueInputManager::GetInstance()->IsInputDel(nChar)) {
		DeleteSelectedPartPlacements();
	}
	// 步进移动选中的零件。
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputUpArrow(nChar))
		StepMove(POSITION_UP);
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputDownArrow(nChar))
		StepMove(POSITION_DOWN);
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputLeftArrow(nChar))
		StepMove(POSITION_LEFT);
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputRightArrow(nChar))
		StepMove(POSITION_RIGHT);
	// 快速移动选中的零件。
	else if (ValueInputManager::GetInstance()->IsInputUpArrow(nChar))
		RapidMove(POSITION_UP);
	else if (ValueInputManager::GetInstance()->IsInputDownArrow(nChar))
		RapidMove(POSITION_DOWN);
	else if (ValueInputManager::GetInstance()->IsInputLeftArrow(nChar))
		RapidMove(POSITION_LEFT);
	else if (ValueInputManager::GetInstance()->IsInputRightArrow(nChar))
		RapidMove(POSITION_RIGHT);
	// 旋转选中的零件。
	else if (ValueInputManager::GetInstance()->IsInputAdd(nChar) && m_pSelectedPartPlacements->size() > 0) {
		if (m_emQuickEditType == NoneQuickEdit) {
			RotateSelectedPartPlacements(-pSheetEditParam->GetRotateStep()*D2R);
		} else {
			UpdateWhenRotate(-pSheetEditParam->GetRotateStep()*D2R);
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputDes(nChar) && m_pSelectedPartPlacements->size() > 0) {
		if (m_emQuickEditType == NoneQuickEdit) {
			RotateSelectedPartPlacements(pSheetEditParam->GetRotateStep()*D2R);
		} else {
			UpdateWhenRotate(pSheetEditParam->GetRotateStep()*D2R);
		}
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputS(nChar) && m_pSelectedPartPlacements->size() > 0) { // 逆时针旋转90度。
		if (m_emQuickEditType == NoneQuickEdit) {
			RotateSelectedPartPlacements(PID2);
		} else {
			UpdateWhenRotate(PID2);
		}
	}
	else if (!ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputW(nChar) && m_pSelectedPartPlacements->size() > 0) { // 顺时针旋转90度。
		if (m_emQuickEditType == NoneQuickEdit) {
			RotateSelectedPartPlacements(-PID2);
		} else {
			UpdateWhenRotate(-PID2);
		}
	}
	//
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputC(nChar))
	{
		if (m_pSelectedPartPlacements->size() > 0) {
			m_pBasePartPlacements->clear();
			m_pBasePartPlacements->insert(m_pBasePartPlacements->end(), m_pSelectedPartPlacements->begin(), m_pSelectedPartPlacements->end());
		}
		else {
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SELECT_PRTPMT_COPY);
			MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
		}
	}
	else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputV(nChar))
	{
		if (m_pBasePartPlacements->size() > 0) // 拷贝缓冲区中有没有“零件放置”对象。
		{
			m_emQuickEditType = Ctrl_C_Ctrl_V;
			m_quickEditBasePt = m_pBasePartPlacements->GetRectBox().GetCenterPt();

			// 为移动准备数据。
			PartPlacementListPtr pMovePartPlacements(new PartPlacementList);
			for (unsigned int i = 0; i < m_pBasePartPlacements->size(); i++)
				pMovePartPlacements->push_back(PartPlacementManager::ClonePartPmt(m_pBasePartPlacements->at(i).get()));
			UpdateBeforeMov(pMovePartPlacements, pMovePartPlacements->GetRectBox().GetCenterPt(), m_pMatSize, m_pPartInstanceList);

			PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pSelectPartOrLoopActionBar->HideActionBar();
			GetPartDistanceDetectActionBar()->Show_PartDistanceDetectActionBar();
		}
	}
    else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputQ(nChar)) // 试图移动
    {
        if (m_pSelectedPartPlacements->size() > 0) {
			// 要先隐藏这两个。
			PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pSelectPartOrLoopActionBar->HideActionBar();

			Point2D referencePt = m_pSelectedPartPlacements->GetRectBox().GetCenterPt();
            PartPlacementMoveActionPtr pPartPlacementMoveAction(new PartPlacementMoveAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList,
				m_pPartGroupList, m_pSelectedPartPlacements, referencePt, m_pViewPort, GetPartDistanceDetectActionBar()));
            m_pNextAction = pPartPlacementMoveAction;
        }
        else {
			CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SELECT_PRTPMT_MOVE);
            MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
        }
    }
    else if (ValueInputManager::GetInstance()->IsInputCtrl() && ValueInputManager::GetInstance()->IsInputW(nChar)) // 试图旋转
    {
        if (m_pSelectedPartPlacements->size() > 0) {
			// 要先隐藏这两个。
			PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pSelectPartOrLoopActionBar->HideActionBar();

            PartPlacementRotateActionPtr pPartPlacementRotateAction(new PartPlacementRotateAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList,
				m_pPartGroupList, m_pSelectedPartPlacements, m_pViewPort));
            m_pNextAction = pPartPlacementRotateAction;
        }
        else {
            CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SELECT_PRTPMT_ROTATE);
            MessageBox(m_pViewPort->GetWnd(), str, MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME), MB_OK | MB_ICONWARNING);
        }
    }
	// 现在不用这样了。
	//else if (ValueInputManager::GetInstance()->IsInputTab(nChar))
	//{
	//	if (m_emQuickEditType == Drag) // 切换到PartPlacementMoveAction动作。
	//	{
	//		m_pNextAction.reset(new PartPlacementMoveAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroupList,
	//			m_pSelectedPartPlacements, m_quickEditBasePt, m_pViewPort, GetPartDistanceDetectActionBar()));
	//	}
	//	else if (m_emQuickEditType == Ctrl_Drag) // 切换到PartPlacementCopyAction动作。
	//	{
	//		m_pNextAction.reset(new PartPlacementCopyAction(m_pMatSize, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroupList,
	//			m_pSelectedPartPlacements, m_quickEditBasePt, m_pViewPort, GetPartDistanceDetectActionBar()));
	//	}
	//}

	return TRUE;
}

BOOL PartPlacementSelectAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PartPlacementDrawer partPlacementDrawer(m_pViewPort);

    // 绘制偏移有问题的轮廓实例。
    if (pDisplayParam->GetShowIncorrectToolSize()) {
        m_pWrongToolSizeLoopInstances->Draw(m_pViewPort);
    }

	// 绘制选中的零件放置。
	partPlacementDrawer.DrawSelectedPartPlacements(m_pSelectedPartPlacements.get());

	if (m_emQuickEditType == Ctrl_C_Ctrl_V || m_emQuickEditType == Drag || m_emQuickEditType == Ctrl_Drag)
	{
		// 绘制移动中的“零件放置”。
		const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();
		partPlacementDrawer.DrawPartPlacements(pMovePartPlacements);

		// 在光标右下偏一点显示提示信息。
		CString strExtraTip;
		if (m_emQuickEditType != Ctrl_C_Ctrl_V && ValueInputManager::GetInstance()->IsInputLeftMouseButton())
			strExtraTip = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PressKey_SetPartDistance);
		DrawAssistInfo(strExtraTip);

		// draw cursor.
		m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
		if (m_emQuickEditType == Ctrl_C_Ctrl_V)
			CursorDrawer::DrawCrossLineCursor(m_pViewPort);
		else
			CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}
	else {
		if (m_state == PARTPMT_SELECT_FIRST_POINT) { // 框选。
			DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
		}

        // 高亮零件放置
        for (unsigned int i = 0; i < m_pActivePartPlacements->size(); i++) {
            const PartPlacement* pActivePartPlacement = m_pActivePartPlacements->at(i).get();
            if (!m_pSelectedPartPlacements->GetPartPlacementByID(pActivePartPlacement->GetID())) { // 要是它已在选择集中，就不用高亮方式画之。
                partPlacementDrawer.DrawActivePartPlacement(pActivePartPlacement);
            }
        }

		// draw cursor.
		CursorDrawer::DrawAimSelCursor(m_pViewPort);
	}

	return TRUE;
}

void PartPlacementSelectAction::UpdateForUndoRedo()
{
	m_pSelectedPartPlacements->clear();
    m_pActivePartPlacements->clear();
    UpdateOuterRects_ofAllPartPlacements();
	PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE);
	m_pSelectPartOrLoopActionBar->Show_SelectPartOrLoopActionBar(true);
}

void PartPlacementSelectAction::SelectPartPlacements(PartPlacementListPtr pPartPlacements)
{
	m_pSelectedPartPlacements = pPartPlacements;
	m_state = PARTPMT_SELECT_SECOND_POINT;

	// 显示属性面板，注意这里要先显示它，这样后续的action bar才能显示在正确的位置。
	PartPlacementPropertyBar::GetInstance()->DisplayPartPmt(m_pSelectedPartPlacements, m_pPartInstanceList, m_pViewPort);

	// 这里还要显示这个action bar。
	m_pSelectPartOrLoopActionBar->Show_SelectPartOrLoopActionBar(true);
}

void PartPlacementSelectAction::DeleteSelectedPartPlacements()
{
	if (m_pSelectedPartPlacements->size() > 0)
	{
		// create "Delete Pattern" command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new PartPlacementDeleteCommand(editorData, m_pPartPlacementList, m_pPartInstanceList, m_pPartGroupList, m_pSelectedPartPlacements));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

        // 清理被删除的轮廓实例。
        {
            LoopInstanceListPtr pLoopInstances_missing(new LoopInstanceList); // 被删除的轮廓实例
            LoopInstanceListPtr pLoopInstances_wrongToolSize = m_pWrongToolSizeLoopInstances->GetLoopInstances_wrongToolSize();
            for (unsigned int i = 0; i < pLoopInstances_wrongToolSize->size(); i++) {
                LoopInstancePtr pLoopInstance_wrongToolSize = pLoopInstances_wrongToolSize->operator [](i);
                if (!m_pPartInstanceList->GetPartInstanceByID(pLoopInstance_wrongToolSize->GetPartInstance()->GetID())) {
                    pLoopInstances_missing->push_back(pLoopInstance_wrongToolSize);
                }
            }
            pLoopInstances_wrongToolSize->DeleteLoopInstances(pLoopInstances_missing.get()); // 删除轮廓实例
        }

		// clear selected part pmts.
		m_pSelectedPartPlacements->clear();

		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);

		PartPlacementPropertyBar::GetInstance()->ShowBar(FALSE); // 隐藏零件属性界面，这个隐藏动作会激发mousemove事件，这样就会更新m_pActivePartPlacement变量。
	}
}

void PartPlacementSelectAction::StepMove(POSITION_INFO emDir)
{
	if (m_pSelectedPartPlacements->size() == 0)
		return;

	const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();

	// 确定矩阵。
	Matrix2D mat;
	if (emDir == POSITION_UP) {
		mat.SetTransfer(Vector2D(0, pSheetEditParam->GetMoveStep()));
	} else if (emDir == POSITION_DOWN) {
		mat.SetTransfer(Vector2D(0, -pSheetEditParam->GetMoveStep()));
	} else if (emDir == POSITION_LEFT) {
		mat.SetTransfer(Vector2D(-pSheetEditParam->GetMoveStep(), 0));
	} else if (emDir == POSITION_RIGHT) {
		mat.SetTransfer(Vector2D(pSheetEditParam->GetMoveStep(), 0));
	}

	// 移动零件。
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_MOVE_PARTPMT);
	PartPlacementListPtr pSelectedPartPlacements(new PartPlacementList);
	pSelectedPartPlacements->insert(pSelectedPartPlacements->end(), m_pSelectedPartPlacements->begin(), m_pSelectedPartPlacements->end());
	ICommandPtr pCommand(new PartPlacementTransformCommand_2(str, GetEditorData(), m_pPartPlacementList, pSelectedPartPlacements, mat));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	UpdateOuterRects_ofAllPartPlacements();
	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	if (PartPlacementPropertyBar::GetInstance()->IsVisible()) {
		PartPlacementPropertyBar::GetInstance()->UpdatePmtProp();
	}
}

void PartPlacementSelectAction::RapidMove(POSITION_INFO emDir)
{
	if (m_pSelectedPartPlacements->size() == 0)
		return;

	// 按照快速移动的先后顺序对选中的零件放置进行排序。
	sort(m_pSelectedPartPlacements->begin(), m_pSelectedPartPlacements->end(), boost::bind(&PartPlacement::BeforePartPmt, _1, _2, emDir));

	// 得到不移动的零件放置和实例。
	PartPlacementListPtr pFixPartPmts(new PartPlacementList);
	PartInstanceListPtr pFixPartInsts(new PartInstanceList);
	for (unsigned int i = 0; i < m_pPartPlacementList->size(); i++) {
		PartPlacementPtr pPartPlacement = m_pPartPlacementList->at(i);
		if (!m_pSelectedPartPlacements->GetPartPlacementByID(pPartPlacement->GetID())) {
			pFixPartPmts->push_back(pPartPlacement);
			PartInstanceListPtr pPartInstances = m_pPartInstanceList->GetPartInstanceByPartPlacement(pPartPlacement.get());
			pFixPartInsts->insert(pFixPartInsts->end(), pPartInstances->begin(), pPartInstances->end());
		}
	}

    // 零件靠碰参数
    const SheetEditParam* pSheetEditParam = CLOptionInfo::GetInstance()->GetSheetEditParam().get();
    PartAttachConfig partAttachConfig(pSheetEditParam->EnableDistanceDetect(), pSheetEditParam->EnableComDetect(), pSheetEditParam->GetPartDis(), pSheetEditParam->GetSheetMargin());

	// 准备数据。
	PartPlacementListPtr pOldPartPmts(new PartPlacementList);
	PartPlacementListPtr pNewPartPlacements(new PartPlacementList);
	for (unsigned int i = 0; i < m_pSelectedPartPlacements->size(); i++)
	{
		PartPlacementPtr pMovPartPmt = m_pSelectedPartPlacements->at(i);
		PartInstanceListPtr pMovPartInsts = m_pPartInstanceList->GetPartInstanceByPartPlacement(pMovPartPmt.get());

		// 把零件放置沿着一个方向做快速移动。
		Matrix2D retMoveMat;
		if (RapidMovePartPmt(m_pMatSize, partAttachConfig, pFixPartPmts, pFixPartInsts, emDir, pMovPartPmt, pMovPartInsts, retMoveMat)) { // 如果快速移动成功。
			// 老的零件。
			pOldPartPmts->push_back(pMovPartPmt);

			// 新的零件。
			PartPlacementPtr pNewPartPlacement = boost::dynamic_pointer_cast<PartPlacement>(pMovPartPmt->Clone());
			pNewPartPlacement->Transform(retMoveMat);
			pNewPartPlacements->push_back(pNewPartPlacement);

			// 更新不移动的零件放置和实例。
			pFixPartPmts->push_back(pNewPartPlacement);
			pFixPartInsts->AddPartInstanceByPartPlacement(pNewPartPlacement);
		} else { // 不成功的话需要把老的零件放回去。
			pFixPartPmts->push_back(pMovPartPmt);
			pFixPartInsts->insert(pFixPartInsts->end(), pMovPartInsts->begin(), pMovPartInsts->end());
		}
	}

	if (pNewPartPlacements->size() > 0) { // 移动零件放置。
		CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_MOVE_PARTPMT);
		ICommandPtr pCommand(new PartPlacementTransformCommand(str, GetEditorData(), m_pPartPlacementList, pOldPartPmts, pNewPartPlacements));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		UpdateOuterRects_ofAllPartPlacements();
		ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
		ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
		if (PartPlacementPropertyBar::GetInstance()->IsVisible()) {
			PartPlacementPropertyBar::GetInstance()->UpdatePmtProp();
		}
	}
}

void PartPlacementSelectAction::ExecuteQuickCopy()
{
	const PartPlacementList* pMovePartPlacements = GetMovePartPlacements().get();

	// 所有要新建的零件分组
	PartGroupListPtr pPartGroups_willAdded(new PartGroupList);
	{
		// 克隆零件放置后，也克隆相关的零件组，不过只需克隆顶层零件组。
		PartGroupListPtr pPartGroupList_new = PartPlacementManager::ClonePartGroups_whenClonePartPlacements_2(m_pPartGroupList.get(),
			m_pSelectedPartPlacements.get(), pMovePartPlacements);

		for (unsigned int i = 0; i < pPartGroupList_new->size(); i++)
		{
			PartGroupPtr pPartGroup = pPartGroupList_new->operator [](i);
			pPartGroups_willAdded->push_back(pPartGroup);
			PartGroupListPtr pPartGroups_tmp = pPartGroup->GetSubPartGroupsRecursively();
			pPartGroups_willAdded->insert(pPartGroups_willAdded->end(), pPartGroups_tmp->begin(), pPartGroups_tmp->end());
		}
	}

	// 复制“零件放置”对象。
	PartPlacementListPtr pNewPartPlacements(new PartPlacementList);
	pNewPartPlacements->insert(pNewPartPlacements->end(), pMovePartPlacements->begin(), pMovePartPlacements->end());
	ICommandPtr pCommand(new PartPlacementCopyCommand(GetEditorData(), m_pPartPlacementList, m_pPartInstanceList, m_pPartGroupList, pNewPartPlacements, pPartGroups_willAdded));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	UpdateOuterRects_ofAllPartPlacements();

	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_ReDisplayNestedCountOfEachPart, 0, 0);

	GetPartDistanceDetectActionBar()->HideActionBar();
	m_pSelectPartOrLoopActionBar->Show_SelectPartOrLoopActionBar(true);

	m_emQuickEditType = NoneQuickEdit;
}

void PartPlacementSelectAction::ExecuteQuickMove()
{
	// 移动零件放置。
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_MOVE_PARTPMT);
	PartPlacementListPtr pSelectedPartPmts(new PartPlacementList);
	pSelectedPartPmts->insert(pSelectedPartPmts->end(), m_pSelectedPartPlacements->begin(), m_pSelectedPartPlacements->end());
	ICommandPtr pCommand(new PartPlacementTransformCommand(str, GetEditorData(), m_pPartPlacementList, pSelectedPartPmts, GetMovePartPlacements()));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	UpdateOuterRects_ofAllPartPlacements();

	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);

	// 隐藏距离检测action bar。
	GetPartDistanceDetectActionBar()->HideActionBar();

	// 选中移动后的零件放置。
	PostMessage2EnterPartPlacementSelectAction(m_pSelectedPartPlacements.get());

	m_emQuickEditType = NoneQuickEdit;
}

void PartPlacementSelectAction::RotateSelectedPartPlacements(double dRotateAngle)
{
	if (m_pSelectedPartPlacements->size() == 0) {
		LogMgr::DebugWarn(_T("479009"));
		return;
	}

	Matrix2D mat;
	mat.RotateBy(m_pSelectedPartPlacements->GetRectBox().GetCenterPt(), dRotateAngle);

	// 执行命令。
	PartPlacementListPtr pSelectedPartPlacements(new PartPlacementList);
	pSelectedPartPlacements->insert(pSelectedPartPlacements->end(), m_pSelectedPartPlacements->begin(), m_pSelectedPartPlacements->end());
	CString str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_COMMAND_ROTATE_PARTPMT);
	ICommandPtr pCommand(new PartPlacementTransformCommand_2(str, GetEditorData(), m_pPartPlacementList, pSelectedPartPlacements, mat));
	pCommand->Do();
	CommandManager::GetInstance()->AddCommand(pCommand);

	UpdateOuterRects_ofAllPartPlacements();
	ClWindowFinder::GetCurrentView_1()->SendMessage(WM_updateSheetPreviewData, 0, 0);
	ClWindowFinder::GetCurrentView_2()->SendMessage(WM_redrawSheetThumbnailOnSheetManagementPanel, 0, 0);
	if (PartPlacementPropertyBar::GetInstance()->IsVisible()) {
		PartPlacementPropertyBar::GetInstance()->UpdatePmtProp();
	}
}

END_CUTLEADER_NAMESPACE()
