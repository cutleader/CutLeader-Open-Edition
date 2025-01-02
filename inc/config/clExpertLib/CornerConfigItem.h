#pragma once

#include "ClData.h"
#include "ParamConfigItem.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(CornerRangeInfo)
DECLARE_CUTLEADER_CLASS(CornerConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// ��������������򡱡�
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
	// Ĭ��ʹ�õĽ����͡�
	CornerType m_iDflCornerType;

	// Ĭ��ʹ�õĽ�ֵ��
	double m_dDflVal;

	// ÿ���Ƕȷ�Χ��Ӧ�Ľ�������
	std::vector<CornerRangeInfoPtr> m_pRangeCornerInfoList;
};

END_CUTLEADER_NAMESPACE()
