#pragma once

#include "ParamConfigItem.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(MicroJointRangeInfo)
DECLARE_CUTLEADER_CLASS(MicroJointRange)
DECLARE_CUTLEADER_CLASS(MicroJointConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// 微连接分配规则。
class ClExpertLib_Export MicroJointConfigItem : public ClData, virtual public ParamConfigItem
{
public:
	MicroJointConfigItem(void);
	virtual ~MicroJointConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // implements ParamConfigItem interface.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_CUT_MJ_ASSIGN; }

public: // get/set functions.
	void SetRangeList(const std::vector<MicroJointRangePtr>& rangeList) { m_rangeList = rangeList; }
	const std::vector<MicroJointRangePtr>* GetRangeList() const { return &m_rangeList; }
	std::vector<MicroJointRangePtr>* GetRangeList_FW() { return &m_rangeList; }

	void SetManualWidth(double dManualWidth) { m_dManualWidth = dManualWidth; }
	double GetManualWidth() const { return m_dManualWidth; }

	void IsLeadIn(BOOL bLeadIn) { m_bLeadIn = bLeadIn; }
	BOOL IsLeadIn() const { return m_bLeadIn; }

	void SetLeadInLen(double leadInLen) { m_leadInLen = leadInLen; }
	double GetLeadInLen() const { return m_leadInLen; }

	void SetLeadInAngle(double leadInAngle) { m_leadInAngle = leadInAngle; }
	double GetLeadInAngle() const { return m_leadInAngle; }

	void IsLeadOut(BOOL bLeadOut) { m_bLeadOut = bLeadOut; }
	BOOL IsLeadOut() const { return m_bLeadOut; }

	void SetLeadOutLen(double leadOutLen) { m_leadOutLen = leadOutLen; }
	double GetLeadOutLen() const { return m_leadOutLen; }

	void SetLeadOutAngle(double leadOutAngle) { m_leadOutAngle = leadOutAngle; }
	double GetLeadOutAngle() const { return m_leadOutAngle; }

public: // other functions.
	// notes: iRangeIdx based on 0.
	MicroJointRangeInfoPtr GetRangeInfo(int iRangeIdx, BOOL bHole) const;

	// get the proper micro joint assign info.
	// notes: when assign micro joint to loop, we should choose the correct micro joint assign info, if we do not find, return NULL.
	/* param:
	*  d1,d2: the rectangle range of the loop.
	*/
	MicroJointRangeInfoPtr GetRangeInfo(double d1, double d2, BOOL bHole) const;

private:
	// by default, we provide 3 range setting, and the order is 1,2,3.
	std::vector<MicroJointRangePtr> m_rangeList;

	// 手动分配时的微连接宽度。
	double m_dManualWidth;

	/************************************************************************/
	// the lead in/out settings for micro joint.
	// notes: here, we only support line type lead in/out.
	
	BOOL m_bLeadIn;
	double m_leadInLen;
	double m_leadInAngle;

	BOOL m_bLeadOut;
	double m_leadOutLen;
	double m_leadOutAngle;	
	/************************************************************************/
};

END_CUTLEADER_NAMESPACE()
