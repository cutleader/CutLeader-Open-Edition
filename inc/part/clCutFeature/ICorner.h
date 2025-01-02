#pragma once

#include "clExpertLibCommon.h"
#include "clCutFeatureCommon.h"
#include "IData.h"

DECLARE_CUTLEADER_CLASS(LineArc2DList)
DECLARE_CUTLEADER_CLASS(IPatternLoop)

BEGIN_CUTLEADER_NAMESPACE()

// “角特征”接口。
// 注：
//    1) we can only auto assign corner to close polygon loop, BUT can manually assign to open polygon loop.
//    2) 角特征上没有切割条件。
//    3) 父亲ID中记录了“回路角特征”ID。
class ClCutFeature_Export ICorner : virtual public IData
{
public:
    virtual ~ICorner() {}

public:
	virtual CornerType GetCornerType() const = 0;

	virtual LONGLONG GetFirstPatternID() const = 0;
	virtual void SetFirstPatternID(LONGLONG iPatternID) = 0;

	virtual LONGLONG GetSecondPatternID() const = 0;
	virtual void SetSecondPatternID(LONGLONG iPatternID) = 0;

	// 计算出角特征的二维直线和圆弧表达，这是要实际加工的刀路，不是示意性的。
    virtual bool HasCutPath() const = 0; // 角特征是否有实际加工刀路。
	virtual LineArc2DListPtr Calculate2DLineArcs(const IPatternLoop* pPatternLoop, BOOL bFollowLoopDirection) const = 0;

    // 这个角特征是否有“角特征值”。如打环角特征有半径值，而冷却点角特征暂无值设置。
    virtual bool HasCornerValue() const = 0;
};

END_CUTLEADER_NAMESPACE()
