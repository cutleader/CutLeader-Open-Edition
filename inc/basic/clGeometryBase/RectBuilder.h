#pragma once

#include "clGeometryBaseCommon.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(Point2DList)

BEGIN_CUTLEADER_NAMESPACE()

// this class will build Rect2D object.
class ClGeometryBase_Export RectBuilder
{
public:
	RectBuilder(void);
	~RectBuilder(void);

public:
	// build the rect from the point list.
	// notes:
	//   1) even one point exists, we can build a rect.
	Rect2D BuildRectFromPt(Point2DListPtr pPtList);
};

END_CUTLEADER_NAMESPACE()
