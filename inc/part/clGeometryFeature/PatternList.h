#pragma once

#include "ClData.h"
#include "IPattern.h"
#include "Rect2D.h"
#include "SubjectBase.h"

DECLARE_CUTLEADER_CLASS(IntegerList)
DECLARE_CUTLEADER_CLASS(PatternList)

BEGIN_CUTLEADER_NAMESPACE()

// pattern list.
class ClGeometryFeature_Export PatternList : public ClData, public std::vector<IPatternPtr>, public SubjectBase
{
public:
	PatternList(void);
	virtual ~PatternList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override;

	// clone the pattern list.
	// notes:
	//   1) when cloning a pattern, we should check all "Grid" patterns.
	//   2) do not care the cache data.
	virtual IDataPtr Clone() const override;

public:
	/************************************************************************/
	// get patterns simply.

	// get pattern by ID.
	IPatternPtr GetPatternFromID(LONGLONG iPatternID) const;

	// get index of the pattern.
	int GetPatternIndex(const IPattern* pPattern) const;

	// 从图形ID得到序号。
	int GetIndexByID(LONGLONG iPatternID) const;

	// get the patterns by the pattern category.
	PatternListPtr GetPatternByCategory(PatternCategory category, BOOL bIgnoreBuried = TRUE) const;

	// get the patterns by pattern type.
	PatternListPtr GetPatternByType(PatternType emPatternType, BOOL bIgnoreBuried = TRUE) const;

	// get all control patterns.
	PatternListPtr GetAllCtrlPatterns() const;

	// get the pattern types within pattern list.
	IntegerListPtr GetPatternTypeList(BOOL bIgnoreBuried = TRUE) const;
	/************************************************************************/


	/************************************************************************/
	// get patterns which fit some condition.

	// get the pattern that is nearest to the point.
	// notes:
	//   1) if there are huge count patterns, use function PatternLoopList::GetClosestPat().
	IPatternPtr GetPatternClosestTo(const Point2D& pt, BOOL bIgnoreBuried = TRUE) const;

	// get patterns in "rect".
	PatternListPtr GetPatternInRect(const Rect2D& rect, BOOL bIgnoreBuried = TRUE) const;

	// get "closed" and "not grid" patterns.
	PatternListPtr GetClosedPatternList(BOOL bIgnoreBuried = TRUE) const;
	/************************************************************************/


	/************************************************************************/
	// add/remove/insert item from the pattern list.

	void AddPatterns(PatternListPtr pPatterns);

	// 删除图形。
	void RemovePattern(const IPattern* pPattern);
	void RemovePattern(LONGLONG iPatternID);
	void RemovePatterns(const PatternList* pPatterns);
	void RemovePatternsEx(const IntegerList* pPatterns);

	// insert "pPattern" before "iIndex".
	void InsertBefore(IPatternPtr pPattern, int iIndex);
	/************************************************************************/


	// get the proper control point.
	// notes:
	//   1) the control point will fit:
	//      a) closest to "pt".
	//      b) within the capture range(dCaptureRange) to "pt".
	//   2) we may not find the point, return NULL.
	Point2DPtr GetCtrlPtClosestTo(const Point2D& pt, double dCaptureRange) const;

	// get the rect box of all patterns.
	// notes:
	//   1) if huge number patterns exist, PatternLoopList::GetRectBox() is more fast because using cache data.
	//   2) this function will use cache data of pattern.
	Rect2D GetRectBox() const;

	// 变换几何特征。
	/* params:
	*  bSkipCtrlPat: whether transform the control pattern.
	*/
	void Transform(const Matrix2D& mat, BOOL bSkipCtrlPat = TRUE);
	void TransformAndUpdateCache(const Matrix2D& transformMatrix); // 矩阵变换几何特征并更新其缓存。
	void TransferAndUpdateCache(const Vector2D& transferVect); // 平移几何特征并更新其缓存。

	// accept the pattern visitors.
	void Accept(IPatternVisitorPtr patVisitor);

	// calculate the 2d line/arcs of the patterns.
	// notes:
	//   1) if huge number patterns exist, PatternLoopList::Calculate2DLineArcs() is more fast because using cache data.
	//   2) this function will use the cache of pattern.
	//   3) this function do not care control pattern.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// update the cache data of the pattern object.
	void UpdateCache();
	void UpdateCache4Moving(const Vector2D& transferVect); // 移动几何特征后可调用该函数更新缓存，比重新生成缓存要高效不少。
};

END_CUTLEADER_NAMESPACE()
