#pragma once

#include "clCutFeatureCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ICorner)
DECLARE_CUTLEADER_CLASS(CornerList)

BEGIN_CUTLEADER_NAMESPACE()

// 角特征集合。
class ClCutFeature_Export CornerList : public std::vector<ICornerPtr>,
									 public ClData
{
public:
	CornerList(void);
	virtual ~CornerList(void);

public: // implement IData interface
	virtual IDataPtr Clone() const override;

public:
	// 删除指定的角特征。
	void RemoveCorner(ICornerPtr pCorner);

	// 得到指定的角特征。
	ICornerPtr GetCornerByID(LONGLONG iID);

	// 这些角特征是不是有一样的类型。
	BOOL SameCornerType();
};

END_CUTLEADER_NAMESPACE()
