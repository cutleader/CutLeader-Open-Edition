#pragma once

#include "baseConst.h"
#include "IPatternLoop.h"
#include "SubjectBase.h"
#include "ClData.h"
#include <vector>
#include <map>

DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)

BEGIN_CUTLEADER_NAMESPACE()

// this class stands for all pattern loops.
// notes:
//  1) after call "AddPattern","RemovePattern","ChangePattern", we may create new loop or delete old loop, once delete loop, the CAM feature on this loop will be cleared.
//  2) the deleted loop cannot be "undo".
class ClGeometryFeature_Export PatternLoopList : public std::vector<IPatternLoopPtr>, public ClData, public SubjectBase
{
public:
	PatternLoopList(void);
	virtual ~PatternLoopList(void);

public: // implement IData interface.
	virtual BOOL IsModified() const override;

	// clone the pattern loops.
	// notes:
	//   1) clone each loop.
	//   2) do not clone the pattern within the loop node.
	virtual IDataPtr Clone() const override;

public:
	/************************************************************************/
	// get the loop by ID.

	// get the loop by the loop id.
	IPatternLoopPtr GetLoopByID(LONGLONG iLoopID) const;

	// get the index of "iLoopID".
	int GetPatternLoopIndex(LONGLONG iLoopID) const;
	/************************************************************************/


	/************************************************************************/
	// get the loop using pattern.

	// find the pattern loop using the pattern.
	IPatternLoopPtr FindLoopByPat(const IPattern* pPattern) const;

	// get the loops which is built with some patterns in "pPatList".
	PatternLoopListPtr GetLoopWithinPats(const PatternList* pPatList) const;

	// get the open loops which connected with "pPattern".
	//   1) by default we use SYSTEM_TOLERANCE as the criteria to check whether geometries are connected.
	//   2) returned loops will fit:
	//      a) is open loop.
	//      b) start or end point connects with the start or end point of pattern.
	PatternLoopListPtr GetOpenLoopLinkToPat(const IPattern* pPattern, double dTol = SYSTEM_TOLERANCE) const;
	/************************************************************************/


	/************************************************************************/
	// manipulate the loop.

	// delete the open pattern loops.
	void DeleteOpenLoops();

	// delete the loop.
	void DeleteLoop(LONGLONG iLoopID);
	void DeleteLoop(const IPatternLoop* pLoop);
	void DeleteLoopEx(const IPatternLoop* pLoop);
	void DeleteLoopList(const PatternLoopList* pLoopList);

	// remove the tiny loops.
	// notes:
	//   1) the function will check the width and height of the loop rect.
	//   2) the function will use the cache data of the loop.
	void RemoveTinyLoops(PatternListPtr pPatList, double dVal);
	void RemoveTinyLoops(double dVal);
	/************************************************************************/


	// get the loops by loop type.
	PatternLoopListPtr GetPatternLoopByType(PatternLoopType loopType) const;

	// get the pattern which is closest to "pt".
	// notes:
	//   1) if there are huge count patterns, use this function.
	//   2) this function also return the distance.
	IPatternPtr GetClosestPat(const Point2D& pt, double& dDistance) const;

	// get the rect box of all loops.
	// notes:
	//   1) this function will use the cache data, so run more fast.
	//   2) 当没有几何轮廓时，返回尺寸为0的矩形。
	Rect2D GetRectBox() const;

	// calc the base geoms of all loops.
	// notes:
	//   1) this function will use the cache data, so run more fast.
	LineArc2DListPtr Calculate2DLineArcs() const;

	// 得到从原点到图形左下点的向量。
	Vector2D GetVectToOrigin() const;

	// 得到所有回路中的Pattern。
	PatternListPtr GetPatList() const;
	PatternListPtr GetPatList(std::map<LONGLONG, IPatternLoopPtr>& pLoopsOfPatterns) const; // 该函数还返回每个几何特征对应的轮廓。

    // 得到在矩形区域内的轮廓。需要使用轮廓缓存。
    PatternLoopListPtr GetPatternLoopsInRect(const Rect2D& rect) const;
};

END_CUTLEADER_NAMESPACE()
