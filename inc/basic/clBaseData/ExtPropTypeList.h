#pragma once

#include "clBaseDataCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ExtPropType)

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export ExtPropTypeList : public std::vector<ExtPropTypePtr>
{
public:
	ExtPropTypeList(void);
	~ExtPropTypeList(void);

public:
	// �����Ƶõ�����
	ExtPropTypePtr GetItemByName(CString strName);

	// ɾ������
	void DeleteItemByName(CString strName);
};

END_CUTLEADER_NAMESPACE()
