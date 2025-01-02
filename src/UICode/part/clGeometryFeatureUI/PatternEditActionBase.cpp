#include "StdAfx.h"
#include "PatternEditActionBase.h"

#include "GlViewPort.h"
#include "DummyAction.h"
#include "clUILibResource.h"
#include "ValueInputManager.h"
#include "LogMgr.h"
#include "ClWindowFinder.h"

#include "clGeometryFeatureResource.h"
#include "PatternList.h"
#include "PatternPropertyBar.h"
#include "PatternSelectAction.h"
#include "clGeometryFeatureUIResource.h"
#include "TextStruct.h"
#include "TextStructList.h"
#include "PartCadData.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

PatternEditActionBase::PatternEditActionBase(void)
{
}

PatternEditActionBase::~PatternEditActionBase(void)
{
}

EditorData PatternEditActionBase::GetEditorData() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_GEOMETRY);

	EditorData editorData(CWnd::FromHandle(m_pViewPort->GetWnd()), str);
	return editorData;
}

BOOL PatternEditActionBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ActionBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (ValueInputManager::GetInstance()->IsInputCancel(nChar))
		PostMessage2EnterPatternSelectAction();

	return TRUE;
}

SnapInputPtr PatternEditActionBase::GetSnapInput() const
{
	DWORD dwSnapStyle = SnapEndPtOf2DLineArc | SnapMiddlePtOf2DLineArc | SnapCenterPtOf2DArc | SnapClosestPtTo2DLineArc | SnapClosestPtToOneAssistLine |
		SnapCrossPtOfTwo2DLineArc | SnapCrossPtOfTwoAssistLines;
	SnapInputPtr pSnapInput(new SnapInput(dwSnapStyle, m_pPartCadData->GetPatternList(), Matrix2D()));
	return pSnapInput;
}

void PatternEditActionBase::PostMessage2EnterPatternSelectAction()
{
	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectGeometryAction, 0, 0);
}

void PatternEditActionBase::PostMessage2EnterPatternSelectAction(const IPattern* pSelectingPattern)
{
	// 要预选中的几何轮廓的ID。
	if (!pSelectingPattern) {
		LogMgr::DebugWarn(_T("564352"));
		return;
	}
	vector<LONGLONG>* pPatternIDs_preSelect = new vector<LONGLONG>();
	pPatternIDs_preSelect->push_back(pSelectingPattern->GetID());

	// 要预选中的文字对象的ID，这里需要传送一个空集合。
	vector<LONGLONG>* pTextStructIDs_preSelect = new vector<LONGLONG>();

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectGeometryAction_AndPreSelect, (WPARAM)pPatternIDs_preSelect, (LPARAM)pTextStructIDs_preSelect);
}

void PatternEditActionBase::PostMessage2EnterPatternSelectAction(const PatternList* pSelectingPatterns, const TextStructList* pSelectingTextStructs)
{
	// 要预选中的几何轮廓的ID。
	vector<LONGLONG>* pPatternIDs_preSelect = new vector<LONGLONG>();
	{
		if (!pSelectingPatterns || pSelectingPatterns->size() == 0) {
			LogMgr::DebugWarn(_T("704073"));
			return;
		}
		for (unsigned int i = 0; i < pSelectingPatterns->size(); i++) {
			const IPattern* pPattern = pSelectingPatterns->operator [](i).get();
			pPatternIDs_preSelect->push_back(pPattern->GetID());
		}
	}

	// 要预选中的文字对象的ID。
	vector<LONGLONG>* pTextStructIDs_preSelect = new vector<LONGLONG>();
	for (unsigned int i = 0; i < pSelectingTextStructs->size(); i++)
		pTextStructIDs_preSelect->push_back(pSelectingTextStructs->operator [](i)->GetID());

	ClWindowFinder::GetCurrentView_1()->PostMessage(WM_EnterSelectGeometryAction_AndPreSelect, (WPARAM)pPatternIDs_preSelect, (LPARAM)pTextStructIDs_preSelect);
}

END_CUTLEADER_NAMESPACE()
