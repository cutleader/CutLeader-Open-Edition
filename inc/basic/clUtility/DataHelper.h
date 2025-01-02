#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// this class can do some data conversions.
class ClUtility_Export DataHelper
{
public:
	// convert the value by the format.
	// e.g., convert 3.236 to 3.24 if the format is "3.2"
	static double ConvertValueByFormat(double dOldValue, CString strFormat);

	// convert string to variant value.
	// notes:
	//   1) if "dataType" is "BASIC_DATA_BOOL", "strVal" will be "1/0", and "VT_BOOL" will be returned.
	static CComVariant ConvertStrToVar(const CString& strVal, BASIC_DATA_TYPE dataType);

	// convert variant value to string.
	// notes:
	//   1) though "var" has the type, we also use "dataType" to indicate the type.
	//   2) if "dataType" is "BASIC_DATA_BOOL", "var" will be "VT_BOOL", and "1/0" will be returned.
	static CString ConvertVarToStr(const CComVariant& var, BASIC_DATA_TYPE dataType);

    // 得到各种类型数据的默认值
    static CString GetDefaultValue(BASIC_DATA_TYPE dataType);
	static CString GetDefaultValue_4_display(BASIC_DATA_TYPE dataType);

	// 从用于显示的值转到实际值
	static CString DisplayVal_2_realVal(CString strVal_4_display, BASIC_DATA_TYPE iDataType);
};

END_CUTLEADER_NAMESPACE()
