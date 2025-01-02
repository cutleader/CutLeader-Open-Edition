#include "GeomItemListEx.h"

#include "LineArc2DList.h"
#include "LineItemEx.h"
#include "ArcItemEx.h"

BEGIN_CUTLEADER_NAMESPACE()

GeomItemListEx::GeomItemListEx(void)
{
	m_ppGeomItemList = new LineArc2DListPtr();
	m_ppGeomItemList->reset(new LineArc2DList());
}

GeomItemListEx::GeomItemListEx(LineArc2DListPtr* ppGeomItemList)
{
	m_ppGeomItemList = new LineArc2DListPtr;
	*m_ppGeomItemList = *ppGeomItemList;
}

GeomItemListEx::~GeomItemListEx()
{
	this->!GeomItemListEx();
}

GeomItemListEx::!GeomItemListEx()
{
	if (m_ppGeomItemList)
		delete m_ppGeomItemList;
}

GeomItemListEx^ GeomItemListEx::Clone()
{
	LineArc2DListPtr pNewGeomItems = (*m_ppGeomItemList)->Clone();
	GeomItemListEx^ newGeomItemsEx = gcnew GeomItemListEx(&pNewGeomItems);
	return newGeomItemsEx;
}

void GeomItemListEx::Transform(Matrix2DEx^ mat)
{
	(*m_ppGeomItemList)->Transform(*(mat->GetMatrix()));
}

Rect2DEx^ GeomItemListEx::GetRectBox()
{
	Rect2D rect = (*m_ppGeomItemList)->GetRectBox();
	return gcnew Rect2DEx(rect);
}

int GeomItemListEx::Count()
{
	return (*m_ppGeomItemList)->size();
}

GeomItemEx^ GeomItemListEx::GetItemByIndex(int iIndex)
{
	GeomItemEx^ geomItem;

	ILineArc2DPtr pGeomItem = (*m_ppGeomItemList)->at(iIndex);
	if (pGeomItem->GetType() == LineArc2D_Line)
	{
		Line2DPtr pLineItem = boost::dynamic_pointer_cast<Line2D>(pGeomItem);
		geomItem = gcnew LineItemEx(&pLineItem);
	}
	else if (pGeomItem->GetType() == LineArc2D_Arc)
	{
		Arc2DPtr pArcItem = boost::dynamic_pointer_cast<Arc2D>(pGeomItem);
		geomItem = gcnew ArcItemEx(&pArcItem);
	}

	return geomItem;
}

void GeomItemListEx::AddItem(GeomItemEx^ geomItem)
{
	if (geomItem->GetType() == GEOMITEM_TYPE_EX::GEOMITEM_EX_LINE)
	{
		LineItemEx^ lineItem = (LineItemEx^)geomItem;
		(*m_ppGeomItemList)->push_back(lineItem->GetLineItem());
	}
	else if (geomItem->GetType() == GEOMITEM_TYPE_EX::GEOMITEM_EX_ARC)
	{
		ArcItemEx^ arcItem = (ArcItemEx^)geomItem;
		(*m_ppGeomItemList)->push_back(arcItem->GetArcItem());
	}
}

void GeomItemListEx::AddItemList(GeomItemListEx^ geomItemList)
{
	LineArc2DListPtr pGeomItemList = geomItemList->GetGeomItemList();
	(*m_ppGeomItemList)->insert((*m_ppGeomItemList)->end(), pGeomItemList->begin(), pGeomItemList->end());
}

void GeomItemListEx::RemoveItem(int iIndex)
{
	(*m_ppGeomItemList)->erase((*m_ppGeomItemList)->begin()+iIndex);
}

void GeomItemListEx::Clear()
{
	(*m_ppGeomItemList)->clear();
}

END_CUTLEADER_NAMESPACE()
