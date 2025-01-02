#pragma once

#include "IData.h"

DECLARE_CUTLEADER_CLASS(ClData)

BEGIN_CUTLEADER_NAMESPACE()

// the basic class of all type data, cutleader数据基类。
// 注：
//  1) 按照命名规则应该是DataBase，为了避免和数据库混淆，所以命名为ClData。
class ClBaseData_Export ClData : virtual public IData
{
public:
	ClData(void);
	virtual ~ClData(void);

public: // implements IData interface.
	virtual BOOL IsModified() const override { return m_bModified; }
	virtual void SetModified(BOOL bModified) override;
    virtual LONGLONG GetLatestModifyTimeStamp() const override { return m_iLatestModifyTimeStamp; }

	virtual BOOL IDEqual(LONGLONG iID) const override { return m_iID == iID ? TRUE : FALSE; }

	// implement this two functions here, maybe some class do not need to implement them.
	virtual DataProperty GetProperty() const override;
	virtual void UpdateProperty(const DataPropItem& propItem) override;

	// provide a default implement here, actually it's meaningless.
	virtual IDataPtr Clone() const override;
};

END_CUTLEADER_NAMESPACE()
