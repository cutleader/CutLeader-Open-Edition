#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(ParamConfigList)

BEGIN_CUTLEADER_NAMESPACE()

// ���������á����ϡ�
class ParamConfigList : public std::vector<ParamConfigPtr>,
						public ClData
{
public:
	ParamConfigList(void);
	~ParamConfigList(void);

public: // implements IData interface.
	// ��¡���������á����ϡ�
	// ע��:
	//   1) ��Ҫ��¡ÿ�����������á���
	IDataPtr Clone() const override;

public:
	// update the parent ID of all scheme info object.
	void UpdateParentID(LONGLONG iExpLibItemID);
};

END_CUTLEADER_NAMESPACE()
