#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(CanvasZone)
DECLARE_CUTLEADER_CLASS(CanvasZoneList)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// ��ͼ���򼯺ϡ�
class CanvasZoneList : public std::vector<CanvasZonePtr>
{
public:
	CanvasZoneList(void);
	~CanvasZoneList(void);

public:
	// �õ�����������μ��ϡ�����С����ͼ���򡱡�
	/* ������
	*  polysRect: ����μ��ϵ���Ӿ��Ρ�
	*/
	// ע��
	//  1) ���û�оͷ��ؿա�
	//  2) �ҵ��ġ���ͼ���򡱲������ϸ������pPolys������Ϊ�ж��ϸ������Ҫ���Ѳ���ʱ�䣬������������Ӿ������жϰ�����
	//     ���ҵ��ġ���ͼ���򡱻���һ�����ƣ����Ǵӡ�����μ��ϡ���һ���㣬����㲻���ڡ���ͼ�������档���������õ���
	//     ����ͼ���򡱼�ʹ�����ϸ������Щ����Σ�Ҳ��������Щ����ζ��ڡ���ͼ�������档
	CanvasZonePtr GetContainer(const Polygon2DList* pPolys, const Rect2D& polysRect, const PatternLoopList* pAllPatternLoops) const;
};

END_CUTLEADER_NAMESPACE()
