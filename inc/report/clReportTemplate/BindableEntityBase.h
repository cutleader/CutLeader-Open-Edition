#pragma once

#include "clReportTemplateCommon.h"
#include "IBindableEntity.h"
#include "TplEntityBase.h"
#include "BindItem.h"

BEGIN_CUTLEADER_NAMESPACE()

// the basic class for the bindable entities.
class BindableEntityBase : virtual public TplEntityBase,
						   virtual public IBindableEntity
{
public:
	BindableEntityBase(void);
	~BindableEntityBase(void);

public: // implement interface IBindableEntity.
	virtual void SetBindTargetType(BIND_ITEM_TYPE bindItemType) { m_bindItemType = bindItemType; }
	virtual BIND_ITEM_TYPE GetBindTargetType() { return m_bindItemType; }

	virtual void SetExtPropName(CString strExtPropName) { m_strExtPropName = strExtPropName; }
	virtual CString GetExtPropName() { return m_strExtPropName; }

protected:
	// the "bind item" type.
	// notes:
	//   1) use intData18.
	BIND_ITEM_TYPE m_bindItemType;

	// 绑定的扩展属性，为空即没有绑定。
	CString m_strExtPropName;
};

END_CUTLEADER_NAMESPACE()
