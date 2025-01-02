#pragma once

#include "clCutSequenceCommon.h"
#include "Point2D.h"
#include "IData.h"

DECLARE_CUTLEADER_CLASS(Point2DList)

BEGIN_CUTLEADER_NAMESPACE()


enum CutSequenceType
{
	CutSequence_Loop			    = 0,
	CutSequence_CommonCut		    = 1,
	CutSequence_GridLoop	        = 2,
	CutSequence_Bridge		        = 3,
	CutSequence_ChainCut		    = 4,
	CutSequence_GridCut			    = 5,
	CutSequence_FlyCut		        = 6,
	CutSequence_RemnantCutLine	    = 7,
	CutSequence_CanvasCommonCut		= 8,
};


// 切割工序。
// 注：
// 1) 切割工序表示零件的加工顺序。不过并不是每个工序都会被加工的，比如我们可以把轮廓设置为不加工，
//    那么这些轮廓对应的工序也不会被加工，可能以后还会有其他设置会让某些工序不被加工。
class ICutSequence : virtual public IData
{
public:
	virtual ~ICutSequence() {}

public:
	// 工序类型。
	virtual CutSequenceType GetCutSequenceType() const = 0;

	// 工序是不是切割零件。
	virtual BOOL IsCutPart() const = 0;

	// 快速移动点。
	virtual Point2DListPtr GetTracePtList() const = 0;
	virtual void SetTracePtList(Point2DListPtr pPtList) = 0;
	virtual void ResetTraceData() = 0;

	virtual BOOL DonotDropHeadAtBegin() const = 0;
	virtual void DonotDropHeadAtBegin(BOOL bDonotDropHeadAtBegin) = 0;

	// 得到工序开始/结束点。
	virtual Point2D GetFirstCutPt() const = 0;
	virtual Point2D GetLastCutPt() const = 0;

    // 该工序是不是会被加工，参考该类的注释。
    virtual bool SequenceWillBeCut() const = 0;
};

END_CUTLEADER_NAMESPACE()
