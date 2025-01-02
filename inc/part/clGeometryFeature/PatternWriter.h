#pragma once

#include "clGeometryFeatureCommon.h"
#include "IPatternVisitor.h"

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IPattern)
DECLARE_CUTLEADER_CLASS(PatternWriter)

BEGIN_CUTLEADER_NAMESPACE()

// this class writes the pattern into database.
// notes:
//   1) do not commit transaction in this class.
class ClGeometryFeature_Export PatternWriter : public IPatternVisitor
{
public:
	PatternWriter(DBConnectPtr pDBConnect);
	~PatternWriter(void);

public:
	virtual BOOL Visit(LinePatternPtr pLinePat);
	virtual BOOL Visit(ArcPatternPtr pArcPat);
    virtual BOOL Visit(PolygonPatternPtr pPolygonPat);
	virtual BOOL Visit(EllipsePatternPtr pEllipsePat);
	virtual BOOL Visit(CirclePatternPtr pCirclePat);
	virtual BOOL Visit(RectHolePatternPtr pRectHolePat);
	virtual BOOL Visit(RoundCornerHolePatternPtr pRCHolePat);
	virtual BOOL Visit(SingleDPatternPtr pSingleDPat);
	virtual BOOL Visit(DoubleDPatternPtr pDoubleDPat);
	virtual BOOL Visit(LineArrayPatternPtr pPatLinePat);
	virtual BOOL Visit(ArcArrayPatternPtr pPatArcPat);
	virtual BOOL Visit(CircleArrayPatternPtr pPatCirclePat);
	virtual BOOL Visit(GridArrayPatternPtr pPatGridPat);
	virtual BOOL Visit(TriangleHolePatternPtr pTrHolePat);
	virtual BOOL Visit(OBroundPatternPtr pOBroundPat);
	virtual BOOL Visit(RadSlotPatternPtr pRadSlotPat);

private:
	// save the relations of this pattern.
	void SavePatternRelation(IPatternPtr pPattern);

private:
	DBConnectPtr m_pDBConnect;
};

END_CUTLEADER_NAMESPACE()
