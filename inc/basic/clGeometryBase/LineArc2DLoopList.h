#pragma once

#include "clGeometryBaseCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(LineArc2DLoop)
DECLARE_CUTLEADER_CLASS(LineArc2DLoopList)

BEGIN_CUTLEADER_NAMESPACE()

class ClGeometryBase_Export LineArc2DLoopList : public std::vector<LineArc2DLoopPtr>
{
public:
	LineArc2DLoopList(void);
	~LineArc2DLoopList(void);

public:
	// get the loops which intersect with "pLine".
	/* params:
	*  bShouldPierce: if TRUE, only when "pLine" pierce cross the loop, we will think "intersect".
	*/
	// notes:
	//   1) caller should manage "pCrossLoopList".
	//   2) if loop has already existed in "pCrossLoopList", do not add it.
	BOOL GetIntersectLoop(Line2DPtr pLine, LineArc2DLoopListPtr pCrossLoopList, BOOL bShouldPierce = TRUE);

	// get item by ID.
	LineArc2DLoopPtr GetGeomLoopByID(LONGLONG iLoopID);
};

END_CUTLEADER_NAMESPACE()
