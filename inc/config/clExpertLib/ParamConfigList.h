#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(ParamConfigList)

BEGIN_CUTLEADER_NAMESPACE()

// “参数配置”集合。
class ParamConfigList : public std::vector<ParamConfigPtr>,
						public ClData
{
public:
	ParamConfigList(void);
	~ParamConfigList(void);

public: // implements IData interface.
	// 克隆“参数配置”集合。
	// 注意:
	//   1) 需要克隆每个“参数配置”。
	IDataPtr Clone() const override;

public:
	// update the parent ID of all scheme info object.
	void UpdateParentID(LONGLONG iExpLibItemID);
};

END_CUTLEADER_NAMESPACE()
