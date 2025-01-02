#pragma once

#include "ClData.h"
#include "ParamConfigItem.h"

DECLARE_CUTLEADER_CLASS(NCConfigValueList)
DECLARE_CUTLEADER_CLASS(NCConfigItem)

BEGIN_CUTLEADER_NAMESPACE()

// “NC代码生成规则”。
class ClExpertLib_Export NCConfigItem : public ClData,
								  virtual public ParamConfigItem
{
public:
	NCConfigItem(void);
	virtual ~NCConfigItem(void);

public: // implements IData interface.
	virtual IDataPtr Clone() const override;

public: // implements ParamConfigItem interface.
	PARAMCONFIG_ITEM_TYPE GetParamConfigItemType() const override { return PARAMCONFIG_ITEM_GENERATE_NC; }

public: // get/set functions.
	void SetNCConfigVal(NCConfigValueListPtr pNCConfigValList) { m_pNCConfigValList = pNCConfigValList; }
	NCConfigValueListPtr GetNCConfigVal() const { return m_pNCConfigValList; }

private:
	NCConfigValueListPtr m_pNCConfigValList;
};

END_CUTLEADER_NAMESPACE()
