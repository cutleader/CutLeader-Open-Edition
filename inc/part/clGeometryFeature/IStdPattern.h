#pragma once

#include "IPattern.h"

BEGIN_CUTLEADER_NAMESPACE()

class IStdPattern;
typedef boost::shared_ptr<IStdPattern> StdPatternPtr;

// “标准图形”接口。
class IStdPattern : virtual public IPattern
{
public:
	// 得到标准图形的对称点。
	// 注：
	//   1) 在“标准图形”做了偏移后“对称点”保持不动。
	virtual Point2D GetSymmetryPt() const = 0;

	// 偏移“标准图形”。
	virtual BOOL OffsetStdPat(BOOL bInside, double dOffsetVal) = 0;
};

END_CUTLEADER_NAMESPACE()
