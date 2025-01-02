#pragma once

#include "clPartLayoutCommon.h"
#include "Rect2D.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(RemnantRegion)
DECLARE_CUTLEADER_CLASS(Polygon2DList)
DECLARE_CUTLEADER_CLASS(RemnantRegionList)

BEGIN_CUTLEADER_NAMESPACE()

// �������򼯺ϡ�
class ClPartLayout_Export RemnantRegionList : public std::vector<RemnantRegionPtr>
{
public:
	RemnantRegionList(void);
	~RemnantRegionList(void);

public:
	// �õ�����������μ��ϡ�����С���������򡱡�
	/* ������
	*  polysRect: ����μ��ϵ���Ӿ��Ρ�
	*/
	// ע��
	//  1) ���û�оͷ��ؿա�
	//  2) �ҵ��ġ��������򡱲������ϸ������pPolys������Ϊ�ж��ϸ������Ҫ���Ѳ���ʱ�䣬������������Ӿ������жϰ�����
	//     ���ҵ��ġ��������򡱻���һ�����ƣ����Ǵӡ�����μ��ϡ���һ���㣬����㲻���ڡ������������档���������õ���
	//     ���������򡱼�ʹ�����ϸ������Щ����Σ�Ҳ��������Щ����ζ��ڡ������������档
	RemnantRegionPtr GetContainer(const Polygon2DList* pPolys, const Rect2D& polysRect) const;
};

END_CUTLEADER_NAMESPACE()
