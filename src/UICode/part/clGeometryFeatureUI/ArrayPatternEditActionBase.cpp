#include "StdAfx.h"
#include "ArrayPatternEditActionBase.h"

#include "clBaseDataResource.h"
#include "GlViewPort.h"
#include "CursorDrawer.h"

#include "PatternDrawer.h"
#include "IPattern.h"
#include "clGeometryFeatureResource.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

BEGIN_CUTLEADER_NAMESPACE()

ArrayPatternEditActionBase::ArrayPatternEditActionBase(void)
{
	m_pActivePat.reset();
	m_pControlPat.reset();
}

ArrayPatternEditActionBase::~ArrayPatternEditActionBase(void)
{
}

BOOL ArrayPatternEditActionBase::LButtonDown(double& dPtX, double& dPtY)
{
	// decide the control pattern.
	// notes:
	//   1) the control pattern only can be standard pattern.
	if (!m_pActivePat || !m_pActivePat->IsStdPattern())
	{
		// display message to user.
		CString strMsg, strProductName;
		strProductName = MultiLanguageMgr::GetInstance()->TranslateString(IDS_PRODUCT_NAME);
		strMsg = MultiLanguageMgr::GetInstance()->TranslateString(IDS_OUTPUT_SELECT_STDPAT);
		MessageBox(m_pViewPort->GetWnd(), strMsg, strProductName, MB_OK | MB_ICONWARNING);

		return FALSE;
	}

	m_pControlPat = m_pActivePat;

	return TRUE;
}

BOOL ArrayPatternEditActionBase::DrawBeforeFinish()
{
	if (m_drawState == DRAW_ARRAY_NOTSTART) {
		if (m_pActivePat) {
			PatternDrawer patDrawer(m_pViewPort);
			patDrawer.DrawActivePat(m_pActivePat.get());
		}
	}

	// draw cursor.
	if (m_pControlPat)
		CursorDrawer::DrawCrossLineCursor(m_pViewPort);
	else
		CursorDrawer::DrawRectSelCursor(m_pViewPort);

	return TRUE;
}

END_CUTLEADER_NAMESPACE()
