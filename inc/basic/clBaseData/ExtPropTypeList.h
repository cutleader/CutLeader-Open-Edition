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
	// 从名称得到对象。
	ExtPropTypePtr GetItemByName(CString strName);

	// 删除对象。
	void DeleteItemByName(CString strName);
};

END_CUTLEADER_NAMESPACE()
