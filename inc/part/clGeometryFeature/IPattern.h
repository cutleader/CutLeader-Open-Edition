#pragma once

#include "clGeometryFeatureCommon.h"
#include "IData.h"
#include "Point2D.h"
#include "Point2DList.h"
#include "PatternPosition.h"
#include "PatternRelation.h"
#include "ISubject.h"

// declare classes.
DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(PatternRelation)
DECLARE_CUTLEADER_CLASS(PatternRelationList)
DECLARE_CUTLEADER_CLASS(PatternCache)
DECLARE_CUTLEADER_CLASS(IPatternVisitor)
//

BEGIN_CUTLEADER_NAMESPACE()

// 几何特征类型。
enum PatternType
{
	// line
	PATTERN_LINE			= 200,

	// arc
	PATTERN_ARC 			= 203,

    // polygon
    PATTERN_POLYGON         = 310,

	// hole
	PATTERN_RECT_HOLE 		= 202,
	PATTERN_TR_HOLE 		= 208,

	// round hole
	PATTERN_ROUNDHOLE		= 204,
	PATTERN_OBROUND			= 209,
	PATTERN_ELLIPSE			= 210,

	// D shape
	PATTERN_SINGLED			= 205,
	PATTERN_DOUBLED			= 211,

	// Round Corner
	PATTERN_RCHOLE			= 206,

	// radius slot
	PATTERN_RADSLOT			= 300,

	// grid pattern
	PATTERN_PATTERNLINE		= 207,
	PATTERN_PATTERNARC		= 301,
	PATTERN_PATTERNCIRCLE	= 302,
	PATTERN_PATTERNGRID		= 303,
};

// we defined three geometry category，定义了3中几何特征类别。
enum PatternCategory
{
	GEOMETRY_STANDARD		= 0,
	GEOMETRY_POLYGON		= 1,
	GEOMETRY_ARRAY			= 2,
};

// the interface for geometry feature, 几何特征接口。
class ClGeometryFeature_Export IPattern : virtual public IData, virtual public ISubject
{
public:
	IPattern();
	virtual ~IPattern() {}

public:
	/************************************************************************/
	// the basic info of pattern.

	// the pattern type
	virtual PatternType GetPatternType() const = 0;

	// the basic point of the pattern.
	virtual Point2D GetBasePoint() const = 0;
	virtual void SetBasePoint(const Point2D& pt) = 0;

	// the basic vector of the pattern.
	virtual Vector2D GetBaseVector() const = 0;
	virtual void SetBaseVector(const Vector2D& vect) = 0;

	// whether this pattern is the standard pattern.
	virtual BOOL IsStdPattern() const = 0;

	// whether this pattern is "array" pattern.
	virtual BOOL IsArrayPattern() const = 0;

	// whether the pattern is smooth.
	// notes:
	//   1) line/arc, circle, ellipse,.. pattern are smooth.
	//   2) triangle, rect... pattern are not smooth.
	virtual BOOL IsSmooth() const = 0;
	/************************************************************************/


	/************************************************************************/
	// about the point.

	// about start/end point.
	//   1) used for some "open" pattern, e.g., line,arc. 
	//   2) used for some "closed" pattern, e.g. RectHolePattern.
	virtual Point2D GetStartPoint() const = 0;
	virtual void SetStartPoint(const Point2D& pt) = 0;
	virtual Point2D GetEndPoint() const = 0;
	virtual void SetEndPoint(const Point2D& pt) = 0;

	// get snap points of the pattern.
	// notes: 
	//   1) get the control points of the pattern, the better name is "GetControlPoints".
	virtual void GetSnapPoints(Point2DList& snapPts) const = 0;

	// get the control point that is closest to point
	virtual Point2D GetSnapPtClosestTo(const Point2D& pt) const = 0;

	// get the control point:
	//  1) closest to the pt.
	//  2) within the capture range.
	virtual Point2DPtr GetSnapPtClosestTo(const Point2D& pt, double dCaptureRange) const = 0;

	// get the point using PatternPosition
	virtual Point2D GetPointByPatPos(const PatternPosition& patternPosition) const = 0;

	// get the PatternPosition of the point on the pattern.
	// notes:
	//   1) if "pt" is the cross point of two geoms, "secondPatPos" will be set.
	virtual PatternPosition GetPatPosByPoint(const Point2D& pt, PatternPosition& secondPatPos) const = 0;

	// get the point on the pattern, this point should be the closest point to "pt".
	// notes: 
	//   1) if caller set "mat", we transform the pattern using "mat", then calculate the closest point.
	virtual Point2D GetClosestPt(const Point2D& pt, const Matrix2D& mat) const = 0;

	// calculate the distance between the pattern and the point.
	virtual double DistanceTo(const Point2D& pt) const = 0;
	/************************************************************************/


	/************************************************************************/
	// related to Pattern relations.

	// get the relation list of this pattern.
	virtual PatternRelationListPtr GetPatternRelationList() const = 0;

	// add the relation to this pattern.
	virtual void AddPatternRelation(PatternRelationPtr pPatternRelation) = 0;
	/************************************************************************/


	/************************************************************************/
	// about control pattern.

	// whether this pattern is a control pattern.
	virtual bool IsControlPat() const = 0;

	// "pPattern" will be grid pattern.
	// notes:
	//   1) if "pPattern" is NULL, reset this pattern as not control pattern. BUT user should make sure whether he will maintain control pattern info in grid pattern.
	virtual void SetBeControlPat(IPatternPtr pPattern) = 0;
	/************************************************************************/


	/************************************************************************/
	// with other patterns.

	// use pPattern to update the pattern
	// notes:
	//   1) do not change the ID of the current pattern.
	virtual void UpdateFrom(IPatternPtr pPattern) = 0;

	// whether this pattern is duplicated with pPattern.
	// notes: 
	//   1) currently we do not implement this function for all pattern type.
	//   2) "duplicate" do not mean "absolutely same", so two "duplicate" lines may have different direction.
	virtual BOOL IsDuplicate(IPatternPtr pPattern) const = 0;

	// the difference with "IsDuplicate()" is that the origin point can be different.
	virtual BOOL IsSameShape(IPatternPtr pPattern) const = 0;
	/************************************************************************/


	virtual LineArc2DListPtr Calculate2DLineArcs() const = 0;
	virtual void Accept(IPatternVisitorPtr patVisitor) = 0;
	virtual void Transform(const Matrix2D& mat) = 0;

	// 从dxf导入图形时，记录图层和颜色信息，这些信息只在导入后做工艺分配时使用。
	virtual CString GetLayerName() const = 0;
	virtual void SetLayerName(CString strLayerName) = 0;
	virtual int GetColorIndex() const = 0;
	virtual void SetColorIndex(int iColorIndex) = 0;

public:
	// 有关缓存。
	PatternCachePtr GetCacheData() const { return m_pPatternCache; }
	const PatternCache* GetCacheData_FR() const { return m_pPatternCache.get(); }
	void UpdateCache();

protected:
	// the cache data of the pattern.
	// 注：
	// 1) 该变量放在接口中，这样可以不用虚函数来访问它，效率方面有优势。
	PatternCachePtr m_pPatternCache;
};

END_CUTLEADER_NAMESPACE()
