#pragma once

#include "clUtilityCommon.h"
#include "clBaseDataCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// 扩展属性类型。
// 注：
//  1) 不从IData继承，通过名称唯一性来查找对象。
class ClBaseData_Export ExtPropType
{
public:
	ExtPropType(void);
	ExtPropType(CString strName, BASIC_DATA_TYPE iDataType);
	~ExtPropType(void);

public: // get/set functions.
	void SetName(CString strName) { m_strName = strName; }
	CString GetName() { return m_strName; }

	void SetDataType(BASIC_DATA_TYPE iDataType) { m_iDataType = iDataType; }
	BASIC_DATA_TYPE GetDataType() { return m_iDataType; }

public:
	// 通过类型的名称。
	CString GetDataTypeName();
	void SetDataType(CString str);

public:
	static StringListPtr GetAllDataType();

private:
	// 属性名称。
	CString m_strName;

	// 属性数据类型。
	BASIC_DATA_TYPE m_iDataType;
};

END_CUTLEADER_NAMESPACE()
