#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"
#include "ConditionType.h"

DECLARE_CUTLEADER_CLASS(CondValInfo)

BEGIN_CUTLEADER_NAMESPACE()

// Ìõ¼þÖµ¡£
class ClExpertLib_Export CondValInfo : public ClData
{
public:
	CondValInfo();
	~CondValInfo();

public: // implements IData interface.
	// notes:
	//   1) we should not clone "m_condType".
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetCondType(ConditionTypePtr condType) { m_condType = condType; }
	ConditionTypePtr GetCondType() { return m_condType; }

	void SetCondVal(const CComVariant& condValue) { m_condValue = condValue; }
	const CComVariant& GetCondVal() { return m_condValue; }

public:
	// notes: 
	//   1) before call this function, "m_condType" should be set, otherwise we can not get the condition data type.
	//   2) if bForDisplay=TRUE, "pStrVal" will be "Yes/No" for BOOL type, otherwise will be "1/0".
	void SetCondValue(const char* pStrVal, BOOL bForDisplay = FALSE);

	// condition value may have different types, here we convert the variant to string.
	// if bForDisplay=TRUE BOOL will be converted to "Yes/No", otherwise BOOL will be converted to "1/0".
	CString GetStrValue(BOOL bForDisplay);

private:
	// the condition type.
	ConditionTypePtr m_condType;

	// the condition value.
	CComVariant m_condValue;
};

END_CUTLEADER_NAMESPACE()
