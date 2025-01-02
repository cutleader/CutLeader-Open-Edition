#pragma once

#include "clExpertLibCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ParamConfigList)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(DefaultParamConfig)
DECLARE_CUTLEADER_CLASS(MicroJointRange)
DECLARE_CUTLEADER_CLASS(MicroJointConfigItem)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)
DECLARE_CUTLEADER_CLASS(ParamConfig)
DECLARE_CUTLEADER_CLASS(ParamConfigList)

BEGIN_CUTLEADER_NAMESPACE()

class ClExpertLib_Export ParamConfigWriter
{
public:
	/************************************************************************/
	// about the basic info.

	// update the name of the scheme.
	static void UpdateConfigName(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, CString strName, BOOL bTransaction = TRUE);
	/************************************************************************/

	/************************************************************************/
	// 关于保存“参数配置”。
	// notes:
	//   1) before save the param config, caller should make sure that the expLib item exist.

	// 保存“参数配置”。
	static void SaveParamConfig(DBConnectPtr pDBConnect, ParamConfigPtr pParamConfig, BOOL bTransaction = TRUE);

	// 保存“参数配置”集合。
	static void SaveParamConfigList(DBConnectPtr pDBConnect, ParamConfigListPtr pParamConfigList, BOOL bTransaction = TRUE);
	/************************************************************************/

	// 删除“参数配置”。
	static void DeleteConfig(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, BOOL bTransaction = TRUE);

	static void SaveDflParamConfig(DBConnectPtr pDBConnect, DefaultParamConfigPtr pDflParamConfig);
};

END_CUTLEADER_NAMESPACE()
