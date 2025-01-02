#pragma once

#include "clPartLayoutCommon.h"
#include "Point2D.h"
#include "PatternPosition.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopInstance)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)
DECLARE_CUTLEADER_CLASS(PatternInstance)
DECLARE_CUTLEADER_CLASS(PatternInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ��ʵ������������
class ClPartLayout_Export PatternInstanceManager
{
public:
	// �ӻ�·ʵ���õ�ͼ��ʵ������Щͼ��ʵ����ѭ�и��ע������ֻ����Щ��ͼ��ʵ������������ѭ�и��ÿ����ͼ��ʵ��������ķ�����һ����ѭ�и��
	// notes: 
	//   1) sometimes the "start" pattern instance also be the last cut pattern instance. this function cannot cover this case.
	static PatternInstanceListPtr GetSortPatternInstanceList(LoopInstancePtr pLoopInstance, const LoopStartCutData* pLoopStartCutData);

	// �ӻ�·ʵ���õ�ͼ��ʵ����
	static PatternInstanceListPtr GetPatternInstanceList(LoopInstancePtr pLoopInstance);
	static PatternInstanceListPtr GetPatternInstanceList(LoopInstanceListPtr pLoopInstList);

	// �õ��㡰pt���ڡ�ͼ��ʵ����(pLoopInstance->iLoopNodeIndex)�ϵġ�PatternPosition����
	static PatternPosition GetPatternPosition(LoopInstancePtr pLoopInstance, int iLoopNodeIndex, const Point2D& pt);
};

END_CUTLEADER_NAMESPACE()
