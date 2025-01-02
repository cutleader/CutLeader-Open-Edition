#pragma once

#include "clGeometryBaseCommon.h"


DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(LineArc2DLoop)
DECLARE_CUTLEADER_CLASS(Point2DList)
DECLARE_CUTLEADER_CLASS(Line2D)
DECLARE_CUTLEADER_CLASS(ILineArc2D)


BEGIN_CUTLEADER_NAMESPACE()


// 由“二维直线和圆弧”连成的轮廓。
// 注：
//  1) 回路中的图形首尾相连
class ClGeometryBase_Export LineArc2DLoop
{
public:
	LineArc2DLoop(void);
	~LineArc2DLoop(void);

public: // get/set functions.
	LineArc2DListPtr GetGeom2DList() const { return m_pGeo2DList; }
	LONGLONG GetID() const { return m_iID; }

public: // other functions.
	// whether current loop is adjacent with "pGeomLoop".
	/* params:
	*  dSpace: use this to make sure whether two loops are adjacent.
	*/
	// notes:
	//   1) we think the following two loops are adjacent.
	//   2) now we only check the lines within the loops, if lines of different loops are "common edge", we think these loops are adjacent.
	/*
			+-----------++-----------+ 
			|           ||           | 
			|           ||           | 
			|           ||           | 
			+-----------++-----------+ 
	*/
	BOOL AdjacentWith(const LineArc2DLoop* pGeomLoop, double dSpace) const;

	// get the points of the loop.
	/*
				 (pt4)	3th geom   (pt3)                         
					  +-------------+    
					  |             |    
					  |             |    
		    4th geom  |				|   2th geom 
					  |             |    
					  |             |    
					  +-------------+    
				 (pt1)    1th geom   (pt2)
	*/
	// notes: 
	//   1) these points indicated the track of this loop. 
	//   2) we should filter out the repeat points.
	//   3) To be implemented: if the loop is composed of a line and an arc, current algorithm cannot cover this case.
	Point2DListPtr GetPointList() const;

	// whether this loop intersect with "pLine".
	/* params:
	*  bShouldPierce: if TRUE, only when "pLine" pierce cross the loop, we will think "intersect", refer to the illustration, figure 1 is NOT "intersect".
	*  bContainLine: TRUE, figure 2 is not "intersect", FALSE, it's "intersect".
	*/
	/* 
				 +-------------+          
				 |             |      
				 |             ****** 
				 |             |       
				 +-------------+      
					figure 1     
			                          
				 +-------------+           
				 |             |      
				 |        ******          
				 |             |      
				 +-------------+      
				    figure 2
	*/
	BOOL IntersectWithLine(Line2DPtr pLine, BOOL bShouldPierce = TRUE, BOOL bContainLine = TRUE);

	// about the direction of the loop.
	DIRECTION GetDirection() const;
	void ReverseDirection();

	// 是不是矩形。目前必须是四个直线组成的矩形，斜着放的也行。
	bool LoopIsRect() const;

	// 是不是圆。目前必须是一个arc2D的圆。
	bool LoopIsRound() const;

private:
	// whether "pLine" is adjacent with this loop.
	/* params:
	*  dSpace: use this to make sure whether two loops are adjacent.
	*/
	BOOL AdjacentWith(const Line2D* pLine, double dSpace = 0) const;

	// 拆分回路的各个节点，生成一个新的回路。
	LineArc2DLoopPtr SplitAllNodes() const;

	// 拆分回路的弧度大于90度的圆弧节点为4等分节点，生成一个新的回路。
	LineArc2DLoopPtr SplitArcNodes() const;

    std::pair<ILineArc2DPtr, ILineArc2DPtr> GetTwo2DLineArcsAtConnectionPt(int iConnectPtIdx) const;

    // 拆分“左点”的2个相邻图形中的一个，参考PolygonPatternLoop中同名函数。
    LineArc2DLoopPtr SplitOneArcGeom_of_twoAdjacentGeoms(const ILineArc2D* p2DLineArc_willBeSplit) const;

private:
	// the basic geometries in the geometry loop.
	LineArc2DListPtr m_pGeo2DList;

	// 回路ID。
	LONGLONG m_iID;
};

END_CUTLEADER_NAMESPACE()
