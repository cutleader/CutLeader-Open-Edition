#pragma once

#include "clExpertLibCommon.h"
#include <vector>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(ToolAssignConfigItem)
DECLARE_CUTLEADER_CLASS(ToolInstanceList)
DECLARE_CUTLEADER_CLASS(ToolAssignRuleList)
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
DECLARE_CUTLEADER_CLASS(ChainCutParam)
DECLARE_CUTLEADER_CLASS(DxfLayerToolMap)

BEGIN_CUTLEADER_NAMESPACE()

// 这个类从数据库加载“参数配置项”。
class ClExpertLib_Export ConfigItemLoader
{
public:
    // 加载刀具分配规则
    static ToolAssignConfigItemPtr LoadToolAssignConfigItem(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, ConditionTypeListPtr pAllCondTypeList);

	// 加载“微连接分配规则”。
	static MicroJointConfigItemPtr LoadMicroJointSetting(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// 加载“角特征分配规则”。
	static CornerConfigItemPtr LoadCornerConfigItem(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// 加载“引入引出线分配规则”。
	static LeadConfigItemPtr LoadLeadSetting(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// 加载“刺穿处理规则”。
	static PierceConfigItemPtr LoadPierceSetting(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	// 加载“NC代码生成规则”。
	// notes:
	//   1) we should refer to "expLib_NCConfigType" table to make the count of NCConfigValue equals to the count of "NCConfigType".
	static NCConfigItemPtr LoadNCConfigItem(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, NCConfigTypeListPtr pAllNCConfigTypeList);

	// 加载“工序分配规则”。
	static SequenceConfigItemPtr LoadSeqConfigItem(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

	static NestConfigItemPtr LoadNestConfigItem(DBConnectPtr pDBConnect, LONGLONG iParamConfigID);

private:
    // 关于刀具分配
    static ToolInstanceListPtr LoadToolInstanceList(DBConnectPtr pDBConnect, LONGLONG iToolAssignInfoID, ConditionTypeListPtr pAllCondTypeList); // 加载刀具
	static CondValInfoListPtr LoadCondVals_of_tool(DBConnectPtr pDBConnect, LONGLONG iToolInstanceID, ConditionTypeListPtr pAllCondTypeList); // 加载刀具上的条件值
    static ToolAssignRuleListPtr LoadToolAssignRules(DBConnectPtr pDBConnect, LONGLONG iToolAssignInfoID, ToolInstanceListPtr pToolInstances, bool bForInnerLoop); // 加载刀具分配规则
	static DxfLayerToolMapPtr LoadDxfLayerToolMap(DBConnectPtr pDBConnect, LONGLONG iToolAssignInfoID, ToolInstanceListPtr pToolInstances);

	// init the ranges of the MicroJointSetting.
	static void InitMJRanges(DBConnectPtr pDBConnect, LONGLONG iMicroJointSettingId, std::vector<MicroJointRangePtr>* pRangeList);

	// 加载角特征分配规则。
	static void LoadRangeCornerInfo(DBConnectPtr pDBConnect, LONGLONG cornerAssignInfoid, std::vector<CornerRangeInfoPtr>* rangeCornerInfos);

	// 有关“引入引出线规则”。
	static void LoadGeomLead(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, LeadConfigItemPtr pLeadConfigItem);
	static void LoadCornerLead(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, LeadConfigItemPtr pLeadConfigItem);

	// load NCConfigValue object by NCConfigType and 参数配置ID.
	static NCConfigValuePtr LoadNCConfigVal(DBConnectPtr pDBConnect, LONGLONG iParamConfigID, NCConfigTypePtr pNCConfigType);

	// 有关“工序分配规则”。
	static ComCutParamPtr LoadComCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID);
	static CollisionAvoidParamPtr LoadCollisionAvoidParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID);
	static BridgeCutParamPtr LoadBdgeCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID);
	static GridCutParamPtr LoadGridCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID);
	static FlyCutParamPtr LoadFlyCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID);
	static ChainCutParamPtr LoadChainCutParam(DBConnectPtr pDBConnect, LONGLONG iSeqConfigItemID);
};

END_CUTLEADER_NAMESPACE()
