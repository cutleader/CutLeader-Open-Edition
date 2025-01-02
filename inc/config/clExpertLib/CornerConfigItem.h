#pragma once

#include "ClData.h"
#include "ParamConfigItem.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(CornerRangeInfo)
DECLARE_CUTLEADER_CLASS(CornerConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// “角特征分配规则”。
class ClExpertLib_Export CornerConfigItem : public ClData,
									  virtual public ParamConfigItem
{
public:
	CornerConfigItem(void);
	virtual ~CornerConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // implements ParamConfigItem interface.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_CORNER_ASSIGN; }

public: // get/set functions.
	void SetDflCornerType(CornerType cornerType) { m_iDflCornerType = cornerType; }
	CornerType GetDflCornerType() const { return m_iDflCornerType; }

	void SetDflCornerVal(double dVal) { m_dDflVal = dVal; }
	double GetDflCornerVal() const { return m_dDflVal; }

	void SetRangeCornerInfoList(std::vector<CornerRangeInfoPtr> pRangeCornerInfoList) { m_pRangeCornerInfoList = pRangeCornerInfoList; }
	std::vector<CornerRangeInfoPtr>* GetRangeCornerInfoList() { return &m_pRangeCornerInfoList; }

public: // other functions.
	CornerRangeInfoPtr GetRangeCornerByAngle(double dAngle) const;

private:
	// 默认使用的角类型。
	CornerType m_iDflCornerType;

	// 默认使用的角值。
	double m_dDflVal;

	// 每个角度范围对应的角特征。
	std::vector<CornerRangeInfoPtr> m_pRangeCornerInfoList;
};

END_CUTLEADER_NAMESPACE()
