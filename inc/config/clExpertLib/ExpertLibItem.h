#pragma once

#include "clExpertLibCommon.h"
#include "BusinessData.h"

DECLARE_CUTLEADER_CLASS(MachineItem)
DECLARE_CUTLEADER_CLASS(ParamConfigList)
DECLARE_CUTLEADER_CLASS(ExpertLibItem)

BEGIN_CUTLEADER_NAMESPACE()

// the expert library info under a machine.
// notes:
//   1) data members in this class will be allocated outside this class.
//   2) for reserved machine, i think we MUST provide the driver. BUT maybe not provide the condition info.
//   3) the ExpertLibItem of "reserved" machine, we can see it only in explib management UI, cannot use it somewhere else.
class ClExpertLib_Export ExpertLibItem : public BusinessData
{
public:
	ExpertLibItem(void);
	virtual ~ExpertLibItem(void);

public: // implement interface IData
	// clone ExpertLibItem object.
	// notes:
	//   1) after cloned each scheme info object, we should update their parent ID as the ExpertLibItem ID.
	//   2) this function do not clone the mac item.
	virtual IDataPtr Clone() const override;

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_ExpertLibItem; }

public: // get/set functions.
	void SetMacItem(MachineItemPtr pMacItem) { m_pMacItem = pMacItem; }
	MachineItemPtr GetMacItem() const { return m_pMacItem; }

	void SetParamConfigList(ParamConfigListPtr pParamConfigList) { m_pParamConfigList = pParamConfigList; }
	ParamConfigListPtr GetParamConfigList() const { return m_pParamConfigList; }

private:
	// the machine item.
	MachineItemPtr m_pMacItem;

	// 该专家库项下面的“参数配置”集合。
	ParamConfigListPtr m_pParamConfigList;
};

END_CUTLEADER_NAMESPACE()
