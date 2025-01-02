#pragma once

#include "ClData.h"
#include "ParamConfigItemList.h"

DECLARE_CUTLEADER_CLASS(ParamConfig)

BEGIN_CUTLEADER_NAMESPACE()

// ���������á�����
// ע��
//  1) �ѡ�ר�ҿ����ID����Ϊ����ID��
class ClExpertLib_Export ParamConfig : public ParamConfigItemList, public ClData
{
public:
	ParamConfig(void);
	ParamConfig(CString strName);
	virtual ~ParamConfig(void);

public: // implement interface IData
	// notes:
	//   1) ��¡֮��Ҫ�����¶���ĸ���ID��
	virtual IDataPtr Clone() const override;

public: // set/get functions
	CString GetName() const { return m_strName; }
	void SetName(CString strName) { m_strName = strName; }

private:
	// the param config name.
	CString m_strName;
};

END_CUTLEADER_NAMESPACE()
