#include "StdAfx.h"
#include "ActionBase.h"

#include "Point2DList.h"
#include "GlViewPort.h"
#include "clUILibResource.h"
#include "ValueInputManager.h"
#include "DummyAction.h"
#include "CLOptionInfo.h"
#include "DisplayParam.h"

BEGIN_CUTLEADER_NAMESPACE()

ActionBase::ActionBase(void)
{
	m_pInputPtList.reset(new Point2DList);
	m_iMsgID = INVALID_ID;
	m_pNextAction.reset();
}

ActionBase::~ActionBase(void)
{
}

Point2DPtr ActionBase::GetFirstInput() const
{
	Point2DPtr ptPtr;

	if (m_pInputPtList->size() == 0)
		return ptPtr;

	ptPtr.reset(new Point2D(m_pInputPtList->at(0)));
	return ptPtr;
}

void ActionBase::GetPromptMessage(int& msgID) const
{
	msgID = m_iMsgID;
}

SnapInputPtr ActionBase::GetSnapInput() const
{
	return SnapInput::BuildSnapInput4SnapNone();
}

void ActionBase::DrawSelectionBox(double dLeftTopPointX, double dLeftTopPointY, double dRightBottomPointX, double dRightBottomPointY)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// ±¸·Ý»æÍ¼ÉèÖÃ¡£
	bool oldStipple = false;
	int oldRepeat = 1;
	unsigned short oldPattern = 0xffff;
	m_pViewPort->GetLineStipple(oldStipple, oldRepeat, oldPattern);
	COLORREF dwOldColor;
	m_pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = m_pViewPort->GetLineWidth();

	// draw the selection rectangle.
	m_pViewPort->SetLineStipple(true, 2, 0xcccc);
	m_pViewPort->SetLineWidth(1);
	m_pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());
	m_pViewPort->DrawRect(dLeftTopPointX, dLeftTopPointY, dRightBottomPointX, dRightBottomPointY);

	// restore the drawer
	m_pViewPort->SetLineStipple(oldStipple, oldRepeat, oldPattern);
	m_pViewPort->SetDrawColor(dwOldColor);
	m_pViewPort->SetLineWidth(iOldLineWid);
}

END_CUTLEADER_NAMESPACE()
