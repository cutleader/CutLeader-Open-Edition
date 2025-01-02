#pragma once

#include "clBaseDataCommon.h"
#include "IBusinessData.h"
#include "ClData.h"

BEGIN_CUTLEADER_NAMESPACE()

class ClBaseData_Export BusinessData : public ClData, virtual public IBusinessData
{
public:
	BusinessData(void);
	virtual ~BusinessData(void);

public: // implement interface IBusinessData.
	void SetName(CString strName) override { m_strName = strName; }
	CString GetName() const override { return m_strName; }

	void SetComment(CString strComment) override { m_strComment = strComment; }
	CString GetComment() const override { return m_strComment; }

	void SetCreateTime(LONGLONG iCreateTimeStamp) override { m_iCreateTimeStamp = iCreateTimeStamp; }
	LONGLONG GetCreateTime() const override { return m_iCreateTimeStamp; }

	void SetModifyTime(LONGLONG iModifyTimeStamp) override { m_iModifyTimeStamp = iModifyTimeStamp; }
	LONGLONG GetModifyTime() const override { return m_iModifyTimeStamp; }

protected:
	//
	CString m_strName;
	CString m_strComment;

	// create and modify date.
	LONGLONG m_iCreateTimeStamp;
	LONGLONG m_iModifyTimeStamp;
};

END_CUTLEADER_NAMESPACE()
