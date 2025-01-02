#pragma once

#include "ClData.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(ExtPropVal)
DECLARE_CUTLEADER_CLASS(ExtPropValList)

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export ExtPropValList : public std::vector<ExtPropValPtr>, public ClData
{
public:
	ExtPropValList(void);
	virtual ~ExtPropValList(void);

public:
	virtual IDataPtr Clone() const override;

public:
	// 从名称得到对象。
	ExtPropValPtr GetItemByName(CString strName) const;
};

END_CUTLEADER_NAMESPACE()
