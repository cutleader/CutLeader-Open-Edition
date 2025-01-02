#include "StdAfx.h"
#include "CanvasZoneList.h"

#include "Polygon2D.h"
#include "Polygon2DList.h"
#include "CanvasZone.h"
#include "IPatternLoop.h"
#include "PatternLoopCache.h"
#include "PatternLoopList.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

CanvasZoneList::CanvasZoneList(void)
{
}

CanvasZoneList::~CanvasZoneList(void)
{
}

CanvasZonePtr CanvasZoneList::GetContainer(const Polygon2DList* pPolys, const Rect2D& polysRect, const PatternLoopList* pAllPatternLoops) const
{
	CanvasZonePtr pRetCanvasZone;

	if (pPolys->size() == 0)
		return pRetCanvasZone;

	// �ȵõ�һЩ����ͼ���򡱣����ǵ���Ӿ��ΰ����˶���ε���Ӿ��Ρ�
	CanvasZoneListPtr pCanvasZoneList1(new CanvasZoneList);
	for (unsigned int i = 0; i < size(); i++)
	{
		CanvasZonePtr pCanvasZone = this->operator[](i);
		if (pCanvasZone->GetPatLoopID4OuterPoly() == INVALID_IDX) // ���������ĵ�ͼ����
			pCanvasZoneList1->push_back(pCanvasZone);
		else
		{
			const IPatternLoop* pPatternLoop = pAllPatternLoops->GetLoopByID(pCanvasZone->GetPatLoopID4OuterPoly()).get();
			if (pPatternLoop->GetCacheData()->GetLoopRect().IncludeRect(polysRect))
				pCanvasZoneList1->push_back(pCanvasZone);
		}
	}

	CanvasZoneListPtr pCanvasZoneList2(new CanvasZoneList);
	if (pCanvasZoneList1->size() > 0)
	{
		for (unsigned int i = 0; i < pCanvasZoneList1->size(); i++)
		{
			CanvasZonePtr pCanvasZone = pCanvasZoneList1->operator[](i);
			const Point2D& testPt = pPolys->at(0)->at(0);
			if (pCanvasZone->GetOuterPoly()->GetPointPos(testPt) != PT_POLYGON_POS::INSIDE_POLY)
				continue;
			else
				pCanvasZoneList2->push_back(pCanvasZone);
		}
	}

	// ��pCanvasZoneList2�õ�һ�����ڲ�ġ���ͼ���򡱡�
	if (pCanvasZoneList2->size() > 0)
	{
		for (unsigned int i = 0; i < pCanvasZoneList2->size(); i++)
		{
			CanvasZonePtr pCanvasZone = pCanvasZoneList2->operator[](i);
			if (i == 0)
				pRetCanvasZone = pCanvasZone;
			else
			{
				if (pRetCanvasZone->GetPatLoopID4OuterPoly() == INVALID_IDX) // pRetCanvasZone�������ĵ�ͼ����
					pRetCanvasZone = pCanvasZone;
				else if (pCanvasZone->GetPatLoopID4OuterPoly() == INVALID_IDX) // pCanvasZone�������ĵ�ͼ����
					continue;
				else
				{
					const IPatternLoop* pPatternLoop_1 = pAllPatternLoops->GetLoopByID(pRetCanvasZone->GetPatLoopID4OuterPoly()).get();
					const IPatternLoop* pPatternLoop_2 = pAllPatternLoops->GetLoopByID(pCanvasZone->GetPatLoopID4OuterPoly()).get();
					if (pPatternLoop_1->GetCacheData()->GetLoopRect().IncludeRect(pPatternLoop_2->GetCacheData()->GetLoopRect()))
						pRetCanvasZone = pCanvasZone;
				}
			}
		}
	}

	return pRetCanvasZone;
}

END_CUTLEADER_NAMESPACE()
