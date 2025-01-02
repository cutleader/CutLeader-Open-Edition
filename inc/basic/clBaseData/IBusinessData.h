#pragma once

#include "clBaseDataCommon.h"
#include "IData.h"

BEGIN_CUTLEADER_NAMESPACE()


enum BusinessDataType
{
	BusinessData_Part				= 200,
	BusinessData_TaskItem			= 201,
	BusinessData_MachineItem		= 202,
	BusinessData_ExpertLibItem		= 203,
	BusinessData_Material			= 204,
	BusinessData_ReportTemplate		= 205,
};


// the interface for business data, 业务对象的接口。
class ClBaseData_Export IBusinessData : virtual public IData
{
public:
	virtual ~IBusinessData() {}

public:
	virtual BusinessDataType GetBusinessDataType() const = 0;

	virtual void SetName(CString strName) = 0;
	virtual CString GetName() const = 0;

	virtual void SetComment(CString strComment) = 0;
	virtual CString GetComment() const = 0;

	virtual void SetCreateTime(LONGLONG iCreateTimeStamp) = 0;
	virtual LONGLONG GetCreateTime() const = 0;

	virtual void SetModifyTime(LONGLONG iModifyTimeStamp) = 0;
	virtual LONGLONG GetModifyTime() const = 0;
};

END_CUTLEADER_NAMESPACE()
