#pragma once

#include "clGeometryFeatureCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternLoopNode)

BEGIN_CUTLEADER_NAMESPACE()

// 图形回路节点。
class ClGeometryFeature_Export PatternLoopNode : public ClData
{
public:
	PatternLoopNode(IPatternPtr pPattern, BOOL bLoopDirection);
	~PatternLoopNode(void);

public: // implement IData interface.
	// clone the loop node.
	// notes:
	//   1) in this function, we do not clone the pattern, so the new loop node will share pattern with the old one, so sometimes we should care about this outside this function.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	IPatternPtr GetPattern() const { return m_pPattern; }
	const IPattern* GetPattern_FR() const { return m_pPattern.get(); }
	void SetPattern(IPatternPtr pPattern) { m_pPattern = pPattern; }

	BOOL FollowLoopDirection() const { return m_bLoopDirection; }
	void FollowLoopDirection(BOOL bFollow) { m_bLoopDirection = bFollow; }

private:
	// the relative pattern.
	IPatternPtr m_pPattern;

	// notes: 
	//  1) the loop nodes of "PolygonLoop" form a direction(not mean CW or CCW), this variable indicates that whether the pattern has the same direction with this direction.
	//  2) for "SinglePatClosedPath" loop, the line/arcs of "SinglePatClosedPath" loop have the CCW direction, 
	//     "m_bLoopDirection" indicates that whether this direction is same with the cut direction.
	//  3) for "GridPatternLoop", similar with 2).
	BOOL m_bLoopDirection;
};

END_CUTLEADER_NAMESPACE()
