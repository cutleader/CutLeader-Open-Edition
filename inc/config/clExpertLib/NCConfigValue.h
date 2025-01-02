#pragma once

#include "clExpertLibCommon.h"
#include "ClData.h"

DECLARE_CUTLEADER_CLASS(NCConfigType)
DECLARE_CUTLEADER_CLASS(NCConfigValue)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export NCConfigValue : public ClData
{
public:
	NCConfigValue(void);
	virtual ~NCConfigValue(void);

public: // implements IData interface.
	IDataPtr Clone() const override;

public: // get/set functions.
	void SetNCConfigType(NCConfigTypePtr ncConfigType);
	void SetNCConfigTypeEx(NCConfigTypePtr ncConfigType) { m_ncConfigType = ncConfigType; }
	NCConfigTypePtr GetNCConfigType() const { return m_ncConfigType; }

	void SetNCConfigVal(const CComVariant& ncConfigValue) { m_ncConfigValue = ncConfigValue; }
	const CComVariant& GetNCConfigVal() const { return m_ncConfigValue; }

public: // other functions.
	// notes: 
	//   1) before call this function, "m_ncConfigType" should be set, otherwise we can not get the data type of NC config.
	//   2) if bForDisplay=TRUE, "pStrVal" will be "Yes/No" for BOOL type, otherwise will be "1/0".
	void SetNCConfigVal(const char* pStrVal, BOOL bForDisplay = FALSE);

	/************************************************************************/
	// you know, condition value may have different types, here we convert the variant to string.

	// if bForDisplay=TRUE BOOL will be converted to "Yes/No", otherwise BOOL will be converted to "1/0".
	CString GetStrValue(BOOL bForDisplay);
	/************************************************************************/

private:
	//
	NCConfigTypePtr m_ncConfigType;

	// the value of the NC config type.
	CComVariant m_ncConfigValue;
};

END_CUTLEADER_NAMESPACE()
