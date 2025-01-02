#pragma once

#include "clExpertLibCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(MicroJointRange)
DECLARE_CUTLEADER_CLASS(MicroJointConfigItem)
DECLARE_CUTLEADER_CLASS(CondConfigItem)
DECLARE_CUTLEADER_CLASS(CornerConfigItem)
DECLARE_CUTLEADER_CLASS(LeadConfigItem)
DECLARE_CUTLEADER_CLASS(PierceConfigItem)
DECLARE_CUTLEADER_CLASS(NCConfigItem)
DECLARE_CUTLEADER_CLASS(SequenceConfigItem)
DECLARE_CUTLEADER_CLASS(NestConfigItem)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(NCConfigTypeList)
DECLARE_CUTLEADER_CLASS(ConditionTypeList)
DECLARE_CUTLEADER_CLASS(CateAssignInfoList)
DECLARE_CUTLEADER_CLASS(CondValInfoList)
DECLARE_CUTLEADER_CLASS(CornerRangeInfo)
DECLARE_CUTLEADER_CLASS(NCConfigValue)
DECLARE_CUTLEADER_CLASS(NCConfigType)
DECLARE_CUTLEADER_CLASS(ComCutParam)
DECLARE_CUTLEADER_CLASS(CollisionAvoidParam)
DECLARE_CUTLEADER_CLASS(BridgeCutParam)
DECLARE_CUTLEADER_CLASS(GridCutParam)
DECLARE_CUTLEADER_CLASS(FlyCutParam)
DECLARE_CUTLEADER_CLASS(CategoryAssignInfo)
DECLARE_CUTLEADER_CLASS(ChainCutParam)

BEGIN_CUTLEADER_NAMESPACE()

// 这个类把“参数配置项”存入数据库。
class ClExpertLib_Export ConfigItemWriter
{
public:
    static void SaveToolAssignConfigItem(DBConnectPtr pDBConnect, ToolAssignConfigItemPtr pToolAssignConfigItem, BOOL bTransaction = TRUE);
	static void SaveMicroJointConfigItem(DBConnectPtr pDBConnect, MicroJointConfigItemPtr pMicroJointConfigItem, BOOL bTransaction = TRUE);
	static void SaveCornerConfigItem(DBConnectPtr pDBConnect, CornerConfigItemPtr pCornerConfigItem, BOOL bTransaction = TRUE);
	static void SaveLeadSetting(DBConnectPtr pDBConnect, LeadConfigItemPtr pLeadConfigItem, BOOL bTransaction = TRUE);
	static void SavePierceConfigItem(DBConnectPtr pDBConnect, PierceConfigItemPtr setting, BOOL bTransaction = TRUE);
	static void SaveNCConfigItem(DBConnectPtr pDBConnect, NCConfigItemPtr pNCConfigItem, BOOL bTransaction = TRUE);
	static void SaveSeqConfigItem(DBConnectPtr pDBConnect, SequenceConfigItemPtr pSeqConfigItem, BOOL bTransaction = TRUE);
	static void SaveNestConfigItem(DBConnectPtr pDBConnect, NestConfigItemPtr pNestConfigItem, BOOL bTransaction = TRUE);

    // 检查每个参数配置是不是都有刀具分配信息，没有的话需要补上默认的。注意调用这个函数检查之前，需要确认数据库表的字段是对的。
    static void CheckData_of_toolAssignInfo(DBConnectPtr pDBConnect);

    // 专家库版本升到6.3时，expLib_geomLeadInOut中新增了字段，调用这个函数可以把老版本的专家库升级。
    static void CheckTable_4_v63();

private:
	static void UpdateMJRange(DBConnectPtr pDBConnect, LONGLONG iMicroJointSettingId, std::vector<MicroJointRangePtr>* rangeList);
	static void SaveRangeCornerInfo(DBConnectPtr pDBConnect, LONGLONG cornerAssignInfoid, std::vector<CornerRangeInfoPtr>* rangeCornerInfos);
	static void SaveGeomLead(DBConnectPtr pDBConnect, LeadConfigItemPtr pLeadConfigItem);
	static void SaveCornerLead(DBConnectPtr pDBConnect, LeadConfigItemPtr pLeadConfigItem);

	// 关于“工序分配规则”。
	static void SaveComCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID, ComCutParamPtr pComCutParam);
	static void SaveCollisionAvoidParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID, const CollisionAvoidParam* pCollisionAvoidParam);
	static void SaveBdgeCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID, BridgeCutParamPtr pBdgeCutParam);
	static void SaveGridCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID, GridCutParamPtr pGridCutParam);
	static void SaveFlyCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID, FlyCutParamPtr pFlyCutParam);
	static void SaveChainCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID, ChainCutParamPtr pChainCutParam);
};

END_CUTLEADER_NAMESPACE()
