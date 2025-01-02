#include "StdAfx.h"
#include "CursorDrawer.h"

#include "GlViewPort.h"

#include "CLOptionInfo.h"
#include "DisplayParam.h"

BEGIN_CUTLEADER_NAMESPACE()

void CursorDrawer::DrawDefaultCursor(GlViewPortPtr pViewPort)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// keep the old config.
	COLORREF dwOldColor;
	pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = pViewPort->GetLineWidth();

	//
	pViewPort->SetLineWidth(1);
	pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());


	/************************************************************************/
	// decide each point.

	// get the current cursor point(pt 1).
	double dCursorPtX, dCursorPtY;
	pViewPort->GetCursorPos(dCursorPtX, dCursorPtY);
	pViewPort->WToD(dCursorPtX, dCursorPtY);

	// decide pt 2.
	double dPt2X = dCursorPtX;
	double dPt2Y = dCursorPtY+30;

	// decide pt 3.
	double dPt3X = dPt2X+6.467;
	double dPt3Y = dPt2Y-2.679;

	// decide pt 4.
	double dPt4X = dPt3X+4.090;
	double dPt4Y = dPt3Y+9.875;

	// decide pt 5.
	double dPt5X = dPt4X+8.279;
	double dPt5Y = dPt4Y-3.429;

	// decide pt 6.
	double dPt6X = dPt5X-4.090;
	double dPt6Y = dPt5Y-9.875;

	// decide pt 7.
	double dPt7X = dPt6X+6.467;
	double dPt7Y = dPt6Y-2.679;
	/************************************************************************/


	/************************************************************************/
	// draw these lines.

	// convert the device coordinate to world coordinate.
	pViewPort->DToW(dCursorPtX, dCursorPtY);
	pViewPort->DToW(dPt2X, dPt2Y);
	pViewPort->DToW(dPt3X, dPt3Y);
	pViewPort->DToW(dPt4X, dPt4Y);
	pViewPort->DToW(dPt5X, dPt5Y);
	pViewPort->DToW(dPt6X, dPt6Y);
	pViewPort->DToW(dPt7X, dPt7Y);

	//
	pViewPort->DrawLinePoint2D(dCursorPtX, dCursorPtY, dPt2X, dPt2Y);
	pViewPort->DrawLinePoint2D(dPt2X, dPt2Y, dPt3X, dPt3Y);
	pViewPort->DrawLinePoint2D(dPt3X, dPt3Y, dPt4X, dPt4Y);
	pViewPort->DrawLinePoint2D(dPt4X, dPt4Y, dPt5X, dPt5Y);
	pViewPort->DrawLinePoint2D(dPt5X, dPt5Y, dPt6X, dPt6Y);
	pViewPort->DrawLinePoint2D(dPt6X, dPt6Y, dPt7X, dPt7Y);
	pViewPort->DrawLinePoint2D(dPt7X, dPt7Y, dCursorPtX, dCursorPtY);
	/************************************************************************/


	// restore the old config.
	pViewPort->SetDrawColor(dwOldColor);
	pViewPort->SetLineWidth(iOldLineWid);
}

void CursorDrawer::DrawAimSelCursor(GlViewPortPtr pViewPort)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// keep the old config.
	COLORREF dwOldColor;
	pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = pViewPort->GetLineWidth();

	//
	pViewPort->SetLineWidth(1);
	pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());

	// draw the cross lines and the rect.
	double dCursorPtX, dCursorPtY;
	pViewPort->GetCursorPos(dCursorPtX, dCursorPtY);
	pViewPort->DrawCrossPoint2D(dCursorPtX, dCursorPtY, 40, 0);
	pViewPort->DrawRectPoint2D(dCursorPtX, dCursorPtY, 5, 0, true);

	// restore the old config.
	pViewPort->SetDrawColor(dwOldColor);
	pViewPort->SetLineWidth(iOldLineWid);
}

void CursorDrawer::DrawRectSelCursor(GlViewPortPtr pViewPort)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// keep the old config.
	COLORREF dwOldColor;
	pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = pViewPort->GetLineWidth();

	//
	pViewPort->SetLineWidth(1);
	pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());

	// get the current cursor point.
	double dCursorPtX, dCursorPtY;
	pViewPort->GetCursorPos(dCursorPtX, dCursorPtY);

	// draw the rect.
	pViewPort->DrawRectPoint2D(dCursorPtX, dCursorPtY, 10, 0, false);

	// restore the old config.
	pViewPort->SetDrawColor(dwOldColor);
	pViewPort->SetLineWidth(iOldLineWid);
}

void CursorDrawer::DrawCrossLineCursor(GlViewPortPtr pViewPort)
{
	const DisplayParam* pDisplayParam = CLOptionInfo::GetInstance()->GetDisplayParam().get();

	// keep the old config.
	COLORREF dwOldColor;
	pViewPort->GetDrawColor(dwOldColor);
	int iOldLineWid = pViewPort->GetLineWidth();

	//
	pViewPort->SetLineWidth(1);
	pViewPort->SetDrawColor(*pDisplayParam->GetPatColor());

	//
	double dCursorPtX, dCursorPtY;
	pViewPort->GetCursorPos(dCursorPtX, dCursorPtY);
	pViewPort->DrawCrossPoint2D(dCursorPtX, dCursorPtY, 40, 0);

	// restore the old config.
	pViewPort->SetDrawColor(dwOldColor);
	pViewPort->SetLineWidth(iOldLineWid);
}

END_CUTLEADER_NAMESPACE()
