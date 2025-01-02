#pragma once

#include "ITplEntity.h"
#include "BindItem.h"

DECLARE_CUTLEADER_CLASS(IBindableEntity)

BEGIN_CUTLEADER_NAMESPACE()

class IBindableEntity : virtual public ITplEntity
{
public:
	// 关于数据的属性。
	virtual void SetBindTargetType(BIND_ITEM_TYPE bindItemType) = 0;
	virtual BIND_ITEM_TYPE GetBindTargetType() = 0;

	// 关于数据的扩展属性。
	virtual void SetExtPropName(CString strExtPropName) = 0;
	virtual CString GetExtPropName() = 0;
};

END_CUTLEADER_NAMESPACE()
