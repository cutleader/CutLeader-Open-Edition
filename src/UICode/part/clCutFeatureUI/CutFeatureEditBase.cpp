#include "StdAfx.h"
#include "CutFeatureEditBase.h"

#include "GlViewPort.h"
#include "DummyAction.h"
#include "clUILibResource.h"
#include "ValueInputManager.h"

#include "clCutFeatureResource.h"

BEGIN_CUTLEADER_NAMESPACE()

CutFeatureEditBase::CutFeatureEditBase(PatternListPtr pPatList, PatternLoopListPtr pPatternLoopList, LoopTopologyItemListPtr pLoopTopologyItems,
	LoopCutFeatureListPtr pLoopFeatureList, GlViewPortPtr pViewPort)
{
    m_pPatList = pPatList;
    m_pPatternLoopList = pPatternLoopList;
	m_pLoopTopologyItems = pLoopTopologyItems;
    m_pLoopFeatureList = pLoopFeatureList;

    m_pViewPort = pViewPort;
}

CutFeatureEditBase::~CutFeatureEditBase(void)
{
}

EditorData CutFeatureEditBase::GetEditorData() const
{
	CString str;
	str = MultiLanguageMgr::GetInstance()->TranslateString(IDS_CUTFEATURE);
	EditorData editorData(CWnd::FromHandle(m_pViewPort->GetWnd()), str);

	return editorData;
}

END_CUTLEADER_NAMESPACE()
