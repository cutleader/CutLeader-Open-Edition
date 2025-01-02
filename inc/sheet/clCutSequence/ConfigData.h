#pragma once

#include "clCutSequenceCommon.h"

DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(MachineParam)

BEGIN_CUTLEADER_NAMESPACE()

// 工序处理时用到的参数。
class ClCutSequence_Export ConfigData
{
public:
	ConfigData(LeadConfigItemPtr pLeadConfigItem, SequenceConfigItemPtr pSeqConfigItem, IMaterialSizePtr pMaterialSize, MachineParamPtr pMachineParam);
	~ConfigData(void);

public: // get/set functions.
	const LeadConfigItem* GetLeadConfigItem() const { return m_pLeadConfigItem.get(); }
    LeadConfigItemPtr GetLeadConfigItem_FW() const { return m_pLeadConfigItem; }
	const SequenceConfigItem* GetSeqConfigItem() const { return m_pSeqConfigItem.get(); }
	const IMaterialSize* GetMatSize() const { return m_pMatSize.get(); }
	const MachineParam* GetMachineParam() const { return m_pMachineParam.get(); }

private:
	LeadConfigItemPtr m_pLeadConfigItem;
	SequenceConfigItemPtr m_pSeqConfigItem;
	IMaterialSizePtr m_pMatSize;
	MachineParamPtr m_pMachineParam;
};

END_CUTLEADER_NAMESPACE()
