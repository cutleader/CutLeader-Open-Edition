#include "StdAfx.h"
#include "PatternSelectAction.h"

#include "MathEx.h"
#include "GlViewPort.h"
#include "Rect2D.h"
#include "CommandManager.h"
#include "ValueInputManager.h"
#include "clUILibResource.h"
#include "DummyAction.h"
#include "CursorDrawer.h"
#include "Point2DList.h"
#include "clBaseDataResource.h"
#include "LogMgr.h"
#include "Polygon2DList.h"
#include "Text2ContourHelper.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"
#include "PartEditParam.h"

#include "PatternDrawer.h"
#include "SnapPtDrawer.h"
#include "PatternPropertyBar.h"
#include "PatternDeleteCommand.h"
#include "IPattern.h"
#include "IArrayPattern.h"
#include "PatternList.h"
#include "PatternLoopList.h"
#include "clGeometryFeatureResource.h"
#include "LinePatternEditAction.h"
#include "ArcPatternEditAction.h"
#include "ArcPattern3PtEditAction.h"
#include "MultiLinePatternEditAction.h"
#include "RectLinesDrawAction.h"
#include "RoundCornerHolePatternEditAction.h"
#include "EllipsePatternEditAction.h"
#include "RoundCornerLineArcsDrawAction.h"
#include "RectHolePatternEditAction.h"
#include "CirclePatternEditAction.h"
#include "SingleDPatternEditAction.h"
#include "DoubleDPatternEditAction.h"
#include "LineArrayPatternEditAction.h"
#include "ArcArrayPatternEditAction.h"
#include "CircleArrayPatternEditAction.h"
#include "GridArrayPatternEditAction.h"
#include "TriangleHolePatternEditAction.h"
#include "OBroundPatternEditAction.h"
#include "RadSlotPatternEditAction.h"
#include "PartCadData.h"
#include "GeometryFeatureHelper.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "TextParamActionBar.h"
#include "clGeometryFeatureUIResource.h"
#include "TextEditCommand.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternSelectAction::PatternSelectAction(PartCadDataPtr pPartCadData, TextParamActionBar* pTextParamActionBar, GlViewPortPtr pViewPort)
{
	m_pPartCadData = pPartCadData;
	m_pViewPort = pViewPort;

	m_pActivePats.reset(new PatternList);
	m_pSelectedPats.reset(new PatternList);
	m_pActiveTextStructs.reset(new TextStructList);
	m_pSelectedTextStructs.reset(new TextStructList);

	m_state = DRAW_SELECT_NONE;
	m_iMsgID = IDS_OUTPUT_SELECTPATTERN;

	m_pTextParamActionBar = pTextParamActionBar;
	m_pTextParamActionBar->AddObserver(this);
}

PatternSelectAction::~PatternSelectAction(void)
{
	m_pTextParamActionBar->RemoveObserver(GetID());
}

BOOL PatternSelectAction::LButtonDown(double& dPtX, double& dPtY)
{
	Point2D pt(dPtX, dPtY);
	if (CheckWillEnterGripEditAction(pt)) // 要进行夹点编辑。
	{
		SwitchToGripEdit(m_pActivePats->front(), pt);
		return TRUE;
	}

	if (m_state == DRAW_SELECT_NONE || m_state == DRAW_SELECT_SECOND_INPUT)
	{
		m_state = DRAW_SELECT_FIRST_INPUT;
		m_iMsgID = IDS_OUTPUT_SELECTPAT_NEXTPT;

		m_data[0] = dPtX;
		m_data[1] = dPtY;
		m_data[2] = dPtX;
		m_data[3] = dPtY;
	}

	return TRUE;
}

BOOL PatternSelectAction::LButtonUp(double& dPtX, double& dPtY)
{
	PatternListPtr pPatList = m_pPartCadData->GetPatternList();
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();

	if (m_state == DRAW_SELECT_FIRST_INPUT)
	{
		m_state = DRAW_SELECT_SECOND_INPUT;
		m_iMsgID = IDS_OUTPUT_SELECTPATTERN;
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		// 更新选择集。
		if (!ValueInputManager::GetInstance()->IsInputCtrl()) // 如果没有按下control键，那么就换掉选择集。
		{
			m_pSelectedPats->clear();
			m_pSelectedPats->AddPatterns(m_pActivePats);
			m_pSelectedTextStructs->clear();
			m_pSelectedTextStructs->insert(m_pSelectedTextStructs->end(), m_pActiveTextStructs->begin(), m_pActiveTextStructs->end());
		}
		else { // 如果按下了control键，那么需要更新现有选择集。
			for (unsigned int i = 0; i < m_pActivePats->size(); i++) {
				IPatternPtr pActivePat = m_pActivePats->operator[](i);
				if (m_pSelectedPats->GetPatternFromID(pActivePat->GetID()) == nullptr) {
					m_pSelectedPats->push_back(pActivePat);
				}
			}
			for (unsigned int i = 0; i < m_pActiveTextStructs->size(); i++) {
				TextStructPtr pActiveTextStruct = m_pActiveTextStructs->operator [](i);
				if (m_pSelectedTextStructs->GetTextStructByID(pActiveTextStruct->GetID()) == nullptr) {
					m_pSelectedTextStructs->push_back(pActiveTextStruct);
				}
			}
		}
		m_pSelectedPats->SetModified(TRUE); // 这里会更新几何特征集合的时间戳，这样特征点捕捉中会因此更新缓存。

		// 更新界面。
		if (m_pSelectedPats->size() > 0) {
			PatternPropertyBar::GetInstance()->DisplayPatProp(pPatternLoopList, m_pSelectedPats, pLoopTopologyItems, m_pSelectedTextStructs);
			if (m_pSelectedTextStructs->size() == 1) { // 当选中一个文字对象时，显示action bar来编辑文字参数。
				m_pTextParamActionBar->Show_TextParamActionBar(false, CRect());
				const TextStruct* pTextStruct = m_pSelectedTextStructs->front().get();
				TextParamPtr pNewTextParam(new TextParam(*pTextStruct->GetTextParam()));
				m_pTextParamActionBar->InitTextParam(pNewTextParam);
			}
			else {
				m_pTextParamActionBar->HideActionBar();
			}
		}
		else {
			PatternPropertyBar::GetInstance()->ShowBar(FALSE);
			m_pTextParamActionBar->HideActionBar();
		}
	}

	return TRUE;
}

PatternListPtr PatternSelectAction::GetSelectedPats(TextStructListPtr pSelectedTextStructs_return)
{
	pSelectedTextStructs_return->insert(pSelectedTextStructs_return->end(), m_pSelectedTextStructs->begin(), m_pSelectedTextStructs->end());
	return m_pSelectedPats;
}

void PatternSelectAction::SelectPats(PatternListPtr pPatList, TextStructListPtr pTextStructs)
{
	m_pSelectedPats = pPatList;
	m_pSelectedTextStructs = pTextStructs;
	m_state = DRAW_SELECT_SECOND_INPUT;

	// whether need to show prop bar.
	PatternPropertyBar* pPatternPropBar = PatternPropertyBar::GetInstance();
	if (!pPatternPropBar->IsVisible())
		pPatternPropBar->ShowPane(TRUE, FALSE, TRUE);

	// if PatternPropertyBar is visible, we should update it.
	PatternLoopListPtr pPatternLoopList = m_pPartCadData->GetPatternLoopList();
	LoopTopologyItemListPtr pLoopTopologyItems = m_pPartCadData->GetLoopTopologyItems_FW();
	if (pPatternPropBar->IsVisible())
		pPatternPropBar->DisplayPatProp(pPatternLoopList, m_pSelectedPats, pLoopTopologyItems, m_pSelectedTextStructs);

	// 当选中一个文字对象时，显示action bar来编辑文字参数。
	if (m_pSelectedTextStructs->size() == 1) {
		m_pTextParamActionBar->Show_TextParamActionBar(false, CRect());
		const TextStruct* pTextStruct = m_pSelectedTextStructs->front().get();
		TextParamPtr pNewTextParam(new TextParam(*pTextStruct->GetTextParam()));
		m_pTextParamActionBar->InitTextParam(pNewTextParam);
	}
	else {
		m_pTextParamActionBar->HideActionBar();
	}
}

BOOL PatternSelectAction::MovePoint(double& dPtX, double& dPtY)
{
	m_pActivePats->clear();
	m_pActiveTextStructs->clear();

	if (m_state == DRAW_SELECT_FIRST_INPUT) // 框选。
	{
		m_data[2] = dPtX;
		m_data[3] = dPtY;

		Rect2D rect(m_data[0] < m_data[2] ? m_data[0] : m_data[2], m_data[0] < m_data[2] ? m_data[2] : m_data[0],
			m_data[1] < m_data[3] ? m_data[1] : m_data[3], m_data[1] < m_data[3] ? m_data[3] : m_data[1]);
		TextStructListPtr pTextStructs(new TextStructList);
		m_pActivePats = GeometryFeatureHelper::GetPatterns_byRectSelection(m_pPartCadData.get(), rect, pTextStructs);
		if (pTextStructs->size() > 0) {
			m_pActiveTextStructs->insert(m_pActiveTextStructs->end(), pTextStructs->begin(), pTextStructs->end());
		}
	}
	else // 点选。
	{
		TextStructPtr pTextStruct;
		m_pActivePats = GeometryFeatureHelper::GetPatterns_byClickSelection(m_pPartCadData.get(), Point2D(dPtX, dPtY), pTextStruct);
		if (pTextStruct) {
			m_pActiveTextStructs->push_back(pTextStruct);
		}
	}

	return TRUE;
}

BOOL PatternSelectAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	PatternEditActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputDel(nChar)) {
		DeleteSelectedPats();
	}

	return TRUE;
}

BOOL PatternSelectAction::DrawBeforeFinish()
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();
	PatternDrawer patternDrawer(m_pViewPort);
	SnapPtDrawer snapPtDrawer(m_pViewPort);

	// if left button down, we should draw a rectangle in stipple mode.
	if (m_state == DRAW_SELECT_FIRST_INPUT) {
		DrawSelectionBox(m_data[0], m_data[1], m_data[2], m_data[3]);
	}

	// 绘制高亮的和选中的几何特征。
	for (unsigned int i = 0; i < m_pActivePats->size(); i++) {
		IPatternPtr pActivePattern = m_pActivePats->operator[](i);
		if (m_pSelectedPats->GetPatternFromID(pActivePattern->GetID()) == nullptr) { // 这个几何特征不在选择集中。
			patternDrawer.DrawActivePat(pActivePattern.get());
		}
	}
	patternDrawer.DrawSelPatList(m_pSelectedPats.get());

	if (m_pSelectedPats->size() == 1) { // 仅当只选中一个几何特征时，才显示其控制点。
		snapPtDrawer.DrawPatListCtrlPt(m_pSelectedPats);
	}

	// draw cursor.
	CursorDrawer::DrawAimSelCursor(m_pViewPort);

	return TRUE;
}

SnapInputPtr PatternSelectAction::GetSnapInput() const
{
	SnapInputPtr pSnapInput = SnapInput::BuildSnapInput4SnapNone();

	if (m_state == DRAW_SELECT_FIRST_INPUT) // 框选。
	{
		// 不要开启捕捉功能。
		pSnapInput = SnapInput::BuildSnapInput4SnapNone();
	}
	else // 点选
	{
		// 只有当选中了一个几何特征时，才开启控制点捕捉。
		if (m_pSelectedPats->size() == 1)
			pSnapInput.reset(new SnapInput(SnapControlPtOfPattern, m_pSelectedPats, Matrix2D()));
		else
			pSnapInput = SnapInput::BuildSnapInput4SnapNone(); // 不要开启捕捉功能。
	}

	return pSnapInput;
}

void PatternSelectAction::UpdateForUndoRedo()
{
	m_pActivePats->clear();
	m_pSelectedPats->clear();
	m_pSelectedPats->SetModified(TRUE); // 这里会更新几何特征集合的时间戳，这样特征点捕捉中会因此更新缓存。
	m_pActiveTextStructs->clear();
	m_pSelectedTextStructs->clear();

	PatternPropertyBar::GetInstance()->ShowBar(FALSE);
	m_pTextParamActionBar->ShowWindow(SW_HIDE);
}

void PatternSelectAction::Update(int iEventID, const CComVariant& varEventContent)
{
	if (iEventID == ID_EVENT_TextParamChanged)
	{
		// 得到新的文字参数。
		const TextParam* pTextParam = m_pTextParamActionBar->GetTextParam();
		TextParamPtr pNewTextParam(new TextParam(*pTextParam));

		// 要编辑的文字对象。
		if (m_pSelectedTextStructs->size() != 1)
		{
			LogMgr::DebugWarn(_T("653890"));
			return;
		}
		LONGLONG iTextStructID = m_pSelectedTextStructs->front()->GetID();

		// 重新生成文字的轮廓。
		Polygon2DListPtr pNewTextContours(new Polygon2DList);
		{
			const CString& strTextContent = pNewTextParam->GetTextContent();
			const CString& strFontFileName = pNewTextParam->GetFontFileName();
			double dTextHeight = pNewTextParam->GetTextHeight();
			double dTextWidth = pNewTextParam->GetTextWidth();
			double dTextTiltAngle = pNewTextParam->GetTextTiltAngle();
			const CString& strDefaultFontFileName = CLOptionInfo::GetInstance()->GetPartEditParam()->GetDefaultFontFileName();
			TextContourGenerationErrorCode emErrorCode = Text2ContourHelper::GenerateTextContours(strTextContent, strFontFileName, dTextHeight, dTextWidth, dTextTiltAngle,
				strDefaultFontFileName, pNewTextContours);
			if (emErrorCode != Succeed)
			{
				CString strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
				CString strMessage = MultiLanguageMgr::GetInstance()->TranslateString(IDS_MSG_CannotCreateText);
				MessageBox(m_pViewPort->GetWnd(), strMessage, strProductName, MB_OK | MB_ICONWARNING);
				return;
			}
		}

		// 执行命令。
		ICommandPtr pCommand(new TextEditCommand(GetEditorData(), m_pPartCadData, iTextStructID, pNewTextParam, pNewTextContours));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// 重置数据。
		{
			TextStructPtr pTextStruct = m_pPartCadData->GetTextStructs()->GetTextStructByID(iTextStructID);

			// 更新当前的文字和选中的文字对象。
			m_pActiveTextStructs->clear();
			m_pActiveTextStructs->push_back(pTextStruct);
			m_pSelectedTextStructs->clear();
			m_pSelectedTextStructs->push_back(pTextStruct);

			// 更新当前的几何特征和选中的几何特征。
			const PatternList* pInvolvedPatterns = pTextStruct->GetInvolvedPatterns().get();
			m_pActivePats->clear();
			m_pActivePats->insert(m_pActivePats->end(), pInvolvedPatterns->begin(), pInvolvedPatterns->end());
			m_pSelectedPats->clear();
			m_pSelectedPats->insert(m_pSelectedPats->end(), pInvolvedPatterns->begin(), pInvolvedPatterns->end());
		}

		CWnd::FromHandle(m_pViewPort->GetWnd())->Invalidate(); // 更新零件视图。
		PatternPropertyBar::GetInstance()->DisplayPatProp(m_pPartCadData->GetPatternLoopList(), m_pSelectedPats,
			m_pPartCadData->GetLoopTopologyItems_FW(), m_pSelectedTextStructs); // 更新几何特征属性面板。
	}
}

bool PatternSelectAction::CheckWillEnterGripEditAction(const Point2D& pt)
{
	if (m_pSelectedPats->size() != 1)
		return false;

	const IPattern* pPattern = m_pSelectedPats->front().get();
	Point2DList pts;
	pPattern->GetSnapPoints(pts);
	for (unsigned int i = 0; i < pts.size(); i++)
	{
		if (pt.DistanceTo(pts[i]) < GEOM_TOLERANCE)
			return true;
	}

	return false;
}

void PatternSelectAction::SwitchToGripEdit(IPatternPtr pPattern, Point2D snapPt)
{
	PatternType emPatternType = pPattern->GetPatternType();

	// for polygon patterns.
	if (emPatternType == PATTERN_LINE)
		m_pNextAction.reset(new LinePatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_ARC)
		m_pNextAction.reset(new ArcPatternEditAction(m_pPartCadData, m_pViewPort));
	// for std patterns.
	else if(emPatternType == PATTERN_RECT_HOLE)
		m_pNextAction.reset(new RectHolePatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_TR_HOLE)
		m_pNextAction.reset(new TriangleHolePatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_ROUNDHOLE)
		m_pNextAction.reset(new CirclePatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_OBROUND)
		m_pNextAction.reset(new OBroundPatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_ELLIPSE)
		m_pNextAction.reset(new EllipsePatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_SINGLED)
		m_pNextAction.reset(new SingleDPatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_DOUBLED)
		m_pNextAction.reset(new DoubleDPatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_RCHOLE)
		m_pNextAction.reset(new RoundCornerHolePatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_RADSLOT)
		m_pNextAction.reset(new RadSlotPatternEditAction(m_pPartCadData, m_pViewPort));
	// for grid patterns.
	else if(emPatternType == PATTERN_PATTERNLINE)
		m_pNextAction.reset(new LineArrayPatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_PATTERNARC)
		m_pNextAction.reset(new ArcArrayPatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_PATTERNCIRCLE)
		m_pNextAction.reset(new CircleArrayPatternEditAction(m_pPartCadData, m_pViewPort));
	else if(emPatternType == PATTERN_PATTERNGRID)
		m_pNextAction.reset(new GridArrayPatternEditAction(m_pPartCadData, m_pViewPort));

	// init the data for Grip-edit.
	IPatternGripEditAction* pGripEditAction = dynamic_cast<IPatternGripEditAction*>(m_pNextAction.get());
	pGripEditAction->SetGripEditPattern(pPattern);
	pGripEditAction->SetGripEditPt(snapPt.X(), snapPt.Y());
}

void PatternSelectAction::DeleteSelectedPats()
{
	if (m_pSelectedPats->size() > 0)
	{
		// when delete the grid pattern, the control pattern will also be deleted.
		PatternListPtr pSelPatList(new PatternList());
		for (unsigned int i = 0; i < m_pSelectedPats->size(); i++)
		{
			IPatternPtr pPattern = m_pSelectedPats->at(i);
			pSelPatList->push_back(pPattern);

			// check whether it is a grid pattern.
			if (pPattern->IsArrayPattern())
			{
				IArrayPatternPtr pArrayPattern = boost::dynamic_pointer_cast<IArrayPattern>(pPattern);
				IPatternPtr pCtrlPat = pArrayPattern->GetControlPattern();
				pSelPatList->push_back(pCtrlPat);
			}
		}

		// 要删除的文字。
		vector<LONGLONG> textStructs_willDelete;
		for (unsigned int i = 0; i < m_pSelectedTextStructs->size(); i++)
			textStructs_willDelete.push_back(m_pSelectedTextStructs->operator[](i)->GetID());

		// create "Delete Pattern" command
		EditorData editorData = GetEditorData();
		ICommandPtr pCommand(new PatternDeleteCommand(editorData, m_pPartCadData, pSelPatList, textStructs_willDelete));
		pCommand->Do();
		CommandManager::GetInstance()->AddCommand(pCommand);

		// clear selected patterns.
		m_pSelectedPats->clear();

		PatternPropertyBar::GetInstance()->ShowBar(FALSE); // 隐藏图形属性界面，这个隐藏动作会激发mousemove事件，这样就会更新m_pActivePat变量。
		m_pTextParamActionBar->HideActionBar();
	}
}

END_CUTLEADER_NAMESPACE()
