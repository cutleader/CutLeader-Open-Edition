#pragma once

#include "ICutSequence.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// 工序的基类。
class CutSequenceBase : virtual public ICutSequence,
				   public ClData
{
public:
	CutSequenceBase(void);
	virtual ~CutSequenceBase(void);

public: // implement interface ICutSequence.
	virtual BOOL IsCutPart() const override { return TRUE; }
    virtual bool SequenceWillBeCut() const override { return true; }

public: // implement interface ICutPartSequence.
	virtual Point2DListPtr GetTracePtList() const { return m_pTracePtList; }
	virtual void SetTracePtList(Point2DListPtr pPtList) { m_pTracePtList = pPtList; }
	virtual void ResetTraceData();

	virtual BOOL DonotDropHeadAtBegin() const { return m_bDonotDropHeadAtBegin; }
	virtual void DonotDropHeadAtBegin(BOOL bDonotDropHeadAtBegin) { m_bDonotDropHeadAtBegin = bDonotDropHeadAtBegin; }

protected:
	// 从上一个工序到这个工序的“快速移动点”。
	// 注：
	//   1) 当要避让上一个工序时才会初始化这个数据。
	Point2DListPtr m_pTracePtList;

	// 可以理解为加工这个工序时要不要把激光头放下。
	// 注：
	//   1) 为true时，一般都是做了“路径避让”，从而不会经过洞口上方，从而不需抬放激光头。
	//   2) 如果这个值为否，那么上一个工序加工结束时需要把激光头抬起，开始加工这个工序时需要把激光头放下。
	BOOL m_bDonotDropHeadAtBegin;
};

END_CUTLEADER_NAMESPACE()
