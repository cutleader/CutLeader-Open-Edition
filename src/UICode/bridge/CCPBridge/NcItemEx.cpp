#include "stdafx.h"
#include "NcItemEx.h"

#include "LineItemEx.h"
#include "ArcItemEx.h"

BEGIN_CUTLEADER_NAMESPACE()

NcItemEx::NcItemEx(void)
{
	m_ppNcItem = new NcItemPtr();
	m_ppNcItem->reset(new NcItem());
}

NcItemEx::NcItemEx(NcItemPtr* ppNcItem)
{
	m_ppNcItem = new NcItemPtr;
	*m_ppNcItem = *ppNcItem;
}

NcItemEx::~NcItemEx()
{
	this->!NcItemEx();
}

NcItemEx::!NcItemEx()
{
	if (m_ppNcItem)
		delete m_ppNcItem;
}

NcItemTypeEx NcItemEx::GetNcItemType()
{
	NcItemTypeEx emNcItemTypeEx;

	NcItemType emNcItemType = (*m_ppNcItem)->GetNcItemType();	
	if (emNcItemType == NcItem_CutLine)
		emNcItemTypeEx = NcItemTypeEx::NcItem_CutLine_Ex;
	else if (emNcItemType == NcItem_TraceLine)
		emNcItemTypeEx = NcItemTypeEx::NcItem_TraceLine_Ex;
	else if (emNcItemType == NcItem_CutSegment_OrderNum)
		emNcItemTypeEx = NcItemTypeEx::NcItem_CutSegment_OrderNum_Ex;

	return emNcItemTypeEx;
}

void NcItemEx::SetNcItemType(NcItemTypeEx emNcItemTypeEx)
{
	if (emNcItemTypeEx == NcItemTypeEx::NcItem_CutLine_Ex)
		(*m_ppNcItem)->SetNcItemType(NcItem_CutLine);
	else if (emNcItemTypeEx == NcItemTypeEx::NcItem_TraceLine_Ex)
		(*m_ppNcItem)->SetNcItemType(NcItem_TraceLine);
	else if (emNcItemTypeEx == NcItemTypeEx::NcItem_CutSegment_OrderNum_Ex)
		(*m_ppNcItem)->SetNcItemType(NcItem_CutSegment_OrderNum);
}

GeomItemEx^ NcItemEx::GetLineArc2D()
{
	GeomItemEx^ geomItem;

	ILineArc2DPtr pLineArc2D = (*m_ppNcItem)->GetLineArc2D();
	if (pLineArc2D->GetType() == LineArc2D_Line)
	{
		Line2DPtr pLine2D = boost::dynamic_pointer_cast<Line2D>(pLineArc2D);
		geomItem = gcnew LineItemEx(&pLine2D);
	}
	else if (pLineArc2D->GetType() == LineArc2D_Arc)
	{
		Arc2DPtr pArc2D = boost::dynamic_pointer_cast<Arc2D>(pLineArc2D);
		geomItem = gcnew ArcItemEx(&pArc2D);
	}

	return geomItem;
}

void NcItemEx::SetLineArc2D(GeomItemEx^ geomItem)
{
	if (geomItem->GetType() == GEOMITEM_TYPE_EX::GEOMITEM_EX_LINE)
	{
		LineItemEx^ lineItem = (LineItemEx^)geomItem;
		(*m_ppNcItem)->SetLineArc2D(lineItem->GetLineItem());
	}
	else if (geomItem->GetType() == GEOMITEM_TYPE_EX::GEOMITEM_EX_ARC)
	{
		ArcItemEx^ arcItem = (ArcItemEx^)geomItem;
		(*m_ppNcItem)->SetLineArc2D(arcItem->GetArcItem());
	}
}

Point2DEx^ NcItemEx::GetCurrentCutPt()
{
	const Point2D& tmpPt = (*m_ppNcItem)->GetCurrentCutPt();
	return gcnew Point2DEx(tmpPt.X(), tmpPt.Y());
}

void NcItemEx::SetCurrentCutPt(Point2DEx^ currentCutPt)
{
	Point2D tmpPt(currentCutPt->X(), currentCutPt->Y());
	(*m_ppNcItem)->SetCurrentCutPt(tmpPt);
}

int NcItemEx::GetOrderNum()
{
	return (*m_ppNcItem)->GetOrderNum();
}

void NcItemEx::SetOrderNum(int iOrderNum)
{
	(*m_ppNcItem)->SetOrderNum(iOrderNum);
}

END_CUTLEADER_NAMESPACE()
