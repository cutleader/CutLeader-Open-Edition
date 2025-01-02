#pragma once

#include "clExpertLibCommon.h"
#include "DataProperty.h"
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

class ClExpertLib_Export ParamConfigLoader
{
public:
	/************************************************************************/
	// get basic info of the scheme.

	// 得到“参数配置”的信息。
	// notes:
	//   1) the property item:
	//      1) “参数配置” id
	//      2) ExpertLibItem id
	//      3) “参数配置” name
	//      4) ExpertLibItem name
	static BOOL GetParamConfigInfo(DBConnectPtr pDBConnect, LONGLONG iConfigID, DataProperty& prop);

	// 得到“专家库项”下所有“参数配置”信息。
	// 注:
	//   1) the property item:
	//      1) “参数配置” id
	//      2) ExpertLibItem id
	//      3) “参数配置”名称。
	static std::vector<DataProperty> GetParamConfigList(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID);

	// 从“参数配置”ID得到名称。
	// notes:
	//   1) if cannot find, return EMPTY_STRING.
	static CString GetConfigName(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// 通过“参数配置”名称得到ID。
	static LONGLONG GetConfigIDByName(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, CString strConfigName);
	/************************************************************************/

	/************************************************************************/
	// relative to ExpertLibItem.

	// 得到“参数配置”所属的“专家库项”。
	static LONGLONG GetExpLibItemID(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// get ExpertLibItem name by scheme id
	static CString GetExpLibItemName(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);
	/************************************************************************/

	/************************************************************************/
	// 关于加载“参数配置”。

	// 加载“参数配置”对象。
	static ParamConfigPtr LoadParamConfig(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// 加载“专家库项”下所有“参数配置”信息。
	static ParamConfigListPtr LoadParamConfigList(DBConnectPtr pDBConnect, LONGLONG iExpLibItemID, ConditionTypeListPtr pAllCondTypeList, NCConfigTypeListPtr pAllNCConfigTypeList);
	/************************************************************************/

	static DefaultParamConfigPtr LoadDflParamConfig(DBConnectPtr pDBConnect);

	// 加载“微连接分配规则”.
	static MicroJointConfigItemPtr LoadMicroJointSetting(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

private:
	// 加载“参数配置”对象中的“参数配置项”集合。
	static void LoadParamConfigItemList(DBConnectPtr pDBConnect, ParamConfigPtr& pParamConfig, ConditionTypeListPtr pAllCondTypeList, NCConfigTypeListPtr pAllNCConfigTypeList);

	// init the ranges of the MicroJointSetting.
	static void InitMJRanges(DBConnectPtr pDBConnect, LONGLONG iMicroJointSettingId, std::vector<MicroJointRangePtr>* pRangeList);
};

END_CUTLEADER_NAMESPACE()
