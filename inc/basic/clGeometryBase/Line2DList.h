#pragma once

#include "clGeometryBaseCommon.h"
#include <vector>
#include "Matrix2D.h"

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(Line2DList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryBase_Export Line2DList : public std::vector<Line2DPtr>
{
public:
	Line2DList(void);
	~Line2DList(void);

public:
	void AddLines(Line2DListPtr pLineItems); // �ӵ����档
	void InsertLines(Line2DListPtr pLineItems); // ������ǰ��

	// ���任��
	void Transform(const Matrix2D& mat);

	// ��ֱ�߼��Ϸ���һ�㶼��һϵ����β�������߶Ρ�
	void Reverse();
};

END_CUTLEADER_NAMESPACE()