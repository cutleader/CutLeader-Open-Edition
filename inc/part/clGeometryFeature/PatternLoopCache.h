#pragma once

#include "clGeometryFeatureCommon.h"
#include "Rect2D.h"

BEGIN_CUTLEADER_NAMESPACE()

// the cache data for pattern loop object.
class ClGeometryFeature_Export PatternLoopCache
{
public:
	PatternLoopCache(void);
	~PatternLoopCache(void);

public: // get/set functions.
	void SetLoopRect(const Rect2D& loopRect) { m_loopRect = loopRect; }
	const Rect2D& GetLoopRect() const { return m_loopRect; }

	void SetLoopBaseGeom(LineArc2DListPtr pGeom2DList) { m_pGeom2DList = pGeom2DList; }
	LineArc2DListPtr GetLoopBaseGeom() const { return m_pGeom2DList; }

private:
	// the rect of the loop.
	Rect2D m_loopRect;

	// the base geom of pattern loop.
	LineArc2DListPtr m_pGeom2DList;
};

END_CUTLEADER_NAMESPACE()
