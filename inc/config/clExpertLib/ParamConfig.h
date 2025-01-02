#pragma once

#include "ClData.h"
#include "ParamConfigItemList.h"

DECLARE_CUTLEADER_CLASS(ParamConfig)

BEGIN_CUTLEADER_NAMESPACE()

// “参数配置”对象。
// 注：
//  1) 把“专家库项”的ID设置为父亲ID。
class ClExpertLib_Export ParamConfig : public ParamConfigItemList, public ClData
{
public:
	ParamConfig(void);
	ParamConfig(CString strName);
	virtual ~ParamConfig(void);

public: // implement interface IData
	// notes:
	//   1) 克隆之后，要更新新对象的父亲ID。
	virtual IDataPtr Clone() const override;

public: // set/get functions
	CString GetName() const { return m_strName; }
	void SetName(CString strName) { m_strName = strName; }

private:
	// the param config name.
	CString m_strName;
};

END_CUTLEADER_NAMESPACE()
