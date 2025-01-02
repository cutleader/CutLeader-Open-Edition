#pragma once

#include "clMachineLibCommon.h"
#include "BusinessData.h"

DECLARE_CUTLEADER_CLASS(MachineParam)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)
DECLARE_CUTLEADER_CLASS(MachineItem)

BEGIN_CUTLEADER_NAMESPACE()

// 机器对象。
class ClMachineLib_Export MachineItem : public BusinessData
{
public:
	MachineItem(void);
	MachineItem(CString strName, CString strComment);
	MachineItem(LONGLONG iID, CString strName, CString strComment);
	virtual ~MachineItem(void);

public: // implement interface IData
	virtual IDataPtr Clone() const override;

public: // implement IBusinessData interface
	virtual BusinessDataType GetBusinessDataType() const override { return BusinessData_MachineItem; }

public: // get/set functions.
	virtual BOOL IsReserved() const { return m_bReserved; }
	virtual void IsReserved(BOOL bReserved) { m_bReserved = bReserved; }

	MachineParamPtr GetMacParam() const { return m_pMacParam; }
	void SetMacParam(MachineParamPtr pMacParam) { m_pMacParam = pMacParam; }

	ConditionTypeListPtr GetCondTypeList() const { return m_pCondTypeList; }
	void SetCondTypeList(ConditionTypeListPtr pCondTypeList) { m_pCondTypeList = pCondTypeList; }

	NCConfigTypeListPtr GetNCConfigTypeList() const { return m_pNCConfigTypeList; }
	void SetNCConfigTypeList(NCConfigTypeListPtr pNCConfigTypeList) { m_pNCConfigTypeList = pNCConfigTypeList; }

	void IsVisible(BOOL bVisible) { m_bVisible = bVisible; }
	BOOL IsVisible() const { return m_bVisible; }

private:
	// whether this machine is reserved and cannot be deleted.
	BOOL m_bReserved;

	// the machine param.
	MachineParamPtr m_pMacParam;

	// the available condition of the machine.
	ConditionTypeListPtr m_pCondTypeList;

	// the available NC config types of the machine.
	NCConfigTypeListPtr m_pNCConfigTypeList;

	// 是否可见。
	// 注：
	//  1) 保留的机器是不可见的。
	BOOL m_bVisible;
};

END_CUTLEADER_NAMESPACE()
