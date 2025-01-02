#pragma once

#include "clUtilityCommon.h"
#include "clBaseDataCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// ��չ�������͡�
// ע��
//  1) ����IData�̳У�ͨ������Ψһ�������Ҷ���
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
	// ͨ�����͵����ơ�
	CString GetDataTypeName();
	void SetDataType(CString str);

public:
	static StringListPtr GetAllDataType();

private:
	// �������ơ�
	CString m_strName;

	// �����������͡�
	BASIC_DATA_TYPE m_iDataType;
};

END_CUTLEADER_NAMESPACE()
