#pragma once

#include "clCutSequenceCommon.h"
#include "ClData.h"
#include "Rect2D.h"

DECLARE_CUTLEADER_CLASS(AvoidData)

BEGIN_CUTLEADER_NAMESPACE()

// 避让数据。
// 注：
//  1) 这个数据用来处理某些工序内部的避让。
class AvoidData : public ClData
{
public:
	AvoidData(void);
	AvoidData(Rect2D rangeRect, double dCutOutAvoid, double dAvoidDis, double dAvoidRatio);
	virtual ~AvoidData(void);

public: // implement IData interface.
	// clone the avoid data.
	virtual IDataPtr Clone() const override;

public: // get/set functions.
	void IsProcessAvoid(BOOL bProcessAvoid) { m_bProcessAvoid = bProcessAvoid; }
	BOOL IsProcessAvoid() const { return m_bProcessAvoid; }

	void SetRangeRect(Rect2D rangeRect) { m_rangeRect = rangeRect; }
	Rect2D GetRangeRect() const { return m_rangeRect; }

	void SetCutOutAvoid(double dCutOutAvoid) { m_dCutOutAvoid = dCutOutAvoid; }
	double GetCutOutAvoid() const { return m_dCutOutAvoid; }

	void SetAvoidDis(double dAvoidDis) { m_dAvoidDis = dAvoidDis; }
	double GetAvoidDis() const { return m_dAvoidDis; }

	void SetAvoidRatio(double dAvoidRatio) { m_dAvoidRatio = dAvoidRatio; }
	double GetAvoidRatio() const { return m_dAvoidRatio; }

private:
	// 是否处理内部避让。
	BOOL m_bProcessAvoid;

	// 快速移动点被限制的范围。
	// 注：
	//  1) 这个范围是通过板材边界，机器台面，超行程计算出来的，实际计算快速移动点时还会进一步缩小范围。
	Rect2D m_rangeRect;

	// 避让参数。
	double m_dCutOutAvoid;
	double m_dAvoidDis;
	double m_dAvoidRatio;
};

END_CUTLEADER_NAMESPACE()
