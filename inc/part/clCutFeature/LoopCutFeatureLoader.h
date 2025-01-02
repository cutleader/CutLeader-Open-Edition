#pragma once

#include "clCutFeatureCommon.h"
#include <tuple>

DECLARE_CUTLEADER_CLASS(DBConnect)
DECLARE_CUTLEADER_CLASS(IPatternLoop)
DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(LoopToolData)
DECLARE_CUTLEADER_CLASS(LoopToolDataList)
DECLARE_CUTLEADER_CLASS(ConditionNodeList)
DECLARE_CUTLEADER_CLASS(LoopStartCutData)
DECLARE_CUTLEADER_CLASS(LoopCutFeatureList)
DECLARE_CUTLEADER_CLASS(ILead)
DECLARE_CUTLEADER_CLASS(LeadCondDataList)
DECLARE_CUTLEADER_CLASS(LoopMicroJoint)
DECLARE_CUTLEADER_CLASS(LoopMicroJointList)
DECLARE_CUTLEADER_CLASS(MicroJoint)
DECLARE_CUTLEADER_CLASS(MicroJointList)
DECLARE_CUTLEADER_CLASS(CornerList)
DECLARE_CUTLEADER_CLASS(LoopStartCutDataList)
DECLARE_CUTLEADER_CLASS(LoopStaEndCutCondList)
DECLARE_CUTLEADER_CLASS(LeadList)
DECLARE_CUTLEADER_CLASS(MicroJointDataList)
DECLARE_CUTLEADER_CLASS(MicroJointCondDataList)
DECLARE_CUTLEADER_CLASS(LoopCornerData)

BEGIN_CUTLEADER_NAMESPACE()

class ClCutFeature_Export LoopCutFeatureLoader
{
public:
	// load loop features of "iPartCamDataID".
	// 注:
	//   1) 如果某个“回路起切特征”无效了，那它将为空，调用者需要在后面来完善它。
	//   2) the cache data of loop feature will be updated.
	static LoopCutFeatureListPtr LoadLoopFeatures(DBConnectPtr pDBConnect, LONGLONG iCamDataID, PatternLoopListPtr pPatternLoopList);

    // 加载某个轮廓上的刀具信息
    static std::tr1::tuple<CString, double> LoadToolInfo_of_loop(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, LONGLONG iPatternLoopID);

private:
	// 关于轮廓刀具
	static LoopToolDataPtr LoadLoopTool(DBConnectPtr pDBConnect, IPatternLoopPtr pPatternLoop, LONGLONG iLoopToolID);
	static ConditionNodeListPtr LoadLoopConditionInfo(DBConnectPtr pDBConnect, LONGLONG iLoopToolID); // 加载轮廓上的条件信息

	/************************************************************************/
	// 关于“回路起切特征”。

	// 加载“回路起切特征”。
	/* params:
	*  pLeadCondDataList: all lead CondNodes under current cam data set.
	*/
	// notes:
	//   1) update the cache data of loop start/end info.
	static LoopStartCutDataPtr LoadLoopStaEndCut(DBConnectPtr pDBConnect, IPatternLoopPtr pLoop, LONGLONG iLoopStaEndCutId, LeadCondDataListPtr pLeadCondDataList);

	// load all loop pierce data of the "iPartCamDataID".
	// 注:
	//   1) 如果某个“回路起切特征”无效了，去除它。
	//   2) update the cache data of loop start/end info.
	static LoopStartCutDataListPtr LoadAllLoopStaEndCut(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, PatternLoopListPtr pPatternLoopList);

	// load LeadInOut object by id.
	static ILeadPtr LoadLeadInfo(DBConnectPtr pDBConnect, LONGLONG iId);

	// load 2 LeadInOut for the loop.
	/* params:
	*  pLeadCondDataList: all lead CondNodes under current cam data set.
	*/
	static LeadListPtr LoadLoopLeadInfo(DBConnectPtr pDBConnect, LONGLONG iLoopStaEndCutId, LeadCondDataListPtr pLeadCondDataList);

	// load all lead data of the "iPartCamDataID".
	// notes:
	//   1) call "LoadAllLeadCond" in this function.
	static LeadListPtr LoadAllLeadData(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID);
	/************************************************************************/

	/************************************************************************/
	// 关于“回路微连接特征”。

	static LoopMicroJointPtr LoadLoopMicroJoint(DBConnectPtr pDBConnect, IPatternLoopPtr pLoop, LONGLONG iLoopMicroJointId);

	// load all loop micro joint data of the part cam data set.
	// 注：
	//  1) 要是回路没有了，或者微连接所在图形没有了，那就丢弃微连接。
	static LoopMicroJointListPtr LoadAllLoopMicroJoint(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID, PatternLoopListPtr pPatternLoopList);

	static void LoadMicroJoints(DBConnectPtr pDBConnect, LONGLONG iLoopMicroJointid, MicroJointListPtr pMicroJointList);

	// load all micro joint object of the part cam data set.
	static MicroJointDataListPtr LoadAllMicroJoint(DBConnectPtr pDBConnect, LONGLONG iPartCamDataID);
	/************************************************************************/

	// 关于“回路角特征”。
	// 注：
	//  1) 无效的“角特征”需要删除。
	static LoopCornerDataPtr LoadLoopCorner(DBConnectPtr pDBConnect, IPatternLoopPtr pPatternLoop, LONGLONG iLoopCornerID);
	static void LoadCorners(DBConnectPtr pDBConnect, LONGLONG iLoopCornerID, CornerListPtr pCornerList);
};

END_CUTLEADER_NAMESPACE()
