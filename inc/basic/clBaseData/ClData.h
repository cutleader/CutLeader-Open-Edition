#pragma once

#include "IData.h"

DECLARE_CUTLEADER_CLASS(ClData)

BEGIN_CUTLEADER_NAMESPACE()

// the basic class of all type data, cutleader���ݻ��ࡣ
// ע��
//  1) ������������Ӧ����DataBase��Ϊ�˱�������ݿ��������������ΪClData��
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
