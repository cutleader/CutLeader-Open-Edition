#pragma once

#include "clOptionCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(SnapPtParam)
DECLARE_CUTLEADER_CLASS(GlViewPort)

BEGIN_CUTLEADER_NAMESPACE()

// 显示设置。
class ClOption_Export SnapPtParam : public ClData
{
public:
	SnapPtParam(void);
	~SnapPtParam(void);

public: // get/set functions.
	BOOL GetIsEnableSnapPt() const { return m_bEnableSnapPt; }
	void SetIsEnableSnapPt(BOOL b) { m_bEnableSnapPt = b; }

	unsigned int GetSnapRange() const { return m_iSnapRange; }
	void SetSnapRange(unsigned int i) { m_iSnapRange = i; }

	BOOL GetIsSnapEndPt() const { return m_bSnapEndPt; }
	void SetIsSnapEndPt(BOOL b) { m_bSnapEndPt = b; }

	BOOL GetIsSnapMiddlePt() const { return m_bSnapMiddlePt; }
	void SetIsSnapMiddlePt(BOOL b) { m_bSnapMiddlePt = b; }

	BOOL GetIsSnapCenterPt() const { return m_bSnapCenterPt; }
	void SetIsSnapCenterPt(BOOL b) { m_bSnapCenterPt = b; }

	BOOL GetIsSnapClosestPt() const { return m_bSnapClosestPt; }
	void SetIsSnapClosestPt(BOOL b) { m_bSnapClosestPt = b; }

	BOOL GetIsSnapFootPt() const { return m_bSnapFootPt; }
	void SetIsSnapFootPt(BOOL b) { m_bSnapFootPt = b; }

	BOOL GetIsSnapTangencyPt() const { return m_bSnapTangencyPt; }
	void SetIsSnapTangencyPt(BOOL b) { m_bSnapTangencyPt = b; }

	BOOL GetIsSnapCrossPt() const { return m_bSnapCrossPt; }
	void SetIsSnapCrossPt(BOOL b) { m_bSnapCrossPt = b; }

public:
	// 计算出世界坐标的捕获距离。
	double GetSnapRangeInThisView(const GlViewPort* pViewPort) const;

private:
	BOOL m_bEnableSnapPt;
	unsigned int m_iSnapRange;

	BOOL m_bSnapEndPt;
	BOOL m_bSnapMiddlePt;
	BOOL m_bSnapCenterPt;
	BOOL m_bSnapClosestPt;
	BOOL m_bSnapFootPt;
	BOOL m_bSnapTangencyPt;
	BOOL m_bSnapCrossPt;
};

END_CUTLEADER_NAMESPACE()
