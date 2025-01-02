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
	void AddLines(Line2DListPtr pLineItems); // 加到后面。
	void InsertLines(Line2DListPtr pLineItems); // 插在最前面

	// 做变换。
	void Transform(const Matrix2D& mat);

	// 把直线集合反向，一般都是一系列首尾相连的线段。
	void Reverse();
};

END_CUTLEADER_NAMESPACE()