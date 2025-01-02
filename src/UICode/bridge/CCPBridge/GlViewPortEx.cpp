#include "GlViewPortEx.h"
#include "GlViewPort.h"

BEGIN_CUTLEADER_NAMESPACE()

GlViewPortEx::GlViewPortEx(void)
{
	m_ppGlViewPort = new GlViewPortPtr;
	m_ppGlViewPort->reset(new GlViewPort);
}

GlViewPortEx::!GlViewPortEx()
{
	delete m_ppGlViewPort;
}

Boolean GlViewPortEx::InitEnv(IntPtr hWnd, double dMinZoomRatio, double dMaxZoomRatio)
{
	HWND hTmpWnd = (HWND)hWnd.ToInt32();
	return (*m_ppGlViewPort)->InitEnv(hTmpWnd, dMinZoomRatio, dMaxZoomRatio);
}

Boolean GlViewPortEx::BindRendContext()
{
	return (*m_ppGlViewPort)->BindRendContext();
}

Boolean GlViewPortEx::SetDrawingArea(double dXWorldRange, double dClientWidth, double dClientHeight, Point2DEx^ leftBottomPt)
{
	return (*m_ppGlViewPort)->SetDrawingArea(dXWorldRange, dClientWidth, dClientHeight, leftBottomPt->X(), leftBottomPt->Y());
}

void GlViewPortEx::GetDrawingArea(double% dXWorldRange, double% dClientWidth, double% dClientHeight, double% dLeftBottomPtX, double% dLeftBottomPtY)
{
	double dXWorldRange_tmp = dXWorldRange;
	double dClientWidth_tmp = dClientWidth;
	double dClientHeight_tmp = dClientHeight;
	double dLeftBottomPtX_tmp = dLeftBottomPtX;
	double dLeftBottomPtY_tmp = dLeftBottomPtY;
	(*m_ppGlViewPort)->GetDrawingArea(dXWorldRange_tmp, dClientWidth_tmp, dClientHeight_tmp, dLeftBottomPtX_tmp, dLeftBottomPtY_tmp);
	dXWorldRange = dXWorldRange_tmp;
	dClientWidth = dClientWidth_tmp;
	dClientHeight = dClientHeight_tmp;
	dLeftBottomPtX = dLeftBottomPtX_tmp;
	dLeftBottomPtY = dLeftBottomPtY_tmp;
}

Boolean GlViewPortEx::SetLeftBottomPt(Point2DEx^ leftBottomPt)
{
	return (*m_ppGlViewPort)->SetLeftBottomPt(leftBottomPt->X(), leftBottomPt->Y());
}

double GlViewPortEx::GetFitAllParam(double dClientWidth, double dClientHeight, Rect2DEx^ geomRect, double dMarginRatio, Point2DEx^ leftBottomPt)
{
	Rect2D rect = *(geomRect->GetRect2D());
	double dLeftBottomPtX = .0, dLeftBottomPtY = .0;
	double dXDirRange = (*m_ppGlViewPort)->GetFitAllParam(dClientWidth, dClientHeight, rect.GetXMin(), rect.GetXMax(),
		rect.GetYMin(), rect.GetYMax(), dMarginRatio, dLeftBottomPtX, dLeftBottomPtY);
	leftBottomPt->X(dLeftBottomPtX);
	leftBottomPt->Y(dLeftBottomPtY);
	return dXDirRange;
}

Boolean GlViewPortEx::ZoomViewPort(double dZoomFactor, double dClientX, double dClientY)
{
	return (*m_ppGlViewPort)->ZoomViewPort(dZoomFactor, dClientX, dClientY);
}

Boolean GlViewPortEx::PanViewPort(double dXClientOffSet, double dYClientOffSet)
{
	return (*m_ppGlViewPort)->PanViewPort(dXClientOffSet, dYClientOffSet);
}

Boolean GlViewPortEx::ClearScreen()
{
	return (*m_ppGlViewPort)->ClearScreen();
}

Boolean GlViewPortEx::SetBackgroundColor(Color color)
{
	return (*m_ppGlViewPort)->SetBackgroundColor(ColorTranslator::ToOle(color));
}

void GlViewPortEx::WToD(Double% dPtX, Double% dPtY)
{
	double dTmpPtX = dPtX;
	double dTmpPtY = dPtY;
	(*m_ppGlViewPort)->WToD(dTmpPtX, dTmpPtY);
	dPtX = dTmpPtX;
	dPtY = dTmpPtY;
}

void GlViewPortEx::DToW(Double% dPtX, Double% dPtY)
{
	double dTmpPtX = dPtX;
	double dTmpPtY = dPtY;
	(*m_ppGlViewPort)->DToW(dTmpPtX, dTmpPtY);
	dPtX = dTmpPtX;
	dPtY = dTmpPtY;
}

void GlViewPortEx::WToD(Double% dLength)
{
	double dTmpLength = dLength;
	(*m_ppGlViewPort)->WToD(dTmpLength);
	dLength = dTmpLength;
}

void GlViewPortEx::DToW(Double% dLength)
{
	double dTmpLength = dLength;
	(*m_ppGlViewPort)->DToW(dTmpLength);
	dLength = dTmpLength;
}

Boolean GlViewPortEx::GetROP(ROP_MODE_EX% ropMode)
{
	ROP_MODE tmpRopMode;
	bool bRet = (*m_ppGlViewPort)->GetROP(tmpRopMode);
	if (bRet)
	{
		if (tmpRopMode == ROP_COPY)
			ropMode = ROP_MODE_EX::ROP_EX_COPY;
		if (tmpRopMode == ROP_NORMAL)
			ropMode = ROP_MODE_EX::ROP_EX_NORMAL;
		if (tmpRopMode == ROP_XOR)
			ropMode = ROP_MODE_EX::ROP_EX_XOR;

		return true;
	}

	return false;
}

Boolean GlViewPortEx::SetROP(ROP_MODE_EX ropMode)
{
	ROP_MODE tmpRopMode;
	if (ropMode == ROP_MODE_EX::ROP_EX_COPY)
		tmpRopMode = ROP_COPY;
	if (ropMode == ROP_MODE_EX::ROP_EX_NORMAL)
		tmpRopMode = ROP_NORMAL;
	if (ropMode == ROP_MODE_EX::ROP_EX_XOR)
		tmpRopMode = ROP_XOR;

	bool bRet = (*m_ppGlViewPort)->SetROP(tmpRopMode);
	if (!bRet)
		return false;

	return true;
}

bool GlViewPortEx::GetDrawColor(Color% color)
{
	COLORREF dwDrawColor;
	bool bRet = (*m_ppGlViewPort)->GetDrawColor(dwDrawColor);
	if (bRet)
	{
		color = ColorTranslator::FromOle(dwDrawColor);
		return true;
	}

	return false;
}

Boolean GlViewPortEx::SetDrawColor(Color color)
{
	return (*m_ppGlViewPort)->SetDrawColor(ColorTranslator::ToOle(color));
}

Int32 GlViewPortEx::GetLineWidth()
{
	return (*m_ppGlViewPort)->GetLineWidth();
}

Boolean GlViewPortEx::SetLineWidth(Int32 iWidth)
{
	return (*m_ppGlViewPort)->SetLineWidth(iWidth);
}

Boolean GlViewPortEx::GetLineStipple(Boolean% bStipple, Int32% iRepeat, UInt16% iPattern)
{
	Boolean bTmpStipple;
	Int32 iTmpRepeat;
	UInt16 iTmpPattern;
	bool bRet = (*m_ppGlViewPort)->GetLineStipple(bTmpStipple, iTmpRepeat, iTmpPattern);
	if (bRet)
	{
		bStipple = bTmpStipple;
		iRepeat = iTmpRepeat;
		iPattern = iTmpPattern;
		return true;
	}

	return false;
}

Boolean GlViewPortEx::SetLineStipple(Boolean bStipple, Int32 iRepeat, UInt16 iPattern)
{
	return (*m_ppGlViewPort)->SetLineStipple(bStipple, iRepeat, iPattern);
}

void GlViewPortEx::DrawLineItem(LineItemEx^ lineItem)
{
	const Line2D* pLine2D = lineItem->GetLineItem().get();
	(*m_ppGlViewPort)->DrawLinePoint2D(pLine2D->GetStartPoint().X(), pLine2D->GetStartPoint().Y(), pLine2D->GetEndPoint().X(), pLine2D->GetEndPoint().Y());
}

void GlViewPortEx::DrawArcItem(ArcItemEx^ arcItem)
{
	const Arc2D* pArc2D = arcItem->GetArcItem().get();
	(*m_ppGlViewPort)->DrawArc2D(pArc2D->GetBasePoint().X(), pArc2D->GetBasePoint().Y(), pArc2D->GetRadius(), pArc2D->GetStartAngle(), pArc2D->GetSweepAngle());
}

void GlViewPortEx::DrawGeomItem(GeomItemEx^ geomItem)
{
    if (geomItem->GetType() == GEOMITEM_TYPE_EX::GEOMITEM_EX_LINE)
    {
        LineItemEx^ lineItem = (LineItemEx^)geomItem;
        DrawLineItem(lineItem);
    }
    else if (geomItem->GetType() == GEOMITEM_TYPE_EX::GEOMITEM_EX_ARC)
    {
        ArcItemEx^ arcItem = (ArcItemEx^)geomItem;
        DrawArcItem(arcItem);
    }
}

void GlViewPortEx::DrawWord(String^ strContent, double dLeftTopX, double dLeftTopY, double dRightBottomX, double dRightBottomY)
{
	std::wstring str = marshal_as<std::wstring>(strContent);
	(*m_ppGlViewPort)->DrawString(str, dLeftTopX, dLeftTopY, dRightBottomX, dRightBottomY);
}

void GlViewPortEx::DrawCoordinate(double dOrgX, double dOrgY, double dAngle, bool bInverse)
{
	(*m_ppGlViewPort)->DrawCoordinate(dOrgX, dOrgY, dAngle, bInverse);
}

void GlViewPortEx::DrawCross(double dX, double dY, double dSize, double dAngle)
{
    (*m_ppGlViewPort)->DrawCrossPoint2D(dX, dY, dSize, dAngle);
}

void GlViewPortEx::DrawPoint(double dX, double dY, double dSize)
{
	(*m_ppGlViewPort)->DrawPoint2D(dX, dY, dSize);
}

Point2DEx^ GlViewPortEx::GetCursorPos()
{
	double d1 = 0.0, d2 = 0.0;
	(*m_ppGlViewPort)->GetCursorPos(d1, d2);
	return gcnew Point2DEx(d1, d2);
}

bool GlViewPortEx::SwapBuffers()
{
	return (*m_ppGlViewPort)->SwapBuffers();
}

void GlViewPortEx::ReleaseResource()
{
	return (*m_ppGlViewPort)->ReleaseResource();
}

void GlViewPortEx::DrawPath(List<Tuple<double, double>^>^ ptList)
{
	std::vector<std::pair<double, double>> loop;
	loop.resize(ptList->Count);
	for (int i = 0; i < ptList->Count; i++)
	{
		Tuple<double, double>^ tuple = ptList[i];
		loop[i] = std::make_pair(tuple->Item1, tuple->Item2);
	}

	(*m_ppGlViewPort)->DrawLoop(loop);
}

END_CUTLEADER_NAMESPACE()
