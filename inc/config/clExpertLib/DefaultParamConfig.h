#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

// 默认的“参数配置”。
class ClExpertLib_Export DefaultParamConfig : public ClData
{
public:
	DefaultParamConfig(void);
	DefaultParamConfig(LONGLONG iExpLibItemID, LONGLONG iParamConfigID);
	virtual ~DefaultParamConfig(void);

public: // get/set functions.
	void SetExpLibItemID(LONGLONG iExpLibItemID) { m_iExpLibItemID = iExpLibItemID; }
	LONGLONG GetExpLibItemID() const { return m_iExpLibItemID; }

	void SetParamConfigID(LONGLONG iParamConfigID) { m_iParamConfigID = iParamConfigID; }
	LONGLONG GetParamConfigID() const { return m_iParamConfigID; }

private:
	LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;
};

END_CUTLEADER_NAMESPACE()
